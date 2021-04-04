#pragma once

#include <QDebug>
#include <QTime>


inline QDebug operator<<(QDebug dbg, const QTime& time)
{
		return dbg << QString("%1:%2:%3.%4")
					 .arg(time.hour())
					 .arg(time.minute())
					 .arg(time.second())
					 .arg(time.msec());
}


#define LOG_DEBUG(arg) qDebug() << "[DEBUG]" << QTime::currentTime() << __FUNCTION__ << ":" << arg
#define LOG_INFO(arg) qDebug() << "[INFO]" << QTime::currentTime() << __FUNCTION__ << ": " << arg
#define LOG_WARN(arg) qDebug() << "[WARN]" << QTime::currentTime() << __FUNCTION__ << ": " << arg
#define LOG_ERROR(arg) qDebug() << "[ERROR]" << QTime::currentTime() << __FUNCTION__ << ": " << arg
