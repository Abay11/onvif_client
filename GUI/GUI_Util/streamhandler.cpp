#include "streamhandler.h"

#include <gst/video/videooverlay.h>

#include <QDebug>
#include <QThread>
#include <QWidget>

StreamHandler::StreamHandler(QObject *parent) : QObject(parent),
		gstream_context_thread(new QThread(this))
{
		connect(this, &StreamHandler::sigStartStream,
						this, &StreamHandler::slotStartStream);
		connect(this, &StreamHandler::sigStopStream,
						this, &StreamHandler::slotStopStream);
}

StreamHandler::~StreamHandler()
{
		gst_element_set_state(playbin_, GST_STATE_NULL);
		gst_object_unref(playbin_);
		gst_object_unref(playbin_);
		delete gstream_context_thread;
}

void StreamHandler::startStream(const QString &url, QWidget* player)
{
		streamURL_ = url;
		playerWidget_ = player;

		emit sigStartStream();
}

void StreamHandler::stopStream()
{
		emit sigStopStream();
}

void StreamHandler::slotStartStream()
{
		//init GStreamer elements
		if(playbin_ == nullptr)
				{
						gst_init (nullptr, nullptr);

						playbin_ = gst_element_factory_make ("playbin", "playbin");
						videosink_ = gst_element_factory_make ("xvimagesink", nullptr);


						/* Set flags to show Audio and Video but ignore Subtitles */
						/* playbin flags */
						typedef enum
						{
								GST_PLAY_FLAG_VIDEO         = (1 << 0), /* We want video output */
								GST_PLAY_FLAG_AUDIO         = (1 << 1), /* We want audio output */
								GST_PLAY_FLAG_TEXT          = (1 << 2)  /* We want subtitle output */
						} GstPlayFlags;

						gint flags;
						g_object_get(playbin_, "flags", &flags, NULL);
						flags |= GstPlayFlags::GST_PLAY_FLAG_VIDEO;
						flags &= ~GST_PLAY_FLAG_AUDIO;
						g_object_set (playbin_, "flags", flags, NULL);
						g_object_set (playbin_, "video-sink", videosink_, NULL);

						main_loop = g_main_loop_new(nullptr, FALSE);

						/* Start playing */
						connect(gstream_context_thread, &QThread::started,
										[this]()
						{
								GstStateChangeReturn ret = gst_element_set_state (playbin_, GST_STATE_PLAYING);
								if (ret == GST_STATE_CHANGE_FAILURE)
										{
												qWarning("Unable to set the pipeline to the playing state.\n");
												gst_object_unref (playbin_);
												return;
										}

								g_main_loop_run(main_loop);
						});
				}

		/* Pass it to playbin, which implements VideoOverlay and will forward it to the video sink */
		if(playerWidget_)
				{
						guintptr wid = static_cast<guintptr>(playerWidget_->winId());
						gst_video_overlay_set_window_handle(reinterpret_cast<GstVideoOverlay*>(playbin_), wid);
				}
		/* Set stream URI */
		g_object_set(playbin_, "uri", streamURL_.toStdString().c_str(), NULL);


		gstream_context_thread->start();
}

void StreamHandler::slotStopStream()
{
		/* Stop playing */
		GstStateChangeReturn ret = gst_element_set_state (playbin_, GST_STATE_READY);
		if (ret == GST_STATE_CHANGE_FAILURE)
				{
						qWarning() << "Unable to set the pipeline to the READY state";
						gst_object_unref (playbin_);
						return;
				}

		g_main_loop_quit(main_loop);

		gstream_context_thread->quit();

		qDebug() << "Playing is stopped";
}
