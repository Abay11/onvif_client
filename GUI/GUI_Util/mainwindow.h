#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}

namespace utility
{
class SavedDevices;
}

class FormVideoLive;
class FormVideoConfiguration;
class FormDeviceMaintenance;

class QSettings;

QT_END_NAMESPACE

class DevicesManager;
class AddDeviceDialog;
class DialogWaiting;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
		void setCurrentWidget(QWidget*);

private:
    Ui::MainWindow *ui;
		QSettings* settings = nullptr;
    DevicesManager* devicesMgr;
    AddDeviceDialog* addDeviceDialog = nullptr;
		DialogWaiting* dwaiting = nullptr;

		//DevicesManager instance works in separate thread
		QThread* dmngr_thread_;

		//forms
		FormVideoLive* formVideoLive = nullptr;
		FormVideoConfiguration* formVideoConf = nullptr;
		FormDeviceMaintenance* formMaintenance = nullptr;
		std::unique_ptr<utility::SavedDevices> savedDevices;

signals:
		//connected to the DevicesManager to try add a device
		void sigAddDevice(QString ip, short port, QString uri);

		void sigAsyncGetLiveInfo(const QString& deviceID);

private slots:
		//to make the list widget visible
		void slotListWidgetClicked();
		void slotListWidgetContextMenu(const QPoint&);
		void slotDeleteDevice();

		void slotFilterTextChanged(const QString&);

    //emitted by the button AddDevice
    void slotAddDeviceClicked();

    //emitted by AddDeviceDialog after finished
    void slotAddDeviceDialogFinished();

    //emitted by DevicesManager when a new device configured
    void slotNewDeviceAdded(QString deviceAddresses);

		//live slots
		void slotLiveClicked();
		void slotLiveInfoReady();
		void slotProfileSwitched(const QString&);
		void slotLiveUriReady();

    //slots to handle a device functionality
    void slotMaintenanceClicked();
    void slotVideoSettingsClicked();
		void slotVideoSettingsReady();
		void slotVideoEncoderConfigAdded();

		//the slot to load info for specified profile
		//uses when a user switches media profiles on the video configuration form
		void slotLoadMediaProfile(const QString& /*newProfileToken*/);
		void slotLoadMediaProfileReady();

		//apply buttons handlers
		void slotVideoEncoderApplyClicked();

		//need to process separately when user apply new encoder config to a media profile
		void slotAddVideoEncoderConfig(const QString& /*profileToken*/, const QString& /*newEncToken*/);
};
#endif // MAINWINDOW_H
