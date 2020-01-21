#include "formvideoconfiguration.h"
#include "ui_formvideoconfiguration.h"

#include <QDebug>

FormVideoConfiguration::FormVideoConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoConfiguration)
{
    ui->setupUi(this);

		connect(ui->comboMediaProfiles, SIGNAL(activated(int)),
						this, SIGNAL(sigMediaProfilesSwitched(int)));
}

FormVideoConfiguration::~FormVideoConfiguration()
{
    delete ui;
	qDebug() << "FormVideoConfiguration deleted";
}

void FormVideoConfiguration::fillInfo(const _onvif::Profiles profiles,
																			const _onvif::VideoSources& videoSources, int profile_index)
{
	if(profile_index >= profiles->size())
	{
		qDebug() << "skip invalid profile_index";
		profile_index = 0;
	}

	//fill media profiles
	QStringList profile_names;
	for(const auto& p : *profiles)
	{
		profile_names.push_back(p->Name.c_str());
	}

	ui->comboMediaProfiles->clear();
	ui->comboMediaProfiles->addItems(profile_names);
	ui->comboMediaProfiles->setCurrentIndex(profile_index);

	auto current_profile = profiles->at(profile_index);

	ui->lblProfileName->setText(current_profile->Name.c_str());

	//fill comboVideoSources
	QStringList video_sources_tokens;
	for(const auto& vs : videoSources)
	{
		video_sources_tokens.push_back(vs->source_token.c_str());
	}
	ui->comboVideoSources->clear();
	ui->comboVideoSources->addItems(video_sources_tokens);

	//video sources configurations
	if(auto* vs = current_profile->videoSource)
	{
		ui->lblVSName->setText(vs->name.c_str());
		ui->lblVSToken->setText(vs->token.c_str());
		ui->lblVSUseCount->setNum(vs->useCount);
		ui->lblVSBounds->setText(vs->bounds.c_str());
	}

	//video encoder configurations
	if(auto& ve = current_profile->videoEncoder)
	{
		ui->lblECName->setText(ve->name.c_str());

		ui->cmbECToken->clear();
		ui->cmbECToken->addItem(ve->token.c_str());

		ui->lblECUseCount->setNum(ve->useCount);

		ui->cmbEncodings->clear();
		ui->cmbEncodings->addItem(ve->encoding.c_str());

		ui->comboResolutions->clear();
		ui->comboResolutions->addItem(ve->resolution.c_str());

		ui->comboResolutions->clear();
		ui->comboResolutions->addItem(ve->resolution.c_str());

		ui->comboQualities->clear();
		ui->comboQualities->addItem(QString::number(static_cast<double>(ve->quality)));

		ui->comboFramerate->clear();
		ui->comboFramerate->addItem(QString::number(ve->framerate));

		ui->comboEncodingInterval->clear();
		ui->comboEncodingInterval->addItem(QString::number(ve->encoding_interval));

		ui->comboBitrate->clear();
		ui->comboBitrate->addItem(QString::number(ve->bitrate));

		ui->comboGOVLength->clear();
		ui->comboH264Profiles->clear();
		if(!ve->codec_profile.empty())
		{
			ui->comboH264Profiles->addItem(ve->codec_profile.c_str());
			ui->comboGOVLength->addItem(QString::number(ve->gov_length));
			ui->comboH264Profiles->setEnabled(true);
			ui->comboGOVLength->setEnabled(true);
		}
		else
		{
			ui->comboH264Profiles->setEnabled(false);
			ui->comboGOVLength->setEnabled(false);
		}

		ui->leMulticastIP->setText(ve->multicast_ip.c_str());
		ui->leMulticastPort->setText(QString::number(ve->multicast_port));
		ui->leMulticastTTL->setText(QString::number(ve->multicast_ttl));

		ui->comboMulticastAutostart->addItem(ve->multicast_autostart ? "Yes" : "No");
		ui->comboMulticastAutostart->addItem(ve->multicast_autostart ? "No" : "Yes");

		ui->lblSessionTimeout->setText(ve->session_timeout.c_str());
	}

}

void FormVideoConfiguration::appendInfo(const _onvif::VideoEncoderOptionsSP options)
{
	if(!options) return;

	const char* JPEG = "JPEG";
	const char* H264 = "H264";
	const char* MPEG = "MPEG";

	QString currentCodecStr = ui->cmbEncodings->currentText();
	if(options->JPEGOptions.isInit &&  currentCodecStr != JPEG)
		ui->cmbEncodings->addItem(JPEG);
	if(options->H264Options.isInit && currentCodecStr != H264)
		ui->cmbEncodings->addItem(H264);
	if(options->MPEGOptions.isInit && currentCodecStr != MPEG)
		ui->cmbEncodings->addItem(MPEG);

	const std::vector<std::string>* resolutionsPtr = nullptr;
	if(currentCodecStr == JPEG)
	{
		resolutionsPtr = &options->JPEGOptions.Resolutions;
	}
	else if(currentCodecStr == H264)
	{
		resolutionsPtr = &options->H264Options.Resolutions;
	}
	else if(currentCodecStr == MPEG)
	{

	}

	//RESOLUTION
	if(resolutionsPtr)
	{
	QString currentResolutionStr = ui->comboResolutions->currentText();
	ui->comboResolutions->clear();
	for(auto& res : *resolutionsPtr)
	{
		ui->comboResolutions->addItem(res.c_str());

		if(currentResolutionStr == res.c_str())
			ui->comboResolutions->setCurrentIndex(ui->comboResolutions->count() - 1);
	}
	}

	//QUALITIES
	int currentQuality = ui->comboQualities->currentText().toInt();
	ui->comboQualities->clear();
	for(int i = options->QualityMin; i <= options->QualityMax; ++i)
	{
		ui->comboQualities->addItem(QString::number(i));

		if(currentQuality == i)
			ui->comboQualities->setCurrentIndex(ui->comboQualities->count() - 1);
	}

		/*
		ui->comboFramerate->clear();
		ui->comboFramerate->addItem(QString::number(ve->framerate));


		ui->comboEncodingInterval->clear();
		ui->comboEncodingInterval->addItem(QString::number(ve->encoding_interval));

		ui->comboBitrate->clear();
		ui->comboBitrate->addItem(QString::number(ve->bitrate));

		ui->comboGOVLength->clear();
		ui->comboH264Profiles->clear();
		if(!ve->codec_profile.empty())
		{
			ui->comboH264Profiles->addItem(ve->codec_profile.c_str());
			ui->comboGOVLength->addItem(QString::number(ve->gov_length));
			ui->comboH264Profiles->setEnabled(true);
			ui->comboGOVLength->setEnabled(true);
		}
		else
		{
			ui->comboH264Profiles->setEnabled(false);
			ui->comboGOVLength->setEnabled(false);
		}
		*/
}
