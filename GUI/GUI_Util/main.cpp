#include "mainwindow.h"

#include <QApplication>
#include "logging.h"

int main(int argc, char *argv[])
{
		QApplication a(argc, argv);
		try
				{


						LOG_DEBUG("running " << " a program");

						MainWindow w;
						w.show();
						return a.exec();
				}
		catch (const std::exception& e)
				{
						LOG_ERROR(QString("Finished with error: ") + e.what());

						return -1;
				}

}
