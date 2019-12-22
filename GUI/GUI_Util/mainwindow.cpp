#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devicesMgr(new DevicesManager)
{
    ui->setupUi(this);

   connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
   connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAddDeviceClicked()
{
    if(!addDeviceDialog)
    {
        addDeviceDialog = new AddDeviceDialog(this);
        connect(addDeviceDialog, &AddDeviceDialog::finished, this, &MainWindow::slotAddDeviceDialogFinished);
    }

    addDeviceDialog->open();
}

void MainWindow::slotAddDeviceDialogFinished()
{
    if(addDeviceDialog && QDialog::Accepted == addDeviceDialog->result())
    {
        auto ip = addDeviceDialog->getIP();
        auto port = addDeviceDialog->getPort();
        if(!ip.empty() && port)
        {
            emit sigAddDevice(ip, port);
        }
        else
        {
            //do log about incorrect address or something
        }

    }
}

