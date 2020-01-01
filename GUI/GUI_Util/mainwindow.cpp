#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"
#include "formdevicemaintenance.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devicesMgr(new DevicesManager)
{
    ui->setupUi(this);

   //handling of adding new devices
   connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
   connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
   connect(devicesMgr, &DevicesManager::sigNewDeviceAdded, this, &MainWindow::slotNewDeviceAdded);

   //Buttons for devices functionality management
   connect(ui->btnMaintenance, &QPushButton::clicked, this, &MainWindow::slotMaintenanceClicked);
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

//if Maintenance buttons of clicked, we should to set and show a proper widget
//contains corresponded info
void MainWindow::slotMaintenanceClicked()
{
    //do switch on the maintaince widget
    auto selectedItem = ui->listWidget->currentItem();
    if(selectedItem)
    {
        qDebug() << "Current selected item:" << selectedItem->text();

        //setting the maintanance widget to the frame
        FormDeviceMaintenance* formMaintenance = new FormDeviceMaintenance(ui->frameWidgetsHolder);

        auto frameLayout = ui->frameWidgetsHolder->layout();
        if(!frameLayout)
        {
            qDebug() << "Init new layout";
            ui->frameWidgetsHolder->setLayout(new QHBoxLayout);
        }
        else
        {
            frameLayout->removeItem(frameLayout->itemAt(0)); //expected that the form always will be hold only one item
            qDebug() << "Deleting child";
        }

        ui->frameWidgetsHolder->layout()->addWidget(formMaintenance);

        auto requestedDevice = devicesMgr->getDevice(selectedItem->text());
        if(requestedDevice)
            formMaintenance->fillInfo(requestedDevice->getDeviceInfo(), requestedDevice->getCapabilities(), requestedDevice->getONVIFGeneralInfo());
        else
            qDebug() << "ERROR:" << "Can't find selected item from stored devices";
    }
}

