#include "audioinputgrabber.h"

#include <QAudioInput>
#include <QBuffer>
#include <QDebug>

AudioInputGrabber::AudioInputGrabber(write_handler_t handler)
{
		buffer_ = new QBuffer(this);
		if(!buffer_->open(QBuffer::ReadWrite))
				throw std::runtime_error("Cannot open a AudioInputGrabber's buffer!");

		handler_ = handler;

		connect(buffer_, &QBuffer::readyRead, this, &AudioInputGrabber::buffer_read_ready);
}

AudioInputGrabber::~AudioInputGrabber()
{
		buffer_->close();
}

void AudioInputGrabber::Start()
{
		qDebug() << __FUNCTION__;
		QAudioFormat format;
		// Set up the desired format, for example:
		format.setSampleRate(8000);
		format.setChannelCount(1);
		format.setSampleSize(8);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::UnSignedInt);

		QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
		if (!info.isFormatSupported(format))
				{
						qWarning() << "Default format not supported, trying to use the nearest.";
						format = info.nearestFormat(format);
				}


		audio_input_ = new QAudioInput(format, this);
		connect(audio_input_, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
		audio_input_->start(buffer_);
}

void AudioInputGrabber::Stop()
{
		qDebug() << __FUNCTION__;
		audio_input_->stop();
		delete audio_input_;
}

void AudioInputGrabber::handleStateChanged(QAudio::State state)
{
		qDebug() << "QAudioInput state changed: " << state;
}

void AudioInputGrabber::buffer_read_ready()
{
		//TODO: sending to a driver
		qDebug() << "buffer size: " << buffer_->size();

		qDebug() << "buffer is ready. av. size: " << buffer_->bytesAvailable();

		auto bytes = buffer_->bytesAvailable();
		auto bytes_array = buffer_->readAll();
		handler_(bytes_array.data(), bytes);
		//buffer_->readAll();
		//buffer_->buffer().clear();
		//buffer_->skip(buffer_->bytesAvailable());
		//buffer_->skip(buffer_->bytesAvailable());
}



