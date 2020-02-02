#ifndef FORMVIDEOCONFIGURATION_H
#define FORMVIDEOCONFIGURATION_H

#include <QWidget>
#include <QMap>

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
								int profile_index = 0);

signals:
	//some cases hanled internally by this class-widget itself
	//like switching codecs etc., in that case
	//we already have the requered info
	//but when profiles switched we need emit this signal
	//to notify parent-holder widget
	//that we need additional actions (for example, it's more
	//correctly to request configs for currently selected profile
	//to dynamically re-fill elements with received configs)
	void sigMediaProfilesSwitched(int new_index);

private slots:
	//when user selectes other encoding, to escape side effects
	//disable all other settings
	void slotDisableSettings();

	void slotEncodingSwitched();

private:
	void saveValues();

	//this method is used to handle
	//state of all GUI elements
	void makeElementsEnable(bool value);

	//fill specified encoding's params
	void fillEncodingParams(const QString& encoding);

private:
    Ui::FormVideoConfiguration *ui;

		//Maps GUI elements with it's value
		//used to save a current values. it helpfull to restore
		//values if a user canceled changing settings or when need
		//to determine which values changed
		//to send to the device only changed values.
		//Should be set actual values after applying settings,
		//loading new settings and etc.
		QMap<const QObject*, QString> value_holder_;

		//save current profile encoder options
		//should be saved after each call the method fillInfo,
		//which receive profile options
		//I think there will not be any issues with what
		//we not requere dynamically encoding params and store them
		//At least them should be updated every fiilInfo call
		_onvif::ProfileSP profile_params_;
};

#endif // FORMVIDEOCONFIGURATION_H
