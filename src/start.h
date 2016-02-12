/*
* Project created by QtCreator 2013-12-09T19:46:26
* Copyright 2014-2016 by Sein Coray
*/

#ifndef START_H
#define START_H

#include <QMainWindow>
#include "options.h"
#include "util.h"
#include <QFileDialog>
#include <time.h>
#include <QMessageBox>
#include <QTimer>

#define STATUS_INIT 0
#define STATUS_RUN 1
#define STATUS_PAUSE 2
#define STATUS_END 3

#define VERSION_BAR "Pattern-Gen v1.3.0 - Created by s3in!c - All rights reserved - www.smallutilities.org"

namespace Ui {
    class Start;
}

class Start : public QMainWindow{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();

private:
    Ui::Start *ui;
    Options opts;
    bool is_running;
    bool threading;
    int threads_finished;
    int NUM_CORES;
    long long int combinations;
    int last_bm;
    int session_status;
    long long int last_num;
    long long int total_num;
    Calculation *threads[8];
    QTimer updateTimer;

signals:
    void setRunning(bool r);
    void setAbort();

private slots:
    void on_mask_textChanged();
    void on_custom1_textChanged();
    void on_custom2_textChanged();
    void on_custom3_textChanged();
    void on_custom4_textChanged();
    void on_num_inst_textChanged();
    void on_num_dupl_textChanged();
    void on_path_textChanged();
    void on_multi_stateChanged();
    void on_multi_size_textChanged();
    void on_out_stateChanged();
    void on_precalc_clicked();
    void on_start_clicked();
    void on_threading_stateChanged();
    void on_setStart_stateChanged();
    void on_startFrom_textChanged();
    void on_use_inst_stateChanged();
    void on_use_count_stateChanged();
    void on_count_l_textChanged();
    void on_count_u_textChanged();
    void on_count_d_textChanged();
    void on_count_s_textChanged();

    void on_sel_path_clicked();
    void on_pause_clicked();
    void on_stop_clicked();
    void on_saveSession_clicked();
    void on_loadSession_clicked();

    void updateLog(QString s);
    void updateProgress(int n = 0);
    void finished();
};

#endif // START_H
