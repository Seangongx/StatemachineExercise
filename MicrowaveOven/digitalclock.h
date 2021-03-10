#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>
#include <QtWidgets>
#include <QDebug>
#include <QTimer>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT
public:
    DigitalClock(QWidget *parent = 0);

public slots:
    void showCurrentTime();
    void showRestofTime(int s);
    void showCount(int s);
};

#endif // DIGITALCLOCK_H
