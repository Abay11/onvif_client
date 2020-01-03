#ifndef FORMVIDEOCONFIGURATION_H
#define FORMVIDEOCONFIGURATION_H

#include <QWidget>

namespace Ui {
class FormVideoConfiguration;
}

class FormVideoConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit FormVideoConfiguration(QWidget *parent = nullptr);
    ~FormVideoConfiguration();

private:
    Ui::FormVideoConfiguration *ui;
};

#endif // FORMVIDEOCONFIGURATION_H
