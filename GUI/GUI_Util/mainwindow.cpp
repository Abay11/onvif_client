#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devicesMgr(new DevicesManager)
{
    ui->setupUi(this);

   connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
   connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
   connect(devicesMgr, &DevicesManager::sigNewDeviceAdded, this, &MainWindow::slotNewDeviceAdded);
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
        auto uri = addDeviceDialog->getURI();
        if(!ip.empty() && port && !uri.empty())
        {
            emit sigAddDevice(ip, port, uri);
        }
        else
        {
            //do log about incorrect address or something
        }

    }
}

void MainWindow::slotNewDeviceAdded(QString deviceAddresses)
{
    QListWidgetItem* newDevice = new QListWidgetItem(ui->listWidget);
    newDevice->setText(deviceAddresses);
    ui->listWidget->insertItem(ui->listWidget->count(), newDevice);
}

