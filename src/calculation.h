/*
* Project created by QtCreator 2013-12-09T19:46:26
* Copyright 2014-2016 by Sein Coray
*/

#ifndef CALCULATION_H
#define CALCULATION_H

#include <QThread>
#include <QProgressBar>
#include "options.h"
#include "util.h"
#include <fstream>
#include <iostream>

class Calculation : public QThread{
    Q_OBJECT

public:
    Calculation();
    void setOptions(Options o);
    void setRange(long long int f, long long int t);
    void setNum(int n);
    void setSingle();
    long long int getPosition();
    void setPosition(long long int p);

public slots:
    void setRunning(bool r);
    void setAbort();

signals:
    void updateProgress(int prog);
    void updateLog(QString add);
    void finished();

private:
    Options opts;
    int prog;
    bool is_running;
    bool to_abort;
    long long int from, to, pos;
    int num;
    bool single, continuing;

protected:
    void run();

};

#endif // CALCULATION_H
