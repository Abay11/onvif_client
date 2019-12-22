#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "device.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    using namespace _onvif;
    IDevice* device = new Device("192.168.43.196", 8000);
    device->Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

