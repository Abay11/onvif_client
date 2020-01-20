#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"
#include "formdevicemaintenance.h"
#include "formvideoconfiguration.h"

#include <QDebug>

//free/helpers functions
void deleteItems(QLayout* layout);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devicesMgr(new DevicesManager)
{
    ui->setupUi(this);

		//hide controls holder frame until not the device be selected
		ui->frameControlsHolder->setVisible(false);
		connect(ui->listWidget, &QListWidget::itemClicked,
						this, &MainWindow::slotListWidgetClicked);

   //handling of adding new devices
   connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
   connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
   connect(devicesMgr, &DevicesManager::sigNewDeviceAdded, this, &MainWindow::slotNewDeviceAdded);

   //Buttons for devices functionality management
	 connect(ui->btnVideo, &QPushButton::clicked, this, &MainWindow::slotVideoSettingsClicked);
	 connect(ui->btnMaintenance, &QPushButton::clicked, this, &MainWindow::slotMaintenanceClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotListWidgetClicked()
{
	ui->frameControlsHolder->setVisible(true);
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

//received signal from DevicesManager
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
        //setting the maintanance widget to the frame
				if(formMaintenance)
				{
					formMaintenance->setVisible(true);
				}
				else
				{
					formMaintenance = new FormDeviceMaintenance(this);
				}

				//switch on the maintaince widget
				auto frameLayout = ui->frameWidgetsHolder->layout();
        if(!frameLayout)
        {
            ui->frameWidgetsHolder->setLayout(new QHBoxLayout);
						frameLayout = ui->frameWidgetsHolder->layout();
        }
				else
				{
					deleteItems(frameLayout);
				}
				frameLayout->addWidget(formMaintenance);

        auto requestedDevice = devicesMgr->getDevice(selectedItem->text());
        if(requestedDevice)
						formMaintenance->fillInfo(requestedDevice->getDeviceInfo(),
																			requestedDevice->getCapabilities(),
																			requestedDevice->getONVIFGeneralInfo());
        else
						qDebug() << "ERROR:" << "Can't find selected item from stored devices";
    }
}

void MainWindow::slotVideoSettingsClicked()
{
		auto selectedItem = ui->listWidget->currentItem();
		if(selectedItem)
			{
			auto* frameLayout = ui->frameWidgetsHolder->layout();
			if(!frameLayout)
			{
				qDebug() << "Init a new layout";
				ui->frameWidgetsHolder->setLayout(new QHBoxLayout);
				frameLayout = ui->frameWidgetsHolder->layout();
			}
			else
				deleteItems(frameLayout);

			if(formVideoConf)
			{
				formVideoConf->setVisible(true);
			}
			else
			{
				formVideoConf = new FormVideoConfiguration(this);
				connect(formVideoConf, &FormVideoConfiguration::sigMediaProfilesSwitched,
								this, &MainWindow::slotMediaProfileSwitched);
			}

			frameLayout->addWidget(formVideoConf);

			auto requestedDevice = devicesMgr->getDevice(selectedItem->text());
			if(requestedDevice && requestedDevice->GetProfiles()
				 && requestedDevice->GetProfiles()->front())
			{
				auto profiles = requestedDevice->GetProfiles();

				//take the first profile as default and set it settings to the form
				std::string current_profile_token = profiles->front()->token.c_str();
				//as default show configs of the first profile from profiles list
				auto videoSources = requestedDevice->GetCompatibleVideoSources(current_profile_token);

				//set current settings
				formVideoConf->fillInfo(profiles, videoSources);
				//set available settings
				auto encoders = requestedDevice->GetVideoEncoderOptions(current_profile_token, profiles->front()->videoEncoder->token);
				formVideoConf->appendInfo(encoders);
			}
			else
				qDebug() << "ERROR:" << "Can't find selected item from stored devices";
		}
}

void MainWindow::slotMediaProfileSwitched(int new_index)
{
	qDebug() << "Need to load info for a profile: " << new_index;

	auto selectedItem = ui->listWidget->currentItem();
	if(selectedItem)
	{
		auto device = devicesMgr->getDevice(selectedItem->text());
		if(device == nullptr)
		{
			qDebug() << "Can't find specified device";
			return;
		}

		formVideoConf->fillInfo(device->GetProfiles(),
														device->GetVideoSources(),
														new_index);
	}
}

/////////////////////////////
// free/helpers functions //
///////////////////////////

void deleteItems(QLayout* layout)
{
	if(auto child = layout->takeAt(0)) // by current implementation it is assumed that layout will hold one child
	{
		child->widget()->setVisible(false);
		delete child;
	}
}
