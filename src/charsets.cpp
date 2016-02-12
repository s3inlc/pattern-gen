/*
 * $Id: charsets.cpp 7 2014-11-14 13:57:37Z sein $
 */

#include "charsets.h"

Charsets::Charsets(){
    //
}

QString Charsets::allCharset(){
    return Charsets::mixedAlphaNumsCharset() + Charsets::specialCharset();
}

QString Charsets::upperCharset(){
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

QString Charsets::lowerCharset(){
    return "abcdefghijklmnopqrstuvwxyz";
}

QString Charsets::specialCharset(){
    return "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    //return "()/|\\-_.:$!?=*%&+<>";
}

QString Charsets::hexCharset(){
    return "0123456789abcdef";
}

QString Charsets::hexUpperCharset(){
    return "0123456789ABCDEF";
}

QString Charsets::mixedAlphaCharset(){
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

QString Charsets::mixedAlphaNumsCharset(){
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
}

QString Charsets::numsCharset(){
    return "0123456789";
}

QString Charsets::optimisedSpecialCharset(){
    return "!$*@#.()&,:_";
}
