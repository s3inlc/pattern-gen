/*
 * $Id: options.cpp 9 2015-02-18 17:10:49Z sein $
 */

#include "options.h"
using namespace std;

Options::Options(){
    path = QApplication::applicationDirPath() + "/options.ini";
    //qDebug() << "Options file:" << path;
    num_inst = 1;
    num_dupl = 2;
    mask = "";
    custom1 = "";
    custom2 = "";
    custom3 = "";
    custom4 = "";
    save_path = "";
    multi = false;
    multi_size = 0;
    out = false;
    start = false;
    startFrom = "";
    threading = false;
    use_inst = true;
    use_count = false;
    count_l = "0";
    count_u = "0";
    count_d = "0";
    count_s = "0";
    load();
}

void Options::save(){
    ofstream file;
    file.open(path.toStdString().c_str(), ios::binary);
    QString str;
    str = "mask=" + mask + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "custom1=" + custom1 + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "custom2=" + custom2 + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "custom3=" + custom3 + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "custom4=" + custom4 + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "num_inst=" + QString::number(num_inst) + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "num_dupl=" + QString::number(num_dupl) + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "path=" + save_path + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    QString val = "false";
    if(multi){
        val = "true";
    }
    str = "multi=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "multi_size=" + QString::number(multi_size)+"\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    val = "false";
    if(out){
        val = "true";
    }
    str = "stdout=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    val = "false";
    if(threading){
        val = "true";
    }
    str = "threading=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    val = "false";
    if(start){
        val = "true";
    }
    str = "start=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    val = "false";
    if(use_inst){
        val = "true";
    }
    str = "use_inst=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    val = "false";
    if(use_count){
        val = "true";
    }
    str = "use_count=" + val + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "startFrom=" + startFrom + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "count_l=" + count_l + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "count_u=" + count_u + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "count_d=" + count_d + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    str = "count_s=" + count_s + "\n";
    file.write(str.toStdString().c_str(), str.toStdString().length());
    file.close();
}

void Options::load(){
    ifstream file;
    file.open(path.toStdString().c_str(), ios::binary);
    if(!file.good()){
        qDebug() << "Options file couldn't be found! Create new one...";
        save();
        return;
    }
    string line;
    getline(file,line);
    while(!file.eof()){
        QString l = line.c_str();
        if(l.length() == 0){
            getline(file,line);
            continue;
        }
        QString prop = l.left(l.indexOf('='));
        QString val = l.right(l.length() - l.indexOf('=') - 1);

        if(prop.compare("mask") == 0){
            mask = val;
        }
        else if(prop.compare("custom1") == 0){
            custom1 = val;
        }
        else if(prop.compare("custom2") == 0){
            custom2 = val;
        }
        else if(prop.compare("custom3") == 0){
            custom3 = val;
        }
        else if(prop.compare("custom4") == 0){
            custom4 = val;
        }
        else if(prop.compare("num_inst") == 0){
            num_inst = val.toInt();
        }
        else if(prop.compare("num_dupl") == 0){
            num_dupl = val.toInt();
        }
        else if(prop.compare("path") == 0){
            save_path = val;
        }
        else if(prop.compare("startFrom") == 0){
            startFrom = val;
        }
        else if(prop.compare("count_l") == 0){
            count_l = val;
        }
        else if(prop.compare("count_u") == 0){
            count_u = val;
        }
        else if(prop.compare("count_d") == 0){
            count_d = val;
        }
        else if(prop.compare("count_s") == 0){
            count_s = val;
        }
        else if(prop.compare("multi") == 0){
            if(val.compare("true") == 0){
                multi = true;
            }
            else{
                multi = false;
            }
        }
        else if(prop.compare("threading") == 0){
            if(val.compare("true") == 0){
                threading = true;
            }
            else{
                threading = false;
            }
        }
        else if(prop.compare("start") == 0){
            if(val.compare("true") == 0){
                start = true;
            }
            else{
                start = false;
            }
        }
        else if(prop.compare("use_inst") == 0){
            if(val.compare("true") == 0){
                use_inst = true;
            }
            else{
                use_inst = false;
            }
        }
        else if(prop.compare("use_count") == 0){
            if(val.compare("true") == 0){
                use_count = true;
            }
            else{
                use_count = false;
            }
        }
        else if(prop.compare("multi_size") == 0){
            multi_size = val.toInt();
        }
        else if(prop.compare("stdout") == 0){
            if(val.compare("true") == 0){
                out = true;
            }
            else{
                out = false;
            }
        }
        else{
            qDebug() << "Unknown property: " << prop;
            getline(file,line);
            continue;
        }

        //qDebug() << "Property: " << prop << val;
        getline(file,line);
    }
    file.close();
}

void Options::update_threading(bool t){
    threading = t;
    save();
}

bool Options::get_threading(){
    return threading;
}

void Options::update_mask(QString m){
    mask = m;
    save();
}

QString Options::get_mask(){
    return mask;
}

void Options::update_custom1(QString c){
    custom1 = c;
    save();
}

QString Options::get_custom1(){
    return custom1;
}

void Options::update_custom2(QString c){
    custom2 = c;
    save();
}

QString Options::get_custom2(){
    return custom2;
}

void Options::update_custom3(QString c){
    custom3 = c;
    save();
}

QString Options::get_custom3(){
    return custom3;
}

void Options::update_custom4(QString c){
    custom4 = c;
    save();
}

QString Options::get_custom4(){
    return custom4;
}

void Options::update_num_inst(int n){
    num_inst = n;
    save();
}

int Options::get_num_inst(){
    return num_inst;
}

void Options::update_num_dupl(int n){
    num_dupl = n;
    save();
}

int Options::get_num_dupl(){
    return num_dupl;
}

void Options::update_path(QString p){
    save_path = p;
    save();
}

QString Options::get_path(){
    return save_path;
}

void Options::update_multi(bool m){
    multi = m;
    save();
}

bool Options::get_multi(){
    return multi;
}

void Options::update_multi_size(int s){
    multi_size = s;
    save();
}

int Options::get_multi_size(){
    return multi_size;
}

void Options::update_out(bool o){
    out = o;
    save();
}

bool Options::get_out(){
    return out;
}

void Options::update_start(bool s){
    start = s;
    save();
}

bool Options::get_start(){
    return start;
}

void Options::update_startFrom(QString s){
    startFrom = s;
    save();
}

QString Options::get_startFrom(){
    return startFrom;
}

void Options::SaveToPath(QString p){
    path = p;
    save();
    path = QApplication::applicationDirPath() + "/options.ini";
}

void Options::LoadFromPath(QString p){
    path = p;
    load();
    path = QApplication::applicationDirPath() + "/options.ini";
    save();
}

void Options::update_use_inst(bool u){
    use_inst = u;
    save();
}

bool Options::get_use_inst(){
    return use_inst;
}

void Options::update_use_count(bool u){
    use_count = u;
    save();
}

bool Options::get_use_count(){
    return use_count;
}

void Options::update_count_l(QString c){
    count_l = c;
    save();
}

QString Options::get_count_l(){
    return count_l;
}

void Options::update_count_u(QString c){
    count_u = c;
    save();
}

QString Options::get_count_u(){
    return count_u;
}

void Options::update_count_d(QString c){
    count_d = c;
    save();
}

QString Options::get_count_d(){
    return count_d;
}

void Options::update_count_s(QString c){
    count_s = c;
    save();
}

QString Options::get_count_s(){
    return count_s;
}









