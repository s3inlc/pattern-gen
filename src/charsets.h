/*
* Project created by QtCreator 2013-12-09T19:46:26
* Copyright 2014-2016 by Sein Coray
*/

#ifndef CHARSETS_H
#define CHARSETS_H

#include <QString>

class Charsets{
public:
    Charsets();
    static QString allCharset();
    static QString upperCharset();
    static QString lowerCharset();
    static QString specialCharset();
    static QString hexCharset();
    static QString hexUpperCharset();
    static QString mixedAlphaCharset();
    static QString mixedAlphaNumsCharset();
    static QString numsCharset();
    static QString optimisedSpecialCharset();

private:
};

#endif // CHARSETS_H
