/*
 * $Id: main.cpp 3 2014-10-28 10:14:50Z sein $
 */

#include "start.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Start w;
    w.show();

    return a.exec();
}
