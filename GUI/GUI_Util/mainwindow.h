#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DevicesManager;
class AddDeviceDialog;

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

signals:
    void sigAddDevice(std::string ip, short port, std::string uri);

private slots:
		//to make the list widget visible
		void slotListWidgetClicked();

    //emitted by the button AddDevice
    void slotAddDeviceClicked();

    //emitted by AddDeviceDialog after finished
    void slotAddDeviceDialogFinished();

    //emitted by DevicesManager when a new device configured
    void slotNewDeviceAdded(QString deviceAddresses);

    //slots to handle a device functionality
    void slotMaintenanceClicked();
    void slotVideoSettingsClicked();
};
#endif // MAINWINDOW_H
