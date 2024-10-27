#include "MyTime.h"

#include <string.h>
#include <stdio.h>

void Str2MyTime(const char *str, MyTime *t)
{
    const char *ptr = str;
    int num = 0;
    while (*ptr != '\0')
    {
        int tmpNum = 0;
        if (*ptr == ':') {
            t->minute = num;
            num = 0;
        }
        if (*ptr >= '0' && *ptr <= '9') {
            num = num * 10;
            tmpNum = *ptr - '0';
            num += tmpNum;
        }
        ptr++;
    }
    t->second = num;
}

void FixMyTime(MyTime *t)
{
    if (t->second > 60) {
        int overflowSecond = t->second - 60;
        t->minute += 1;
        t->second = overflowSecond;
    }

    if (t->minute >= 100) {
        t->minute = 99;
    }
}

int MyTime2Seconds(const MyTime *t)
{
    return (t->minute*60) + (t->second);
}

void Seconds2MyTime(int seconds, MyTime *t)
{
    t->minute = seconds / 60;
    t->second = seconds % 60;
}

int MyTime2Str(const MyTime *t, char *buffer, int bufLen)
{
    return snprintf(buffer, bufLen, "%02d:%02d", t->minute, t->second);
}

void MyTimeDec(MyTime *t)
{
    if (t->second == 00) {
        t->second = 60;
        t->minute = t->minute - 1;
    }
    t->second--;
}