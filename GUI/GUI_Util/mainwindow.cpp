#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "adddevicedialog.h"
#include "DevicesManager.h"
#include "device.h"
#include "formdevicemaintenance.h"
#include "formvideoconfiguration.h"
#include "dialogwaiting.h"
#include "formvideolive.h"

#include <QDebug>
#include <QThread>

//free/helpers functions
void deleteItems(QLayout* layout);

MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent),
			ui(new Ui::MainWindow),
			dwaiting(new DialogWaiting(this))
{
		ui->setupUi(this);

		ui->frameWidgetsHolder->setLayout(new QHBoxLayout);

		//hide controls holder frame until not the device be selected
		ui->frameControlsHolder->setVisible(false);
		connect(ui->listWidget, &QListWidget::itemClicked,
						this, &MainWindow::slotListWidgetClicked);

		connect(ui->leFilter, QOverload<const QString&>::of(&QLineEdit::textEdited),
						this, &MainWindow::slotFilterTextChanged);

		dmngr_thread_ = new QThread(this);
		devicesMgr = new DevicesManager;
		devicesMgr->moveToThread(dmngr_thread_);

		//handling of adding new devices
		connect(ui->btnAddDevice, &QPushButton::clicked, this, &MainWindow::slotAddDeviceClicked);
		connect(this, &MainWindow::sigAddDevice, devicesMgr, &DevicesManager::slotAddDevice);
		connect(devicesMgr, &DevicesManager::sigNewDeviceAdded, this, &MainWindow::slotNewDeviceAdded);

		//Buttons for devices functionality management
		connect(ui->btnLive, &QPushButton::clicked, this, &MainWindow::slotLiveClicked);
		connect(ui->btnVideo, &QPushButton::clicked, this, &MainWindow::slotVideoSettingsClicked);
		connect(ui->btnMaintenance, &QPushButton::clicked, this, &MainWindow::slotMaintenanceClicked);

		//async requests' results handlers
		connect(devicesMgr, &DevicesManager::sigAsyncGetProfileReady,
						this, &MainWindow::slotLoadMediaProfileReady);
		connect(devicesMgr, &DevicesManager::sigAsyncGetVideoSettingsReady,
						this, &MainWindow::slotVideoSettingsReady);
		connect(devicesMgr, &DevicesManager::sigVideoEncoderConfigAdded,
						this, &MainWindow::slotVideoEncoderConfigAdded);
		connect(devicesMgr, &DevicesManager::sigLiveInfoReady,
						this, &MainWindow::slotLiveInfoReady);
		connect(devicesMgr, &DevicesManager::sigLiveUriReady,
						this, &MainWindow::slotLiveUriReady);

		dmngr_thread_->start();
}

MainWindow::~MainWindow()
{
		dmngr_thread_->quit();
		dmngr_thread_->wait();
		delete ui;
}

void MainWindow::setCurrentWidget(QWidget* widget)
{
		if(widget == nullptr)
				{
						qWarning() << "Invalid QWidget pointer could not be shown!";
						return;
				}

		auto frameLayout = ui->frameWidgetsHolder->layout();
		if(frameLayout == nullptr)
				{
						qWarning() << "FrameWidgetsHolder is not initialized!";
						return;
				}

		deleteItems(frameLayout);
		widget->setVisible(true);
		frameLayout->addWidget(widget);
}

void MainWindow::slotListWidgetClicked()
{
		ui->frameControlsHolder->setVisible(true);
}

void MainWindow::slotFilterTextChanged(const QString& filter)
{
		for(int i = 0; i < ui->listWidget->count(); ++i)
				{
						auto curItem = ui->listWidget->item(i);
						bool isHidden = true;
						if(filter.isEmpty() || curItem->text().contains(filter, Qt::CaseInsensitive))
								isHidden = false;

						ui->listWidget->setItemHidden(curItem, isHidden);
				}
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

void MainWindow::slotLiveClicked()
{
		/*
		 * 1. request asyncly info and animation is start
		 * 2. DeviceManager do asyncly getting needed info and emit result ready signal
		 * 3. result slot process info, close animation and set the live widget
		*/
		if(formVideoLive && formVideoLive->isVisible())
				{
						return;
				}
		else
				{
						formVideoLive = new FormVideoLive(this);

						connect(formVideoLive, &FormVideoLive::sigProfileSwitched,
										this, &MainWindow::slotProfileSwitched);
				}

		setCurrentWidget(formVideoLive);

		auto* selectedItem = ui->listWidget->currentItem();
		auto* requestedDevice = devicesMgr->getDevice(selectedItem->text());
		if(requestedDevice)
				devicesMgr->asyncGetLiveInfo(selectedItem->text());

		dwaiting->open();
}

void MainWindow::slotLiveInfoReady()
{
		QStringList profilesTokens;
		QString uri;
		devicesMgr->getLiveInfoResults(profilesTokens, uri);

		formVideoLive->SetProfileTokens(profilesTokens);
		formVideoLive->SetStreamUri(uri);

		dwaiting->close();
}

void MainWindow::slotProfileSwitched(const QString& profile)
{
		auto selectedItem = ui->listWidget->currentItem();
		auto requestedDevice = devicesMgr->getDevice(selectedItem->text());
		if(requestedDevice == nullptr) return;

		devicesMgr->asyncGetLiveUri(selectedItem->text(), profile);

		dwaiting->open();
}

void MainWindow::slotLiveUriReady()
{
		QString uri;
		devicesMgr->getLiveUriResult(uri);
		formVideoLive->SetStreamUri(uri);

		dwaiting->close();
}

void MainWindow::slotMaintenanceClicked()
{
		auto selectedItem = ui->listWidget->currentItem();
    if(selectedItem)
        {
						if(formMaintenance && formMaintenance->isVisible())
								{
										return;
								}
						else
								{
										formMaintenance = new FormDeviceMaintenance(this);
								}

						setCurrentWidget(formMaintenance);

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
						if(formVideoConf && formVideoConf->isVisible())
								{
										return;
								}
						else
								{
										formVideoConf = new FormVideoConfiguration(this);
										connect(formVideoConf, &FormVideoConfiguration::sigMediaProfilesSwitched,
														this, &MainWindow::slotLoadMediaProfile);
										connect(formVideoConf, &FormVideoConfiguration::sigApplyClicked,
														this, &MainWindow::slotVideoEncoderApplyClicked);
										connect(formVideoConf, &FormVideoConfiguration::sigAddVideoEncoderConfig,
														this, &MainWindow::slotAddVideoEncoderConfig);
								}

						setCurrentWidget(formVideoConf);
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
		qDebug() << "Video config added. Do acquire new settings";
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

void MainWindow::slotVideoEncoderApplyClicked()
{
		qDebug() << "Sending video encoder params is started";

		devicesMgr->setVideoEncoderSettings(ui->listWidget->currentItem()->text(),
																				formVideoConf->getNewSettings());
		qDebug() << "Sending video encoder params is finished";
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
