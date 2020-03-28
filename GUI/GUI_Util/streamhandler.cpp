#include "streamhandler.h"

#include <gst/gst.h>
#include <gst/video/videooverlay.h>

StreamHandler::StreamHandler(QObject *parent) : QObject(parent)
{
	connect(this, &StreamHandler::sigStartStream,
					this, &StreamHandler::slotStartStream);
}

void StreamHandler::startStream(const QString& uri, quintptr wID)
{
	emit sigStartStream(uri, wID);
}

void StreamHandler::slotStartStream(const QString& uri, quintptr windowID)
{
	gint winID = static_cast<gint>(windowID);

	gst_init (NULL, NULL);

		 GstElement* playbin = gst_element_factory_make ("playbin", "playbin");
		 GstElement* videosink = gst_element_factory_make ("xvimagesink", NULL);

		 g_object_set(playbin, "uri", uri.toStdString().c_str(), NULL);

		 /* Set flags to show Audio and Video but ignore Subtitles */
		 /* playbin flags */
		 typedef enum {
			 GST_PLAY_FLAG_VIDEO         = (1 << 0), /* We want video output */
			 GST_PLAY_FLAG_AUDIO         = (1 << 1), /* We want audio output */
			 GST_PLAY_FLAG_TEXT          = (1 << 2)  /* We want subtitle output */
		 } GstPlayFlags;

		 gint flags;
		 g_object_get(playbin, "flags", &flags, NULL);
		 flags |= GST_PLAY_FLAG_VIDEO;
		 flags &= ~GST_PLAY_FLAG_AUDIO;
		 g_object_set (playbin, "flags", flags, NULL);
		 g_object_set (playbin, "video-sink", videosink, NULL);

		 /* Pass it to playbin, which implements VideoOverlay and will forward it to the video sink */
			gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (playbin), winID);

		 /* Start playing */
		 GstStateChangeReturn ret = gst_element_set_state (playbin, GST_STATE_PLAYING);
		 if (ret == GST_STATE_CHANGE_FAILURE) {
				 g_printerr ("Unable to set the pipeline to the playing state.\n");
				 gst_object_unref (playbin);
				 return;
			 }

		 GMainLoop* main_loop = g_main_loop_new(nullptr, FALSE);

		 g_main_loop_run(main_loop);
}

void StreamHandler::slotStopStream()
{
}
