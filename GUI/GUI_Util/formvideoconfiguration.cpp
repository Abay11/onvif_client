#include "formvideoconfiguration.h"
#include "ui_formvideoconfiguration.h"

#include <QDebug>

FormVideoConfiguration::FormVideoConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoConfiguration)
{
    ui->setupUi(this);
}

FormVideoConfiguration::~FormVideoConfiguration()
{
    delete ui;
	qDebug() << "FormVideoConfiguration deleted";
}

void FormVideoConfiguration::fillInfo(const _onvif::VideoSourcesSP videoSources,
																			const _onvif::Profiles* profiles)
{
	//get video sources
	QStringList video_sources_tokens;
	for(const auto& token : videoSources->tokens)
	{
		video_sources_tokens.push_back(token.c_str());
	}
	ui->comboVideoSources->clear();
	ui->comboVideoSources->addItems(video_sources_tokens);

	//fill media profiles
	QStringList profile_names;
	for(const auto& p : *profiles)
	{
		profile_names.push_back(p->Name.c_str());
	}

	ui->comboMediaProfiles->clear();
	ui->comboMediaProfiles->addItems(profile_names);
}
