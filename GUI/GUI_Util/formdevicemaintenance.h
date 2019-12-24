#ifndef FORMDEVICEMAINTENANCE_H
#define FORMDEVICEMAINTENANCE_H

#include <QWidget>

namespace Ui {
class FormDeviceMaintenance;
}

class FormDeviceMaintenance : public QWidget
{
    Q_OBJECT

public:
    explicit FormDeviceMaintenance(QWidget *parent = nullptr);
    ~FormDeviceMaintenance();

private:
    Ui::FormDeviceMaintenance *ui;
};

#endif // FORMDEVICEMAINTENANCE_H
