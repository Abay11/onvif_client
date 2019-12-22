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

public:
    explicit AddDeviceDialog(QWidget *parent = nullptr);
    ~AddDeviceDialog();

    std::string getIP();
    short getPort();
    std::string getURI();

private:
    Ui::AddDeviceDialog *ui;
};

#endif // ADDDEVICEDIALOG_H
