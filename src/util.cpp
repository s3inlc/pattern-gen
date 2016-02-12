/*
* Project created by QtCreator 2013-12-09T19:46:26
* Copyright 2014-2016 by Sein Coray
*/

#include "util.h"

const char *Util::charsU = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *Util::charsL = "abcdefghijklmnopqrstuvwxyz";
const char *Util::charsD = "0123456789";

Util::Util(){
    //
}

bool Util::matches(char *comb, int length, int d, int i){
    int num_inst = i;
    int num_dupl = d;


    int num = 0;
    char dupl[length];
    int m[length];
    for(int x=0;x<length;x++){
        m[x] = 0;
    }
    for(int x=0;x<length-1;x++){
        for(int y=x+1;y<length;y++){
            if(comb[x] == comb[y]){
                bool found = false;
                for(int z=0;z<num;z++){
                    if(dupl[z] == comb[x]){
                        found = true;
                    }
                }
                if(!found){
                    dupl[num] = comb[x];
                    num++;
                }
            }
        }
    }
    for(int x=0;x<length;x++){
        for(int y=0;y<num;y++){
            if(dupl[y] == comb[x]){
                m[y]++;
            }
        }
    }
    if(num_inst == 0 && num == 0){
        return true;
    }
    int inst=0;
    for(int x=0;x<num;x++){
        if(m[x] == num_dupl){
            inst++;
        }
        else if(m[x] > num_dupl){
            return false;
        }
    }
    if(inst == num_inst){
        return true;
    }
    return false;
}

QString Util::printTime(int n){
    QString res;
    int hours = n/3600;
    n -= hours*3600;
    int minutes = n/60;
    n -= minutes*60;
    int seconds = n;
    res = QString::number(hours) + "h " + QString::number(minutes) + "m " + QString::number(seconds) + "s";
    return res;
}

long long Util::power(int b, int p){
    long long int result = 1;
    for(int x=0;x<p;x++){
        result *= b;
    }
    return result;
}


QList<QString> Util::prepareCharsets(QString charset, Options opts){
    QList<QString> ret;
    QString mask = charset;
    for(int x=0;x<mask.length();x++){
        QChar act = mask.at(x);
        bool mask_used = false;
        if(mask.at(x) == '?'){
            if(x+1 >= mask.length()){
                ret.clear();
                return ret;
            }
            x++;
            mask_used = true;
            act = mask.at(x);
        }
        if(!mask_used){
            ret.push_back(mask.at(x));
            continue;
        }
        switch(act.toLatin1()){
        case '?':
            ret.push_back("?");
            break;
        case '1':
            ret.append(Util::convertCharset(Util::prepareCharsets(opts.get_custom1(), opts)));
            break;
        case '2':
            ret.append(Util::prepareCharsets(opts.get_custom2(), opts));
            break;
        case '3':
            ret.append(Util::prepareCharsets(opts.get_custom3(), opts));
            break;
        case '4':
            ret.append(Util::prepareCharsets(opts.get_custom4(), opts));
            break;
        case 'a':
            ret.push_back(Charsets::allCharset());
            break;
        case 'u':
            ret.push_back(Charsets::upperCharset());
            break;
        case 'l':
            ret.push_back(Charsets::lowerCharset());
            break;
        case 's':
            ret.push_back(Charsets::specialCharset());
            break;
        case 'h':
            ret.push_back(Charsets::hexCharset());
            break;
        case 'c':
            ret.push_back(Charsets::mixedAlphaCharset());
            break;
        case 'm':
            ret.push_back(Charsets::mixedAlphaNumsCharset());
            break;
        case 'd':
            ret.push_back(Charsets::numsCharset());
            break;
        case 'o':
            ret.push_back(Charsets::optimisedSpecialCharset());
            break;
        case 'H':
            ret.push_back(Charsets::hexUpperCharset());
            break;
        default:
            qDebug() << "Unknown mask value " << act;
            ret.clear();
            return ret;
            break;
        }
    }
    //qDebug() << "Calculated charset: " << ret;
    return ret;
}

QString Util::convertCharset(QList<QString> list){
    QString ret = "";
    for(int x=0;x<list.size();x++){
        ret += list.at(x);
    }
    return ret;
}

QList<QString> Util::prepareCharsets(Options opts){
    return prepareCharsets(opts.get_mask(), opts);
}

int Util::getCombLength(QString mask){
    int num = 0;
    for(int x=0;x<mask.length();x++){
        if(mask.at(x) == '?'){
            continue;
        }
        num++;
    }
    return num;
}

QList<long long int> Util::preCalc(Options opts){
    long long int num_comb = 1;
    QList<long long int> retval;

    QList<QString> ret = Util::prepareCharsets(opts);
    for(int x=0;x<ret.size();x++){
        num_comb *= ret.at(x).length();
    }
    /*QString mask = opts.get_mask();
    for(int x=0;x<mask.length();x++){
        QChar act = mask.at(x);
        bool mask_used = false;
        if(mask.at(x) == '?'){
            if(x+1 >= mask.length()){
                ret.clear();
                return ret;
            }
            x++;
            mask_used = true;
            act = mask.at(x);
        }
        if(!mask_used){
            continue;
        }
        switch(act.toLatin1()){
        case '1':
            num_comb *= opts.get_custom1().length();
            break;
        case '2':
            num_comb *= opts.get_custom2().length();
            break;
        case '3':
            num_comb *= opts.get_custom3().length();
            break;
        case '4':
            num_comb *= opts.get_custom4().length();
            break;
        case 'a':
            num_comb *= Charsets::allCharset().length();
            break;
        case 'u':
            num_comb *= Charsets::upperCharset().length();
            break;
        case 'l':
            num_comb *= Charsets::lowerCharset().length();
            break;
        case 's':
            num_comb *= Charsets::specialCharset().length();
            break;
        case 'h':
            num_comb *= Charsets::hexCharset().length();
            break;
        case 'c':
            num_comb *= Charsets::mixedAlphaCharset().length();
            break;
        case 'm':
            num_comb *= Charsets::mixedAlphaNumsCharset().length();
            break;
        case 'd':
            num_comb *= Charsets::numsCharset().length();
            break;
        case 'o':
            num_comb *= Charsets::optimisedSpecialCharset().length();
            break;
        default:
            qDebug() << "Unknown mask value " << act;
            ret.clear();
            return ret;
            break;
        }
    }*/
    retval.push_back(num_comb);
    return retval;
}

QString Util::printNum(long long int n){
    QString ret;
    int pre = n/Util::power(10, 10);
    QString val = "";
    if(pre > 0){
        val += QString::number(pre);
    }
    val += QString::number(n-pre*Util::power(10,10));
    for(int x=0;x<val.length();x++){
        ret += val.at(x);
        if((val.length() - x - 1)%3 == 0 && x != val.length() - 1){
            ret += "'";
        }
    }
    return ret;
}

QString Util::convertNum(long long int n){
    int pre = n/Util::power(10, 10);
    QString val = "";
    if(pre > 0){
        val += QString::number(pre);
    }
    val += QString::number(n - pre*Util::power(10,10));
    return val;
}

int Util::calcMaskLen(QString mask){
    int count = 0;
    bool set = false;
    for(int x=0;x<mask.length();x++){
        if(mask[x] != '?'){
            count++;
            set = false;
        }
        else{
            if(set){
                count++;
                set = false;
            }
            else{
                set = true;
            }
        }
    }
    return count;
}

long long Util::getStartNum(Options opts, QString start){
    if(start.length() != Util::calcMaskLen(opts.get_mask())){
        return -2; //not same length
    }
    QList<QString> charsets = Util::prepareCharsets(opts);
    long long int startnum = 0;
    for(int x=0;x<start.length();x++){
        if(!charsets.at(x).contains(start.at(x))){
            return -1; //invalid char
        }
        startnum *= charsets.at(x).length();
        startnum += charsets.at(x).indexOf(start.at(x));
    }
    return startnum;
}

bool Util::matches(char *comb, int length, int n_l, int n_u, int n_n, int n_s){
    int nl = 0, nu = 0, nn = 0, ns = 0;
    for(int x=0;x<length;x++){
        if(nl > n_l || nu > n_u || nn > n_n || ns > n_s){
            return false;
        }
        if(Util::contains(charsL, 26, comb[x])){
            nl++;
        }
        else if(Util::contains(charsD, 10, comb[x])){
            nn++;
        }
        else if(Util::contains(charsU, 26, comb[x])){
            nu++;
        }
        else{
            ns++;
        }
    }
    if(nl > n_l || nu > n_u || nn > n_n || ns > n_s){
        return false;
    }
    return true;
}

bool Util::contains(const char *set, int l, char c){
    for(int x=0;x<l;x++){
        if(set[x] == c){
            return true;
        }
    }
    return false;
}
