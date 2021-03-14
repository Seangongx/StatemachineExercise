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
    void showCurrentTime();
    void tik_tok_up();
    void tik_tok_down();
    void showDial();
    int getRandom(int min, int max);
    // State Slots:
    void slotIdle2Cooking();
    void slotIdle2Defrost();
    void slotCooking2Idle();
    void slotCooking2Cooking();
    void slotWorking2Stop();
    void slotDefrost2Cooking();
    //void slotStop2Idle();
    void slotIdle2Clock();
    void slotClock2Clock();
    void slotClock2Idle();
    void slotIdle2Power();
    void slotPower2Duration();
    void slotDuration2Cooking();
    void slotIdle2Mode();
    void slotMode2Duration();

signals:
    //void CountTimeChanged(int s);
    void FinishedCooking();
    void StopToIdle();
    void DefrostToStart();

private:
    Ui::MainWindow *ui;
    enum Mode {microwave = 1, grill, microwave_grill, error};

    QTimer *defaultTimer; // Default Timer
    QTimer *settingTimer; // Setting Timer
    QTimer *countTimer;   // Countdown Timer
    int m_countSeconds = 0;
    int m_settingSeconds = 0;
    int m_power = 0;
    Mode m_mode = error;
    bool m_setClock = false;
    bool m_setMode = false;
    bool m_blockDisplay = false;

    QStateMachine * mac;
    QState * stateWorking;
    QState * stateCooking;
    QState * stateIdle;
    QState * stateStop;
    QState * stateDefrost;
    //QState * stateClock;
    QState * stateMode;
    QState * statePower;
    QState * stateDuration;
    QState * stateSetHour;
    QState * stateSetMinute;
    QState * stateSetPower;
    QState * stateSetMode;
    QState * stateSetDuration;

};
#endif // MAINWINDOW_H
