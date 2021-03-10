#include "digitalclock.h"

DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Flat);// Outline, Filled and Flat
}

void DigitalClock::showCurrentTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    setDigitCount(5);
    // glitter every second
    if ((time.second() % 2) == 0) { text[2] = ' ';}
    setDigitCount(5);
    setSegmentStyle(Flat);// Outline, Filled and Flat
    display(text);
}

void DigitalClock::showRestofTime(int s)
{
    if(s<0){
        return;
    }
    int mins = s/60, seconds = s % 60;
    //QString text;
    QString tm = mins < 10? QString("0") + QString::number(mins) : QString::number(mins);
    QString ts = seconds < 10? QString("0") + QString::number(seconds) : QString::number(seconds);
    QString text = QString("%1:%3").arg(tm).arg(ts);
    if ((s % 2) == 0) { text[2] = ' ';}
    setDigitCount(5);
    setSegmentStyle(Flat);
    qDebug() << text << endl;
    display(text);
}

void DigitalClock::showCount(int s)
{
    if(s<0){
        return;
    }
    QString text = QString("%1. ").arg(s);
    setDigitCount(5);
    setSegmentStyle(Flat);// Outline, Filled and Flat
    qDebug() << text << endl;
    display(text);
}


