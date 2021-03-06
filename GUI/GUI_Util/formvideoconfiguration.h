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
options for a specified profil.
	@profileParams is a current parameters and available ones for a specified profile@
	@profilesTokens is a optional string list of tokens of all available profiles on a camera.
	It should be specified when FormVideoConfiguration opens
	and not should be specified when profiles were just switched.
	***/
	void fillInfo(const _onvif::ProfileSP profileParams,
								const QStringList* profilesTokens = nullptr);

	//next methods are used to extract currently
	//selected values from UI elements
	QString getMediaProfileToken();

	_onvif::VideoEncoderConfiguration getNewSettings();

signals:
	//some cases hanled internally by this class-widget itself
	//like switching codecs etc., in that case
	//we already have the requered info
	//but when profiles switched we need emit this signal
	//to notify parent-holder widget
	//that we need additional actions (for example, it's more
	//correctly to request configs for currently selected profile
	//to dynamically re-fill elements with received configs)
	void sigMediaProfilesSwitched(const QString& newProfile);

	//notify parent when a user clicked Apply
	//to send settings to a device
	//assumed that signal is emitted for currently
	//selected item on devices widget/lists
	void sigApplyClicked();

	//notify when user want to change encoding for a profile
	void sigAddVideoEncoderConfig(const QString& /*profileToken*/, const QString& /*newEncodingToken*/);

private slots:
	//when user selectes other encoding, to escape side effects
	//disable all other settings
	void slotDisableSettings();

	void slotEncodingSwitched(const QString& /*encoding*/);

	//emits signals depending of changed labels
	void slotApplyClicked();

	//reset all settings to holder_values_ saved
	void slotCancelClicked();

	//by default buttons Apply and Cancel are disabled
	//and they are enabled when at least one parameter changed
	void slotSetupButtons();

private:
	void saveValues();
	void restoreValues();

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
		//Actual values SHOULD BE set after applying settings,
		//loading new settings and etc.
		QMap<const QObject*, QString> value_holder_;

		//save current profile encoder options
		//should be saved after each call the method fillInfo,
		//which receive profile options
		//I think there will not be any issues with what
		//we not requere dynamically encoding params and store them
		//At least them should be updated every fiilInfo call
		_onvif::ProfileSP profile_params_;

		//holder for changed params which will be
		//send to the device
		//should be cleared after send to client immediately
		//TODO: _onvif::VideoEncoderConfiguration new_encoder_params_;
};

#endif // FORMVIDEOCONFIGURATION_H
