/*
 * $Id: charsets.h 7 2014-11-14 13:57:37Z sein $
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
