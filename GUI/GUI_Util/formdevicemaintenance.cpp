#include "formdevicemaintenance.h"
#include "ui_formdevicemaintenance.h"

FormDeviceMaintenance::FormDeviceMaintenance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceMaintenance)
{
    ui->setupUi(this);
}

FormDeviceMaintenance::~FormDeviceMaintenance()
{
    delete ui;
}

void FormDeviceMaintenance::fillInfo(const _onvif::CapabilitiesSP caps)
{
    if(caps->io_filled)
    {
        ui->lblVideoSourcesValue->setNum(caps->io_videoSources);
        ui->lblVideoOutputsValue->setNum(caps->io_videoOutputs);
        ui->lblAudioSourcesValue->setNum(caps->io_audioSources);
        ui->lblAudioOutputsValue->setNum(caps->io_audioOutputs);
        ui->lblDigitalInputsValue->setNum(caps->io_digitalInputs);
        ui->lblRelayOutputsValue->setNum(caps->io_relayOutputs);
    }
}
