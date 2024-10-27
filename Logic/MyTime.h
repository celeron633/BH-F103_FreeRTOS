#ifndef __MY_TIME_H__
#define __MY_TIME_H__

typedef struct MyTime {
    int second;
    int minute;
} MyTime;

void Str2MyTime(const char *str, MyTime *t);
void FixMyTime(MyTime *t);
int MyTime2Seconds(const MyTime *t);
void Seconds2MyTime(int seconds, MyTime *t);
int MyTime2Str(const MyTime *t, char *buffer, int bufLen);
void MyTimeDec(MyTime *t);

#endif