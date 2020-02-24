#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"
#include "formdevicemaintenance.h"
#include "formvideoconfiguration.h"
#include "dialogwaiting.h"

#include <QDebug>
#include <QThread>

//free/helpers functions
void deleteItems(QLayout* layout);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
		, ui(new Ui::MainWindow)
		, dwaiting(new DialogWaiting(this))
{
	ui->setupUi(this);

	//hide controls holder frame until not the device be selected
	ui->frameControlsHolder->setVisible(false);
	connect(ui->listWidget, &QListWidget::itemClicked,
					this, &MainWindow::slotListWidgetClicked);

	dmngr_thread_ = new QThread(this);
	devicesMgr = new DevicesManager;
	devicesMgr->moveToThread(dmngr_thread_);

	//handling of adding new devices
	connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
	connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
	connect(devicesMgr, &DevicesManager::sigNewDeviceAdded, this, &MainWindow::slotNewDeviceAdded);

	//Buttons for devices functionality management
	connect(ui->btnVideo, &QPushButton::clicked, this, &MainWindow::slotVideoSettingsClicked);
	connect(ui->btnMaintenance, &QPushButton::clicked, this, &MainWindow::slotMaintenanceClicked);

	//async requests' results handlers
	connect(devicesMgr, &DevicesManager::sigAsyncGetProfileReady,
					this, &MainWindow::slotLoadMediaProfileReady);
	connect(devicesMgr, &DevicesManager::sigAsyncGetVideoSettingsReady,
					this, &MainWindow::slotVideoSettingsReady);
	connect(devicesMgr, &DevicesManager::sigVideoEncoderConfigAdded,
					this, &MainWindow::slotVideoEncoderConfigAdded);

	dmngr_thread_->start();
}

MainWindow::~MainWindow()
{
	dmngr_thread_->quit();
	dmngr_thread_->wait();
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
		if(!ip.isEmpty() && port && !uri.isEmpty())
		{
			dwaiting->open();

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

		dwaiting->close();
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
								this, &MainWindow::slotLoadMediaProfile);
				connect(formVideoConf, &FormVideoConfiguration::sigAddVideoEncoderConfig,
								this, &MainWindow::slotAddVideoEncoderConfig);
			}

			frameLayout->addWidget(formVideoConf);

			dwaiting->open();
			devicesMgr->asyncGetVideoSettings(selectedItem->text());
		}
}

void MainWindow::slotVideoSettingsReady()
{
	QStringList profilesTokens;
	_onvif::ProfileSP profile;
	devicesMgr->getAsyncGetVideoSettingsResult(profilesTokens, profile);
	formVideoConf->fillInfo(profile, &profilesTokens);

	dwaiting->close();
}

void MainWindow::slotVideoEncoderConfigAdded()
{
	qDebug() << "Need to load info for a profile: " << newProfileToken;
	slotLoadMediaProfile(formVideoConf->getMediaProfileToken());
}

void MainWindow::slotLoadMediaProfile(const QString& newProfileToken)
{
	auto selectedItem = ui->listWidget->currentItem();
	if(selectedItem)
	{
		QString deviceID = selectedItem->text();

		devicesMgr->asyncGetProfile(selectedItem->text(),
																newProfileToken);
		dwaiting->open();
	}
}

void MainWindow::slotLoadMediaProfileReady()
{
		formVideoConf->fillInfo(devicesMgr->getAsyncGetProfileResult());
		dwaiting->close();
}

void MainWindow::slotAddVideoEncoderConfig(const QString& profileToken, const QString& newEncToken)
{
	qDebug() << "New encoding value to apply for profile:" << profileToken << newEncToken;


	bool isCorrectDeviceID = devicesMgr->addVideoEncoderToProfile(ui->listWidget->currentItem()->text(),
																			 profileToken, newEncToken);

	if(isCorrectDeviceID)
	{
		//the dialog windows should be closed only after sending settings
		//and acquiring new encoding parameters
		//device manager should emit signal that config added to a profile
		//and in this class call acquiring new settings
		dwaiting->open();
	} //else open dialog with error text
}

	////////////////////////////
 // free/helpers functions //
////////////////////////////

void deleteItems(QLayout* layout)
{
	if(auto child = layout->takeAt(0)) // by current implementation it is assumed that layout will hold one child
	{
		child->widget()->setVisible(false);
		delete child;
	}
}
