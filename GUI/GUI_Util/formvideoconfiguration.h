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

	// the method will erase all previous settings and set updated settings for a specified profile
	void fillInfo(const _onvif::Profiles profiles,
								const _onvif::VideoSources& videoSources,
								int profile_index = 0);

	// the method not erase any data and
	//it intended to show available video encoding settings
	void appendInfo(const _onvif::VideoEncoderOptionsSP options);

signals:
		void sigMediaProfilesSwitched(int new_index);

private:
    Ui::FormVideoConfiguration *ui;
};

#endif // FORMVIDEOCONFIGURATION_H
