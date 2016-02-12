/*
 * $Id: start.cpp 10 2015-02-18 17:37:42Z sein $
 */

#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent) : QMainWindow(parent), ui(new Ui::Start){
    ui->setupUi(this);

    //set status bar with info
    ui->statusBar->setText(VERSION_BAR);

    //load option info
    ui->mask->setText(opts.get_mask());
    ui->custom1->setText(opts.get_custom1());
    ui->custom2->setText(opts.get_custom2());
    ui->custom3->setText(opts.get_custom3());
    ui->custom4->setText(opts.get_custom4());
    ui->num_inst->setText(QString::number(opts.get_num_inst()));
    ui->num_dupl->setText(QString::number(opts.get_num_dupl()));
    ui->path->setText(opts.get_path());
    ui->multi->setChecked(opts.get_multi());
    ui->multi_size->setText(QString::number(opts.get_multi_size()));
    ui->out->setChecked(opts.get_out());
    ui->setStart->setChecked(opts.get_start());
    ui->startFrom->setText(opts.get_startFrom());
    ui->threading->setChecked(opts.get_threading());
    ui->use_count->setChecked(opts.get_use_count());
    ui->use_inst->setChecked(opts.get_use_inst());
    ui->count_d->setText(opts.get_count_d());
    ui->count_l->setText(opts.get_count_l());
    ui->count_s->setText(opts.get_count_s());
    ui->count_u->setText(opts.get_count_u());

    //reset variables
    is_running = false;
    threading = false;
    last_bm = 0;
    ui->progress->setValue(0);
    session_status = STATUS_INIT;
    QObject::connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateProgress()));
}

Start::~Start(){
    delete ui;
}

void Start::on_setStart_stateChanged(){
    opts.update_start(ui->setStart->isChecked());
}

void Start::on_startFrom_textChanged(){
    opts.update_startFrom(ui->startFrom->text());
    ui->startFrom_len->setText("[" + QString::number(ui->startFrom->text().length()) + "]");
}

void Start::on_threading_stateChanged(){
    opts.update_threading(ui->threading->isChecked());
}

void Start::on_start_clicked(){
    if(is_running){
        return; //process is already running
    }

    //calculate the number of combinations to check
    QList<long long int> pre = Util::preCalc(opts);
    if(pre.length() == 0){
        ui->log->append("Error in preCalc!<br>");
        return;
    }
    combinations = pre.at(0);

    //handle settings for multithreading
    int multi = 1;
    threading = false;
    if(ui->threading->isChecked()){
        multi = 8;
        threading = true;
    }
    NUM_CORES = multi;

    //check if a start combination was set and calculate the start position if neccessary
    long long int last_num = 0;
    if(ui->setStart->isChecked()){
        long long int start = Util::getStartNum(opts, ui->startFrom->text());
        if(start < 0){
            qDebug() << "ERROR: Wrong/invalid start combination!";
            return;
        }
        last_num = start;
        combinations -= last_num;
        qDebug() << "Last num set to:" << last_num;
    }

    //create the required cores
    QList<long long int> cores;
    for(int x=0;x<NUM_CORES;x++){
        long long int end = combinations/NUM_CORES*(x + 1) + last_num;
        if(x == NUM_CORES-1){
            end = combinations + last_num;
        }
        cores.push_back(end);
    }

    //reset progress
    ui->progress->setValue(0);
    ui->progress->setRange(0, 10000);
    ui->status->setText("Current speed: updating -- Time left: updating");
    total_num = combinations;

    //create all threads, connect and start them
    threads_finished = 0;
    for(int x=0;x<NUM_CORES;x++){
        Calculation *thread = new Calculation();
        thread->setOptions(opts);
        thread->setRange(last_num, cores.at(x));
        thread->setNum(x);
        if(!threading){
            thread->setSingle();
        }
        last_num = cores.at(x);
        QObject::connect(thread, SIGNAL(updateLog(QString)), this, SLOT(updateLog(QString)));
        QObject::connect(thread, SIGNAL(updateProgress(int)), this, SLOT(updateProgress(int)));
        QObject::connect(this, SIGNAL(setAbort()), thread, SLOT(setAbort()));
        QObject::connect(this, SIGNAL(setRunning(bool)), thread, SLOT(setRunning(bool)));
        QObject::connect(thread, SIGNAL(finished()), this, SLOT(finished()));
        thread->start();
        threads[x] = thread;
    }
    is_running = true;
    session_status = STATUS_RUN;
    updateTimer.setInterval(2000);
    updateTimer.start();
}

void Start::finished(){
    threads_finished++;
    //check if all threads are finished now
    if(threads_finished >= NUM_CORES){
        ui->status->clear();
        ui->log->append("Processes are finished!");
        if(opts.get_threading() && !opts.get_out()){
            //we use multicores and need to write it into a file
            ui->log->append("Now merging...");
            ui->log->update();
            ui->progress->setValue(0);
            ui->progress->setRange(0,100000);
            ui->progress->update();

            //open output file
            ofstream ofile;
            QString ofile_path = opts.get_path() + ".txt";
            long long int filecount = 0;
            int file_num = 0;
            if(opts.get_multi()){
                ofile_path = opts.get_path() + "_" + QString::number(file_num) + ".txt";
            }
            ofile.open(ofile_path.toStdString().c_str(), ios::binary);
            if(!ofile.good()){
                qDebug() << "Fatal error in opening final file!";
                return;
            }

            //read thread files
            ifstream ifile;
            long long int count = 0;
            for(int x=0;x<NUM_CORES;x++){
                QString ifile_path = QApplication::applicationDirPath() + "/thread" + QString::number(x) + ".txt";
                ifile.open(ifile_path.toStdString().c_str(), ios::binary);
                if(!ifile.good()){
                    qDebug() << "Fatal error in reading temp thread file!";
                    ui->progress->setValue(0);
                    ui->progress->update();
                    session_status = STATUS_INIT;
                    return;
                }
                string line;
                getline(ifile, line);
                while(!ifile.eof()){
                    line += "\n";
                    ofile.write(line.c_str(), line.length());
                    filecount += line.length();
                    if(opts.get_multi() && filecount > 1024*1024*opts.get_multi_size()){
                        //if multiple files is selected, need to increment when size is reached
                        ofile.close();
                        filecount = 0;
                        file_num++;
                        ofile_path = opts.get_path() + "_" + opts.get_mask() + "_" + QString::number(file_num) + ".txt";
                        ofile.open(ofile_path.toStdString().c_str(), ios::binary);
                        if(!ofile.good()){
                            qDebug() << "Fatal error in opening final file!";
                            session_status = STATUS_INIT;
                            return;
                        }
                    }
                    if(count%100000 == 0){
                        ui->progress->setValue(count*100000/combinations);
                        ui->progress->update();
                    }
                    getline(ifile, line);
                    count++;
                }
                ifile.close();
                QFile::remove(ifile_path);
            }
            ofile.close();
            ui->log->append("Merging done!<br>");
        }
        is_running = false;
        ui->progress->setValue(0);
        session_status = STATUS_END;
        updateTimer.stop();
    }
    last_bm = 0;
}

void Start::on_stop_clicked(){
    if(session_status != STATUS_RUN && session_status != STATUS_PAUSE && !is_running){
        return;
    }
    if(threading){
        //ask user if he want to save incomplete output
        int ret = QMessageBox::warning( 0, "Mergin process mode",
                                        "Do you want to save the generated patterns?",
                                        "Yes", "No", QString::null, 0, 1 );
        if(ret == 0){
            ui->log->append("Now merging...");
            ui->log->update();
            ui->progress->setValue(0);
            ui->progress->setRange(0,100000);
            ui->progress->update();
            ofstream ofile;
            QString ofile_path = opts.get_path() + ".txt";
            long long int filecount = 0;
            int file_num = 0;
            if(opts.get_multi()){
                ofile_path = opts.get_path() + "_" + QString::number(file_num) + ".txt";
            }
            ofile.open(ofile_path.toStdString().c_str(), ios::binary);
            if(!ofile.good()){
                qDebug() << "Fatal error in opening final file!";
                return;
            }
            ifstream ifile;
            long long int count = 0;
            for(int x=0;x<NUM_CORES;x++){
                QString ifile_path = QApplication::applicationDirPath() + "/thread" + QString::number(x) + ".txt";
                ifile.open(ifile_path.toStdString().c_str(), ios::binary);
                if(!ifile.good()){
                    qDebug() << "Fatal error in reading temp thread file!";
                    ui->progress->setValue(0);
                    ui->progress->update();
                    return;
                }
                string line;
                getline(ifile, line);
                while(!ifile.eof()){
                    line += "\n";
                    ofile.write(line.c_str(), line.length());
                    filecount += line.length();
                    if(opts.get_multi() && filecount > 1024*1024*opts.get_multi_size()){
                        ofile.close();
                        filecount = 0;
                        file_num++;
                        ofile_path = opts.get_path() + "_" + opts.get_mask() + "_" + QString::number(file_num) + ".txt";
                        ofile.open(ofile_path.toStdString().c_str(), ios::binary);
                        if(!ofile.good()){
                            qDebug() << "Fatal error in opening final file!";
                            return;
                        }
                    }
                    if(count%100000 == 0){
                        ui->progress->setValue(count*100000/combinations);
                        ui->progress->update();
                    }
                    getline(ifile, line);
                    count++;
                }
                ifile.close();
                QFile::remove(ifile_path);
            }
            ofile.close();
            ui->log->append("Merging done!<br>");
        }
        else{
            //delete thread files
            for(int x=0;x<NUM_CORES;x++){
                QString ifile_path = QApplication::applicationDirPath() + "/thread" + QString::number(x) + ".txt";
                QFile::remove(ifile_path);
            }
        }
    }
    emit setAbort();
    is_running = false;
    session_status = STATUS_INIT;
    ui->progress->setValue(0);
    ui->status->clear();
    ui->pause->setText("Pause");
    updateTimer.stop();
    last_bm = 0;
}

void Start::on_pause_clicked(){
    if(is_running){
        emit setRunning(false);
        is_running = false;
        ui->pause->setText("Resume");
        ui->status->setText("Current speed: paused -- Time left: paused");
        updateTimer.stop();
        session_status = STATUS_PAUSE;
    }
    else if(!is_running && ui->pause->text().compare("Resume") == 0){
        emit setRunning(true);
        is_running = true;
        ui->pause->setText("Pause");
        ui->status->setText("Current speed: updating -- Time left: updating");
        updateTimer.start();
        session_status = STATUS_RUN;
    }
    last_bm = 0;
}

void Start::updateLog(QString s){
    ui->log->append(s);
}

void Start::updateProgress(int n){
    if(!threading){
        n *= 8;
    }
    if(last_bm == 0){
        last_bm = time(NULL);
        last_num = ui->progress->value();
    }
    else if(time(NULL) - last_bm > 1){
        int speed = (ui->progress->value() - last_num) * total_num /10000;
        last_bm = time(NULL);
        last_num = ui->progress->value();
        long long int time_left = 0;
        if(speed == 0){
            time_left = LONG_LONG_MAX;
        }
        else{
            time_left = (total_num - (ui->progress->value()* total_num /10000))/speed;
        }
        ui->status->setText("Current speed: " + Util::printNum(speed) + "/s -- Time left: " + Util::printTime(time_left));
    }
    ui->progress->setValue(ui->progress->value() + n);
    ui->progress->update();
}

void Start::on_precalc_clicked(){
    QList<long long int> ret = Util::preCalc(opts);
    if(ret.size() == 0){
        qDebug() << "Error in preCalc!";
        return;
    }
    long long int num_comb = ret.at(0);
    long long int last_num = 0;
    if(ui->setStart->isChecked()){
        long long int start = Util::getStartNum(opts, ui->startFrom->text());
        if(start < 0){
            qDebug() << "ERROR: Wrong/invalid start combination!";
            return;
        }
        last_num = start;
        num_comb -= last_num;
    }
    ui->log->append("<h3>Precalc done</h3>" + Util::printNum(num_comb) + " combinations will be checked.<br><br>");
}

void Start::on_multi_stateChanged(){
    opts.update_multi(ui->multi->isChecked());
}

void Start::on_custom1_textChanged(){
    opts.update_custom1(ui->custom1->text());
}

void Start::on_custom2_textChanged(){
    opts.update_custom2(ui->custom2->text());
}

void Start::on_custom3_textChanged(){
    opts.update_custom3(ui->custom3->text());
}

void Start::on_custom4_textChanged(){
    opts.update_custom4(ui->custom4->text());
}

void Start::on_mask_textChanged(){
    opts.update_mask(ui->mask->text());
    int len = Util::calcMaskLen(ui->mask->text());
    ui->mask_len->setText("[" + QString::number(len) + "]");
}

void Start::on_num_dupl_textChanged(){
    opts.update_num_dupl(ui->num_dupl->text().toInt());
}

void Start::on_num_inst_textChanged(){
    opts.update_num_inst(ui->num_inst->text().toInt());
}

void Start::on_sel_path_clicked(){
    QString path = QFileDialog::getSaveFileName(this, "Select the basename to save to");
    if(path.length() == 0){
        return;
    }
    ui->path->setText(path);
    opts.update_path(path);
}

void Start::on_path_textChanged(){
    opts.update_path(ui->path->text());
}

void Start::on_multi_size_textChanged(){
    opts.update_multi_size(ui->multi_size->text().toInt());
}

void Start::on_out_stateChanged(){
    opts.update_out(ui->out->isChecked());
}

void Start::on_saveSession_clicked(){
    if(is_running){
        on_pause_clicked();
    }
    if(session_status != STATUS_RUN && session_status != STATUS_PAUSE){
        updateLog("\nYou cannot save a session now as there is no running action!\n");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, "Select the basename to save the session to");
    if(path.length() == 0){
        return;
    }
    QString sesspath;
    if(threading){
        QString progress = "";
        for(int x=0;x<NUM_CORES;x++){
            QString oldpath = QApplication::applicationDirPath() + "/thread"+QString::number(x) + ".txt";
            QString savepath = path + "_thread" + QString::number(x)+".txt";
            QFile file(oldpath);
            if(!file.copy(savepath)){
                ui->log->append("Error in saving session. Probably session name already is used?");
                qDebug() << file.errorString();
                return;
            }
            if(threads[x]->getPosition() < 0){
                progress += "thread" + QString::number(x) + ":0\n";
            }
            else{
                progress += "thread" + QString::number(x) + ":"+Util::convertNum(threads[x]->getPosition()) + "\n";
            }
        }
        opts.SaveToPath(path + "_opts.txt");
        ofstream f;
        sesspath = path + ".sess";
        f.open(sesspath.toStdString().c_str(), ios::binary);
        f << progress.toStdString();
        f.close();
    }
    else{
        opts.SaveToPath(path + "_opts.txt");
        QString progress = "";
        if(threads[0]->getPosition() < 0){
            progress += "thread:0\n";
        }
        else{
            progress += "thread:" + Util::convertNum(threads[0]->getPosition()) + "\n";
        }
        opts.SaveToPath(path + "_opts.txt");
        ofstream f;
        sesspath = path + ".sess";
        f.open(sesspath.toStdString().c_str(), ios::binary);
        f << progress.toStdString();
        f.close();
    }
    ui->log->append("Saved session to: " + sesspath);
}

void Start::on_loadSession_clicked(){
    if(is_running){
        ui->log->append("There is already a process running...");
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select the *.sess file to load");
    if(path.length()==0){
        return;
    }
    emit setAbort();
    is_running = false;
    ui->progress->setValue(0);
    ui->status->clear();
    last_bm = 0;
    ifstream f;
    f.open(path.toStdString().c_str(), ios::binary);
    if(!f.good()){
        ui->log->append("File could not be opened!");
        return;
    }
    QFileInfo o(path);
    QString opts_path = o.absolutePath() + "/" + o.completeBaseName();
    string line;
    getline(f, line);
    long long int pos_def[8];
    if(line.find("thread0") == std::string::npos){
        string pos = line.substr(8, line.length() - 8);
        qDebug() << pos.c_str();
        QString p = pos.c_str();
        pos_def[0] = p.toLongLong();
    }
    else{
        int count = 0;
        while(!f.eof()){
            string pos = line.substr(8, line.length() - 8);
            qDebug() << pos.c_str();
            getline(f, line);
            QString oldpath = QApplication::applicationDirPath() + "/thread" + QString::number(count) + ".txt";
            QString savepath = o.absolutePath() + "/" + o.completeBaseName() + "_thread" + QString::number(count) + ".txt";
            QFile nf(savepath);
            QFile of(oldpath);
            of.remove();
            nf.rename(oldpath);
            QString p = pos.c_str();
            pos_def[count] = p.toLongLong();
            count++;
        }
    }
    f.close();
    QFile p(path);
    p.remove();
    opts.LoadFromPath(opts_path + "_opts.txt");
    p.setFileName(opts_path + "_opts.txt");
    p.remove();
    ui->mask->setText(opts.get_mask());
    ui->custom1->setText(opts.get_custom1());
    ui->custom2->setText(opts.get_custom2());
    ui->custom3->setText(opts.get_custom3());
    ui->custom4->setText(opts.get_custom4());
    ui->num_inst->setText(QString::number(opts.get_num_inst()));
    ui->num_dupl->setText(QString::number(opts.get_num_dupl()));
    ui->path->setText(opts.get_path());
    ui->multi->setChecked(opts.get_multi());
    ui->multi_size->setText(QString::number(opts.get_multi_size()));
    ui->out->setChecked(opts.get_out());
    ui->threading->setChecked(opts.get_threading());
    is_running = false;
    threading = false;
    last_bm = 0;
    ui->progress->setValue(0);

    if(is_running){
        return;
    }
    QList<long long int> pre = Util::preCalc(opts);
    if(pre.length() == 0){
        ui->log->append("Error in preCalc!<br>");
        return;
    }
    combinations = pre.at(0);
    int multi = 1;
    threading = false;
    if(ui->threading->isChecked()){
        multi = 8;
        threading = true;
    }
    NUM_CORES = multi;
    QList<long long int> cores;
    for(int x=0;x<NUM_CORES;x++){
        long long int end = combinations/NUM_CORES*(x+1);
        if(x == NUM_CORES-1){
            end = combinations;
        }
        cores.push_back(end);
    }
    ui->progress->setValue(0);
    ui->progress->setRange(0, 10000);
    ui->status->setText("Current speed: updating -- Time left: updating");
    total_num = combinations;
    long long int last_num = 0;
    if(ui->setStart->isChecked()){
        int start = Util::getStartNum(opts, ui->startFrom->text());
        if(start < 0){
            qDebug() << "ERROR: Wrong/invalid start combination!";
            return;
        }
        last_num = start;
        qDebug() << "Last num set to:" << last_num;
    }
    threads_finished = 0;
    for(int x=0;x<NUM_CORES;x++){
        Calculation *thread = new Calculation();
        thread->setOptions(opts);
        thread->setRange(last_num, cores.at(x));
        thread->setNum(x);
        thread->setPosition(pos_def[x]);
        if(!threading){
            thread->setSingle();
        }
        last_num = cores.at(x);
        QObject::connect(thread, SIGNAL(updateLog(QString)), this, SLOT(updateLog(QString)));
        QObject::connect(thread, SIGNAL(updateProgress(int)), this, SLOT(updateProgress(int)));
        QObject::connect(this, SIGNAL(setAbort()), thread, SLOT(setAbort()));
        QObject::connect(this, SIGNAL(setRunning(bool)), thread, SLOT(setRunning(bool)));
        QObject::connect(thread, SIGNAL(finished()), this, SLOT(finished()));
        thread->start();
        threads[x] = thread;
    }
    is_running = true;
    ui->pause->setText("Pause");
}

void Start::on_use_inst_stateChanged(){
    opts.update_use_inst(ui->use_inst->isChecked());
}

void Start::on_use_count_stateChanged(){
    opts.update_use_count(ui->use_count->isChecked());
}

void Start::on_count_l_textChanged(){
    opts.update_count_l(QString::number(ui->count_l->text().toInt()));
    ui->count_l->setText(opts.get_count_l());
}

void Start::on_count_u_textChanged(){
    opts.update_count_u(QString::number(ui->count_u->text().toInt()));
    ui->count_u->setText(opts.get_count_u());
}

void Start::on_count_d_textChanged(){
    opts.update_count_d(QString::number(ui->count_d->text().toInt()));
    ui->count_d->setText(opts.get_count_d());
}

void Start::on_count_s_textChanged(){
    opts.update_count_s(QString::number(ui->count_s->text().toInt()));
    ui->count_s->setText(opts.get_count_s());
}
