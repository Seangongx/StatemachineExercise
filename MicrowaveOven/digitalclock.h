#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>
#include <QtWidgets>
#include <QDebug>
#include <QTimer>
#include <QString>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT
public:
    DigitalClock(QWidget *parent = 0);

public slots:
    void showDefaultTime(QTime t);
    void showSettingTime(int s);
    void showRestofTime(int s);
    void showCount(int s);
};

#endif // DIGITALCLOCK_H
