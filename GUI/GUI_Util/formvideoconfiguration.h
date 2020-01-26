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

	/***
	The method will erase all previous settings and set updated settings with available
options for a specified profile.
	@profilesTokens is a string list of tokens of all available profiles on a camera
	@profileParams is a current parameters and available ones for a specified profile@
	@profile_index specifies a used profile token from  profilesTokens.
	***/
	void fillInfo(const _onvif::StringList* profilesTokens,
								const _onvif::ProfileSP profileParams,
								const _onvif::VideoSources& videoSources,
								int profile_index = 0);

signals:
		void sigMediaProfilesSwitched(int new_index);

private:
    Ui::FormVideoConfiguration *ui;
};

#endif // FORMVIDEOCONFIGURATION_H
