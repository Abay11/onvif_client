#include "formdevicemaintenance.h"
#include "ui_formdevicemaintenance.h"
#include <QDebug>

FormDeviceMaintenance::FormDeviceMaintenance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceMaintenance)
{
    ui->setupUi(this);
}

FormDeviceMaintenance::~FormDeviceMaintenance()
{
    delete ui;
	qDebug() << "FormDeviceMaintenance deleted";
}

void FormDeviceMaintenance::fillInfo(const _onvif::DeviceInformationSP devInfo, const _onvif::CapabilitiesSP caps, const _onvif::ONVIFGeneralInfoSP onvifInfo)
{
    if(devInfo && devInfo->filled)
    {
        ui->lblManufacturerValue->setText(devInfo->manufacturer.c_str());
        ui->lblModelValue->setText(devInfo->model.c_str());
        ui->lblFirmwareValue->setText(devInfo->firmwareVersion.c_str());
        ui->lblSerialValue->setText(devInfo->serialNumber.c_str());
        ui->lblHardwareValue->setText(devInfo->hardwareId.c_str());
    }

    //filling section ONVIF information
    if(onvifInfo)
    {
        ui->lblonvifVersionValue->setText(onvifInfo->onvif_version.c_str());
        ui->lblMedia2SupportValue->setText(onvifInfo->isMedia2Supported ? "Yes" : "No");
        ui->lblDeviceURIValue->setText(onvifInfo->deviceServiceURI.c_str());
    }

    if(caps && caps->io_filled)
    {
        ui->lblVideoSourcesValue->setNum(caps->io_videoSources);
        ui->lblVideoOutputsValue->setNum(caps->io_videoOutputs);
        ui->lblAudioSourcesValue->setNum(caps->io_audioSources);
        ui->lblAudioOutputsValue->setNum(caps->io_audioOutputs);
        ui->lblDigitalInputsValue->setNum(caps->io_digitalInputs);
        ui->lblRelayOutputsValue->setNum(caps->io_relayOutputs);
    }
}
