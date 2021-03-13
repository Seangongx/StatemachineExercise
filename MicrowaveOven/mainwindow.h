#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "digitalclock.h"
#include <QStateMachine>
#include <QState>
#include <QDebug>
#include "Transitions.h"
#include <QtGlobal> // random number

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const int SIXTYSECONDS = 10;

public slots:
    // Class Slots
    void showDebug();
    void tik_tok();
    void showDial();
    int getRandom(int min, int max);
    // State Slots:
    void slotIdle2Cooking();
    void slotIdle2Defrost();
    void slotCooking2Idle();
    void slotCooking2Cooking();
    void slotWorking2Stop();
    void slotDefrost2Cooking();
    void slotStop2Idle();
    void slotIdle2Clock();
    void slotClock2Clock();
    void slotClock2Idle();

signals:
    //void CountTimeChanged(int s);
    void FinishedCooking();
    void StopToIdle();
    void DefrostToStart();

private:
    Ui::MainWindow *ui;

    QTimer *defaultTimer; // Default Timer
    QTimer *settingTimer; // Setting Timer
    QTimer *countTimer;   // Countdown Timer
    int m_countSeconds = 0;
    int m_settingSeconds = 0;
    int m_power = 0;
    bool m_setClock = false;

    QStateMachine * mac;
    QState * stateWorking;
    QState * stateCooking;
    QState * stateIdle;
    QState * stateStop;
    QState * stateDefrost;
    QState * stateClock;
    QState * stateMode;
    QState * statePower;
    QState * stateDuration;
    QState * stateSetHour;
    QState * stateSetMinute;

};
#endif // MAINWINDOW_H
