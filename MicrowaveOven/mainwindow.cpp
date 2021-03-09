#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize:
    currentTimer = new QTimer(this);
    countTimer = new QTimer(this);
    mac = new QStateMachine();

    // States:
    stateWorking = new QState();
    stateCooking = new QState(stateWorking); //count down
    stateIdle = new QState(stateWorking); // display current time
    stateStop = new QState();

    // StateMachine:
    mac->addState(stateWorking);
    mac->addState(stateStop);
    mac->setInitialState(stateWorking);
    stateWorking->setInitialState(stateIdle);

    // InitialState Idle:
    countTimer->stop();
    currentTimer->start(1000);
    connect(currentTimer, SIGNAL(timeout()), ui->lcdClock, SLOT(showCurrentTime()));
    connect(countTimer, SIGNAL(timeout()), this, SLOT(tik_tok()));

    // Transitions:
    addTrans(stateIdle, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotIdle2Cooking()));
    addTrans(stateCooking, stateIdle, this, SIGNAL(FinishedCooking()), this, SLOT(slotCooking2Idle()));
    addTrans(stateCooking, stateCooking, ui->btnStart, SIGNAL(clicked()), this, SLOT(slotCooking2Cooking()));
    addTrans(stateWorking, stateStop, ui->btnStop, SIGNAL(clicked()), this, SLOT(slotWorking2Stop()));

    //stateIdle->addTransition(ui->btnStart, SIGNAL(clicked()),stateCooking);
    //connect(stateCooking, SIGNAL(entered()), this, SLOT(slotStart2Cooking()));
    //stateCooking->addTransition(this, SIGNAL(finishedCooking()), stateIdle);
    //connect(stateIdle, SIGNAL(entered()), this, SLOT(slotCooking2Idle()));
    //stateCooking->addTransition(ui->btnStart, SIGNAL(clicked()), stateCooking);
    //connect(stateCooking, SIGNAL(entered()), this, SLOT(slotCooking2Cooking()));
    //stateWorking->addTransition(ui->btnStop, SIGNAL(clicked()), stateStop);
    //connect(stateStop, SIGNAL(entered()), this, SLOT(slotWorking2Stop()));

    //stateStop->addTransition(this, SIGNAL(StopToIdle()), stateIdle);
    addTrans(stateStop, stateIdle, this, SIGNAL(StopToIdle()));

    mac->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotWorking2Stop() {
    qDebug() << "slotWorking2Stop" << endl;
    if(countTimer->isActive()) {
        countTimer->stop();
    }
    currentTimer->start(1000);
    m_secondCount = 0;
    emit StopToIdle();
}

void MainWindow::slotCooking2Idle() {
    qDebug() << "slotCooking2Idle" << endl;
    if(countTimer->isActive()) {
        countTimer->stop();
    }
    currentTimer->start(1000);
    m_secondCount = 0;
}

void MainWindow::slotIdle2Cooking() {
    qDebug() << "slotStart2Cooking" << endl;
    m_secondCount = SIXTYSECONDS;
    if(currentTimer->isActive())
        currentTimer->stop();
    countTimer->start(1000);
}

void MainWindow::slotCooking2Cooking() {
    qDebug() << "slotCooking2Cooking" << endl;
    if(countTimer->isActive() && !currentTimer->isActive())
        qDebug() << m_secondCount << "+" << SIXTYSECONDS << endl;
        m_secondCount += SIXTYSECONDS;
}

void MainWindow::tik_tok() {
    qDebug() << "tik_tok" << endl;
    if(m_secondCount < 0) {
        m_secondCount = 0;
        countTimer->stop();
        currentTimer->start(1000);
        emit FinishedCooking();
    }
    m_secondCount--;
    ui->lcdClock->showRestofTime(m_secondCount);
}

void MainWindow::showDebug() {
    qDebug() << "It is working!\n";
}

/*void MainWindow::on_btnStart_clicked()
{
    m_secondCount = SIXTYSECONDS;
    currentTimer->stop();
    countTimer->start();
}*/
