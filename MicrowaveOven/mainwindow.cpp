#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize:
    currentTimer = new QTimer(this);
    countTimer = currentTimer;
    mac = new QStateMachine();
    // States:
    stateWorking = new QState();
    stateCooking = new QState(stateWorking);
    stateIdle = new QState(stateWorking);
    stateStop = new QState();
    // StateMachine:
    mac->addState(stateWorking);
    mac->addState(stateStop);
    mac->setInitialState(stateWorking);
    stateWorking->setInitialState(stateIdle);


    // Events:
    connect(currentTimer, SIGNAL(timeout()), ui->lcdClock, SLOT(showCurrentTime()));
    connect(countTimer, SIGNAL(timeout()), this, SLOT(tik_tok()));
    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(on_btnStart_clicked()));
    //connect(ui->lcdClock, SIGNAL(CountTimeChanged(int)), ui->lcdClock, SLOT(showRestofTime(int)));
    //connect(currentTimer, SIGNAL(timeout()), this, SLOT(showDebug()));





    // Functions:
    currentTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showDebug() {
    qDebug() << "It is working!\n";
}

void MainWindow::tik_tok() {
    if(m_secondCount < 0) {
        m_secondCount = 0;
        countTimer->stop();
        currentTimer->start();
    }
    m_secondCount--;
    ui->lcdClock->showRestofTime(m_secondCount);
    //emit CountTimeChanged(m_secondCount);
}

void MainWindow::on_btnStart_clicked()
{
    m_secondCount = SIXTYSECONDS;
    currentTimer->stop();
    countTimer->start();
}
