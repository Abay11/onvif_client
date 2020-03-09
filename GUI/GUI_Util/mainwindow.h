#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

class FormVideoConfiguration;
class FormDeviceMaintenance;

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
    Ui::MainWindow *ui;
    DevicesManager* devicesMgr;
    AddDeviceDialog* addDeviceDialog = nullptr;
		DialogWaiting* dwaiting = nullptr;

		//DevicesManager instance works in separate thread
		QThread* dmngr_thread_;

		//forms
		FormVideoConfiguration* formVideoConf = nullptr;
		FormDeviceMaintenance* formMaintenance = nullptr;

signals:
		//connected to the DevicesManager to try add a device
		void sigAddDevice(QString ip, short port, QString uri);

private slots:
		//to make the list widget visible
		void slotListWidgetClicked();

		//process filter events
		void slotFilterTextChanged(const QString&);

    //emitted by the button AddDevice
    void slotAddDeviceClicked();

    //emitted by AddDeviceDialog after finished
    void slotAddDeviceDialogFinished();

    //emitted by DevicesManager when a new device configured
    void slotNewDeviceAdded(QString deviceAddresses);

    //slots to handle a device functionality
    void slotMaintenanceClicked();
    void slotVideoSettingsClicked();
		void slotVideoSettingsReady();
		void slotVideoEncoderConfigAdded();

		//the slot to load info for specified profile
		//uses when a user switches media profiles on the video configuration form
		void slotLoadMediaProfile(const QString& /*newProfileToken*/);
		void slotLoadMediaProfileReady();

		//need to process separately when user apply new encoding for a media profile
		void slotAddVideoEncoderConfig(const QString& /*profileToken*/, const QString& /*newEncToken*/);
};
#endif // MAINWINDOW_H
