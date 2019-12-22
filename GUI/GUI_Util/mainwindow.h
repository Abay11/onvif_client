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
    void sigAddDevice(std::string, short);

private slots:
    void slotAddDeviceClicked();
    void slotAddDeviceDialogFinished();
};
#endif // MAINWINDOW_H
