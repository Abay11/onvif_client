#ifndef AUDIOINPUTGRABBER_H
#define AUDIOINPUTGRABBER_H

#include <functional>

#include <QObject>
#include <QAudio>

class QAudioInput;
class QBuffer;

class AudioInputGrabber : public QObject
{
		Q_OBJECT

public:
		using write_handler_t = std::function<void(void* /*data*/, size_t /*size*/)>;
		AudioInputGrabber(write_handler_t /*handler*/);
		~AudioInputGrabber();

		void Start();
		void Stop();

private:
		void handleStateChanged(QAudio::State);

		void buffer_read_ready();

private:
		QAudioInput* audio_input_;
		QBuffer* buffer_;

		write_handler_t handler_;
};

#endif // AUDIOINPUTGRABBER_H
