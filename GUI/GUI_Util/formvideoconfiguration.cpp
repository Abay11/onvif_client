#include "formvideoconfiguration.h"
#include "ui_formvideoconfiguration.h"

#include <QDebug>

//helpers
template <typename T>
bool isRangeValueSet(const T* first, const T* second)
{
	//check that pointers not null and values are not empty themselves
	return first && second && *first && *second;
}

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

void FormVideoConfiguration::fillInfo(const _onvif::StringList* profilesTokens,
																			const _onvif::ProfileSP current_profile,
																			const _onvif::VideoSources& videoSources,
																			int profile_index)
{
	if(!current_profile)
	{
		qDebug() << "Received an invalid profile";
		return;
	}

	if(static_cast<size_t>(profile_index) >= profilesTokens->size())
	{
		qDebug() << "skip invalid profile_index";
		profile_index = 0;
	}

	//fill media profiles
	QStringList profilesTokensList;
	for(const auto& p : *profilesTokens)
	{
		profilesTokensList.push_back(p.c_str());
	}

	ui->comboMediaProfiles->clear();
	ui->comboMediaProfiles->addItems(profilesTokensList);
	ui->comboMediaProfiles->setCurrentIndex(profile_index);

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

		//need to declare them somewhere else
		const char* JPEG = "JPEG";
		const char* H264 = "H264";
		const char* MPEG = "MPEG";

		//next lines do clearing combo boxes of elements
		//if a passed profile holds options then fill elements
		//with available values
		//else fill them with only current parameter
		ui->cmbEncodings->clear();
		ui->comboResolutions->clear();
		ui->comboQualities->clear();
		ui->comboFramerate->clear();
		ui->comboEncodingInterval->clear();
		ui->comboBitrate->clear();
		ui->comboGOVLength->clear();
		ui->comboGOVLength->setEnabled(false);
		ui->cmbCodecProfiles->clear();
		ui->cmbCodecProfiles->setEnabled(false);

		ui->cmbEncodings->addItem(ve->encoding.c_str());
		//filling with options
		if(auto opts = current_profile->videoEncoderOptions)
		{
			//encoding
			if(opts->JPEGOptions.isInit && ve->encoding != JPEG)
				ui->cmbEncodings->addItem(JPEG);
			if(opts->H264Options.isInit && ve->encoding != H264)
				ui->cmbEncodings->addItem(H264);
			if(opts->MPEGOptions.isInit && ve->encoding != MPEG)
				ui->cmbEncodings->addItem(MPEG);

			const std::vector<std::string>* resolutionsPtr = nullptr;
			const int* frameMin = nullptr;
			const int* frameMax = nullptr;
			const int* encIntervalMin = nullptr;
			const int* encIntervalMax = nullptr;
			const int* bitrateMin = nullptr;
			const int* bitrateMax = nullptr;
			const std::vector<std::string>* profilesPtr = nullptr;
			const int* govLengthMin = nullptr;
			const int* govLengthMax = nullptr;

			if(ve->encoding == JPEG)
			{
				resolutionsPtr = &opts->JPEGOptions.Resolutions;
				frameMin = &opts->JPEGOptions.FrameRateMin;
				frameMax = &opts->JPEGOptions.FrameRateMax;
				encIntervalMax = &opts->JPEGOptions.EncodingIntervalMax;
				encIntervalMin = &opts->JPEGOptions.EncodingIntervalMin;
				bitrateMax = &opts->JPEGOptions.BitrateRangeMax;
				bitrateMin = &opts->JPEGOptions.BitrateRangeMin;
			}
			else if(ve->encoding == H264)
			{
				resolutionsPtr = &opts->H264Options.Resolutions;
				frameMin = &opts->H264Options.FrameRateMin;
				frameMax = &opts->H264Options.FrameRateMax;
				encIntervalMax = &opts->H264Options.EncodingIntervalMax;
				encIntervalMin = &opts->H264Options.EncodingIntervalMin;
				bitrateMax = &opts->H264Options.BitrateRangeMax;
				bitrateMin = &opts->H264Options.BitrateRangeMin;
				profilesPtr = &opts->H264Options.Profiles;
				govLengthMax = &opts->H264Options.GovLengthMax;
				govLengthMin = &opts->H264Options.GovLengthMin;
			}
			else if(ve->encoding == MPEG)
			{
				resolutionsPtr = &opts->MPEGOptions.Resolutions;
				frameMin = &opts->MPEGOptions.FrameRateMin;
				frameMax = &opts->MPEGOptions.FrameRateMin;
				encIntervalMax = &opts->MPEGOptions.EncodingIntervalMax;
				encIntervalMin = &opts->MPEGOptions.EncodingIntervalMin;
				profilesPtr = &opts->MPEGOptions.Profiles;
				govLengthMax = &opts->MPEGOptions.GovLengthMax;
				govLengthMin = &opts->MPEGOptions.GovLengthMin;
			}

			//resolution
			if(resolutionsPtr)
			{
				ui->comboResolutions->clear();
				for(auto& res : *resolutionsPtr)
				{
					ui->comboResolutions->addItem(res.c_str());

					if(ve->resolution == res.c_str())
						ui->comboResolutions->setCurrentIndex(ui->comboResolutions->count() - 1);
				}
			}

			//qualities
			//NOTE: range is int type, but param type is float
			//need to change on gui combobox with some range element
			//doule currentQuality <- TYPE SHOULD BE DOUBLE/FLOAT NOT INT
			int currentQuality = static_cast<int>(ve->quality);
			ui->comboQualities->clear();
			for(int i = opts->QualityMin; i <= opts->QualityMax; ++i)
			{
				ui->comboQualities->addItem(QString::number(i));

				if(currentQuality == i)
					ui->comboQualities->setCurrentIndex(ui->comboQualities->count() - 1);
			}

			//frames
			//check if min and max not NULL
			//otherwise, consider that values not was read
			if(isRangeValueSet(frameMin, frameMax))
			{
				for(int i = *frameMin; i <= *frameMax; ++i)
				{
					ui->comboFramerate->addItem(QString::number(i));
					if(ve->framerate == i)
						ui->comboFramerate->setCurrentIndex(ui->comboFramerate->count() - 1);
				}
			}
			else
					ui->comboFramerate->addItem(QString::number(ve->framerate));

			//encoding interval
			//if both not are 0, consider they are init
			if(isRangeValueSet(encIntervalMax, encIntervalMin))
			{
				for(int i = *encIntervalMin; i <= *encIntervalMax; ++i)
				{
					ui->comboEncodingInterval->addItem(QString::number(i));
					if(ve->encoding_interval == i)
						ui->comboEncodingInterval->setCurrentIndex(ui->comboEncodingInterval->count() - 1);
				}
			}
			else
					ui->comboEncodingInterval->addItem(QString::number(ve->encoding_interval));

			//bitrate
			if(isRangeValueSet(bitrateMin, bitrateMax))
			{
				for(int i = *bitrateMin; i <= *bitrateMax; ++i)
				{
					ui->comboBitrate->addItem(QString::number(i));
					if(ve->bitrate == i)
						ui->comboBitrate->setCurrentIndex(ui->comboBitrate->count() - 1);
				}
			}
			else
					ui->comboBitrate->addItem(QString::number(ve->bitrate));

			//codec profile
			//note codec profiles not uses for JPEG codec
			if(profilesPtr)
			{
				for(const auto& profStr : *profilesPtr)
				{
					ui->cmbCodecProfiles->addItem(profStr.c_str());
					if(ve->codec_profile == profStr)
						ui->cmbCodecProfiles->setCurrentIndex(ui->cmbCodecProfiles->count() - 1);
				}

				ui->cmbCodecProfiles->setEnabled(true);
			}

			//GOV Length
			//also not applies for JPEG codec
			if(isRangeValueSet(govLengthMax, govLengthMin))
			{
				ui->comboGOVLength->setEnabled(true);
				for(int i = *govLengthMin; i < *govLengthMax; ++i)
				{
					ui->comboGOVLength->addItem(QString::number(i));
					if(i == ve->gov_length)
						ui->comboGOVLength->setCurrentIndex(ui->comboGOVLength->count() - 1);
				}
			}
			else if(ve->gov_length)
			{
				ui->comboGOVLength->setEnabled(true);
				ui->comboGOVLength->addItem(QString::number(ve->gov_length));
			}
		}
		else //filling only current parameters
		{
			ui->comboResolutions->addItem(ve->resolution.c_str());
			ui->comboQualities->addItem(QString::number(static_cast<double>(ve->quality)));
			ui->comboFramerate->addItem(QString::number(ve->framerate));
			ui->comboEncodingInterval->addItem(QString::number(ve->encoding_interval));
			ui->comboBitrate->addItem(QString::number(ve->bitrate));

			if(!ve->codec_profile.empty())
			{
				ui->cmbCodecProfiles->addItem(ve->codec_profile.c_str());
				ui->cmbCodecProfiles->setEnabled(true);
			}

			if(ve->gov_length)
			{
				ui->comboGOVLength->addItem(QString::number(ve->gov_length));
				ui->comboGOVLength->setEnabled(true);
			}
		}

		ui->leMulticastIP->setText(ve->multicast_ip.c_str());
		ui->leMulticastPort->setText(QString::number(ve->multicast_port));
		ui->leMulticastTTL->setText(QString::number(ve->multicast_ttl));

		ui->comboMulticastAutostart->addItem(ve->multicast_autostart ? "Yes" : "No");
		ui->comboMulticastAutostart->addItem(ve->multicast_autostart ? "No" : "Yes");

		ui->lblSessionTimeout->setText(ve->session_timeout.c_str());
	}
}
