#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timer = new QTimer();
    ui->setupUi(this);
    //connect(sldRotationZ,valueChanged(int),NewWidget,SLOT(receiveRotationZ(int)));
    connect(timer,SIGNAL(timeout()),ui->NewWidget,SLOT(update()));
    timer->start(1000/60);
}

MainWindow::~MainWindow()
{
    delete ui;
}
