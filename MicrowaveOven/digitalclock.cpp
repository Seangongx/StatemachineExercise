#include "digitalclock.h"

DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Flat);// Outline, Filled and Flat
}

void DigitalClock::showDefaultTime()
{
    QTime time = QTime::currentTime();
    //qDebug() << time.hour() << "h" << time.minute() << "m" << time.second() << "s" << endl;
    QString text = time.toString("hh:mm");
    setDigitCount(5);
    // glitter every second
    if ((time.second() % 2) == 0) { text[2] = ' ';}
    setDigitCount(5);
    setSegmentStyle(Flat);// Outline, Filled and Flat
    display(text);
}

void DigitalClock::showSettingTime(int s)
{
    if(s<0){
        return;
    }
    s %=86400; // 24 hours
    int hours = s/3600, mins = (s / 60) % 60;
    //QString text;
    QString th = hours < 10? QString("0") + QString::number(hours) : QString::number(hours);
    QString tm = mins < 10? QString("0") + QString::number(mins) : QString::number(mins);
    QString text = QString("%1:%3").arg(th).arg(tm);
    if ((s % 2) == 0) { text[2] = ' ';}
    setDigitCount(5);
    setSegmentStyle(Flat);
    //qDebug() << text << endl;
    display(text);
}

void DigitalClock::showCount(int s)
{
    if(s<0){
        return;
    }
    QString text = QString::number(s);
    setDigitCount(5);
    setSegmentStyle(Flat);// Outline, Filled and Flat
    //qDebug() << text << endl;
    display(text);
}


