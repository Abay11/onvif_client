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

QString AddDeviceDialog::getIP()
{
    if(ui)
				return ui->leIP->text();

		return QString("");
}

ushort AddDeviceDialog::getPort()
{
    if(ui)
				return ui->lePort->text().toUShort();

    return 0;
}

QString AddDeviceDialog::getURI()
{
    if(ui)
				return ui->leURI->text();

		return QString("");
}

QString AddDeviceDialog::getUser()
{
		if(ui)
				return ui->leUser->text();

		return QString("");
}

QString AddDeviceDialog::getPass()
{
		if(ui)
				return ui->lePass->text();

		return QString("");
}
