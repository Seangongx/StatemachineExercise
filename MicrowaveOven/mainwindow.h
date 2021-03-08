#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "digitalclock.h"
#include <QStateMachine>
#include <QState>
#include <QDebug>

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

    void on_btnStart_clicked();
    void showDebug();
    void tik_tok();
signals:
    void CountTimeChanged(int s);

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


};
#endif // MAINWINDOW_H
