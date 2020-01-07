#ifndef FORMVIDEOCONFIGURATION_H
#define FORMVIDEOCONFIGURATION_H

#include <QWidget>

#include "types.h"

namespace Ui {
class FormVideoConfiguration;
}

class FormVideoConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit FormVideoConfiguration(QWidget *parent = nullptr);
    ~FormVideoConfiguration();

	void fillInfo(const _onvif::Profiles* profiles,
								const _onvif::VideoSources& videoSources);


private:
    Ui::FormVideoConfiguration *ui;
};

#endif // FORMVIDEOCONFIGURATION_H
