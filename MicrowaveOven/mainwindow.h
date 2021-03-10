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

signals:
    //void CountTimeChanged(int s);
    void FinishedCooking();
    void StopToIdle();
    void DefrostToStart();

private:
    Ui::MainWindow *ui;

    QTimer *currentTimer;
    QTimer *countTimer;
    int m_secondCount = 0;

    QStateMachine * mac;
    QState * stateWorking;
    QState * stateCooking;
    QState * stateIdle;
    QState * stateStop;
    QState * stateDefrost;


};
#endif // MAINWINDOW_H
