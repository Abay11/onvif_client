#include "streamhandler.h"

#include <gst/video/videooverlay.h>

#include <QDebug>
#include <QThread>
#include <QWidget>

StreamHandler::StreamHandler(QObject *parent) : QObject(parent)
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
		if(playbin_ == nullptr) //the first time of calling start
				{
						//due invoking this method from another thread leads to crash in Qt,
						//pass window ID explicitly
						quintptr winID = playerWidget_->winId();
						QThread* gstream_context_thread = QThread::create([this, winID]()
						{
								gst_init (nullptr, nullptr);

								playbin_ = gst_element_factory_make ("playbin", "playbin");
								videosink_ = gst_element_factory_make ("xvimagesink", nullptr);

								g_object_set(playbin_, "uri", streamURL_.toStdString().c_str(), NULL);

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

								/* Pass it to playbin, which implements VideoOverlay and will forward it to the video sink */
								if(playerWidget_)
										{
												guintptr wid = static_cast<guintptr>(winID);
												gst_video_overlay_set_window_handle(reinterpret_cast<GstVideoOverlay*>(playbin_), wid);
										}

								/* Start playing */
								GstStateChangeReturn ret = gst_element_set_state (playbin_, GST_STATE_PLAYING);
								if (ret == GST_STATE_CHANGE_FAILURE)
										{
												g_printerr ("Unable to set the pipeline to the playing state.\n");
												gst_object_unref (playbin_);
												return;
										}

								GMainLoop* main_loop = g_main_loop_new(nullptr, FALSE);

								g_main_loop_run(main_loop);
						});

						gstream_context_thread->start();

						return;
				}

		GstState curState;
		gst_element_get_state(playbin_, &curState, nullptr, 0);
		if(curState == GST_STATE_READY)
				{
						//the playing was stopped, restore PLAY
						GstStateChangeReturn ret = gst_element_set_state (playbin_, GST_STATE_PLAYING);
						if (ret == GST_STATE_CHANGE_FAILURE)
								{
										qWarning() << "Unable to set the pipeline to the PLAYING state after READY";
										gst_object_unref (playbin_);
										return;
								}
				}
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

		// to clear last frame and restore default background
		if(playerWidget_)
				{
						//playerWidget_->update();
						playerWidget_ = nullptr;
				}

		qDebug() << "Playing is stopped";
}
