#include "mainwindow.h"

#include <QApplication>
#include "logging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

		LOG_DEBUG("running " << " a program");

    MainWindow w;
    w.show();
    return a.exec();
}
