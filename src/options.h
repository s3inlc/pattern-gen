/*
 * $Id: options.h 7 2014-11-14 13:57:37Z sein $
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QDebug>
using namespace std;

class Options{
public:
    Options();

    void update_mask(QString m);
    QString get_mask();
    void update_custom1(QString c);
    QString get_custom1();
    void update_custom2(QString c);
    QString get_custom2();
    void update_custom3(QString c);
    QString get_custom3();
    void update_custom4(QString c);
    QString get_custom4();
    void update_num_inst(int);
    int get_num_inst();
    void update_num_dupl(int);
    int get_num_dupl();
    void update_path(QString p);
    QString get_path();
    void update_multi(bool m);
    bool get_multi();
    void update_multi_size(int s);
    int get_multi_size();
    void update_out(bool o);
    bool get_out();
    void update_threading(bool t);
    bool get_threading();
    void update_startFrom(QString s);
    QString get_startFrom();
    void update_start(bool s);
    bool get_start();
    void update_use_inst(bool u);
    bool get_use_inst();
    void update_use_count(bool u);
    bool get_use_count();
    void update_count_l(QString c);
    QString get_count_l();
    void update_count_u(QString c);
    QString get_count_u();
    void update_count_d(QString c);
    QString get_count_d();
    void update_count_s(QString c);
    QString get_count_s();

    void SaveToPath(QString p);
    void LoadFromPath(QString p);

private:
    void load();
    void save();
    QString path;

    QString mask;
    QString custom1;
    QString custom2;
    QString custom3;
    QString custom4;
    QString startFrom;
    bool start;
    bool use_inst;
    bool use_count;
    QString count_l, count_u, count_d, count_s;
    int num_inst, num_dupl;
    QString save_path;
    bool multi;
    int multi_size;
    bool out;
    bool threading;
};

#endif // OPTIONS_H
