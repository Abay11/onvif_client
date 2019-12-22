#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

    friend MainWindow;

public:
    explicit AddDeviceDialog(QWidget *parent = nullptr);
    ~AddDeviceDialog();

    std::string getIP();
    short getPort();

private:
    Ui::AddDeviceDialog *ui;
};

#endif // ADDDEVICEDIALOG_H
