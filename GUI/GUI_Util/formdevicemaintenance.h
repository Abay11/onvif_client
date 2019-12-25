#ifndef FORMDEVICEMAINTENANCE_H
#define FORMDEVICEMAINTENANCE_H

#include <QWidget>

#include "types.h"

namespace Ui {
class FormDeviceMaintenance;
}

class FormDeviceMaintenance : public QWidget
{
    Q_OBJECT

public:
    explicit FormDeviceMaintenance(QWidget *parent = nullptr);
    ~FormDeviceMaintenance();

    void fillInfo(const _onvif::CapabilitiesSP);

private:
    Ui::FormDeviceMaintenance *ui;
};

#endif // FORMDEVICEMAINTENANCE_H
