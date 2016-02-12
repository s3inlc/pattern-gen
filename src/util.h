/*
 * $Id: util.h 7 2014-11-14 13:57:37Z sein $
 */

#ifndef UTIL_H
#define UTIL_H

#include <QList>
#include "options.h"
#include <QTextBrowser>
#include <QProgressBar>
#include "options.h"
#include "calculation.h"
#include "charsets.h"
#include <math.h>

class Util{
public:
    Util();
    static QList<long long int> preCalc(Options);
    static QString printNum(long long int n);
    static int getCombLength(QString mask);
    static QList<QString> prepareCharsets(Options opts);
    static QList<QString> prepareCharsets(QString charset, Options opts);
    static QString convertCharset(QList<QString> list);
    static bool matches(char *comb, int length, int d, int i);
    static QString printTime(int n);
    static long long int power(int b, int p);
    static QString convertNum(long long int n);
    static int calcMaskLen(QString mask);
    static long long int getStartNum(Options opts, QString start);
    static bool matches(char *comb, int length, int n_l, int n_u, int n_n, int n_s);
    static bool contains(const char *set, int l, char c);

    static const char* charsU;
    static const char* charsL;
    static const char* charsD;
};

#endif // UTIL_H
