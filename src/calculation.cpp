/*
 * $Id: calculation.cpp 7 2014-11-14 13:57:37Z sein $
 */

#include "calculation.h"

Calculation::Calculation(){
    prog = 0;
    to_abort = false;
    is_running = false;
    num = -1;
    single = false;
    continuing = false;
    pos = 0;
}

void Calculation::setOptions(Options o){
    opts = o;
}

void Calculation::setSingle(){
    single = true;
}

void Calculation::run(){
    qDebug() << "Thread" << num << "started: " << from << to;
    ofstream file;
    is_running = true;
    int comb_length = Util::getCombLength(opts.get_mask());
    QList<QString> charsets = Util::prepareCharsets(opts);
    int to_add = 1;
    if(opts.get_out()){
        to_add = 2;
    }
    char comb[comb_length + to_add];
    if(opts.get_out()){
        comb[comb_length] = '\n';
        comb[comb_length + 1] = '\0';
    }
    else{
        comb[comb_length] = '\n';
        QString file_path = QApplication::applicationDirPath() + "/thread" + QString::number(num) + ".txt";
        if(single){
            file_path = opts.get_path() + ".txt";
        }
        if(continuing){
            file.open(file_path.toStdString().c_str(), ios::binary | ios::app);
        }
        else{
            file.open(file_path.toStdString().c_str(), ios::binary);
        }
        if(!file.good()){
            qDebug() << "Fatal error on opening thread file to write";
            return;
        }
    }
    QList<long long int> sep;
    sep.push_back(1);

    int match;
    bool matchI = opts.get_use_inst(), matchC = opts.get_use_count(), print_out;
    int count_l = opts.get_count_l().toInt(), count_u = opts.get_count_u().toInt(), count_n = opts.get_count_d().toInt(), count_s = opts.get_count_s().toInt();
    if(count_l <= 0){
        count_l = 100000;
    }
    if(count_u <= 0){
        count_u = 100000;
    }
    if(count_n <= 0){
        count_n = 100000;
    }
    if(count_s <= 0){
        count_s = 100000;
    }

    long long int run = 1;
    for(int x=comb_length-1;x>=0;x--){
        run *= charsets.at(x).length();
        sep.push_back(run);
    }
    for(long long int x=from;x<to;x++){
        if(continuing){
            x = pos;
            continuing = false;
            emit updateProgress((pos - from)/((to-from)/1250 + 1));
        }
        if(!is_running){
            while(!is_running){
                if(to_abort){
                    return;
                }
                sleep(1);
            }
        }
        if(x%((to-from)/1250 + 1) == 0){
            emit updateProgress(1);
        }
        if(to_abort){
            break;
        }
        run = x;
        for(int y=comb_length-1;y>=0;y--){
            long long int dif = run%sep.at(comb_length - y);
            run -= dif;
            comb[y] = charsets.at(y).at(dif/sep.at(comb_length - y - 1)).toLatin1();
        }
        match = 0;
        if(matchI && Util::matches(comb, comb_length, opts.get_num_dupl(), opts.get_num_inst())){
            match += 1;
        }
        if(matchC && Util::matches(comb, comb_length, count_l, count_u, count_n, count_s)){
            match += 2;
        }
        if(!matchI && !matchC){
            print_out = true;
        }
        else if(match == 0){
            print_out = false;
        }
        else if(matchI && matchC && match == 3){
            print_out = true;
        }
        else if(matchI && !matchC && match == 1){
            print_out = true;
        }
        else if(matchC && !matchI && match == 2){
            print_out = true;
        }
        else{
            print_out = false;
            qDebug() << "Fatal: logical error!";
        }
        if(print_out){
            if(opts.get_out()){
                cout << comb;
            }
            else{
                file.write(comb, comb_length + 1);
            }
        }
        pos = x;
        if(x%1000 == 0){
            cout.flush();
        }
    }
    is_running = false;
    cout.flush();
    if(!opts.get_out()){
        file.close();
    }
    emit finished();
    qDebug() << "Thread" << num << "finished";
}

void Calculation::setNum(int n){
    num = n;
}

void Calculation::setRunning(bool r){
    is_running = r;
}

void Calculation::setAbort(){
    to_abort = true;
}

void Calculation::setRange(long long int f, long long int t){
    from = f;
    to = t;
}

long long int Calculation::getPosition(){
    return pos;
}

void Calculation::setPosition(long long int p){
    pos = p;
    continuing = true;
}
