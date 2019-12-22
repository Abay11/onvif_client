#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"

#include <QLineEdit>

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog)
{
    ui->setupUi(this);
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

std::string AddDeviceDialog::getIP()
{
    if(ui)
        return ui->leIP->text().toStdString();

    return std::string("");
}

short AddDeviceDialog::getPort()
{
    if(ui)
        return ui->lePort->text().toShort();

    return 0;
}
