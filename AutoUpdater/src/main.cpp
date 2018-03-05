#include <QApplication>
#include <QtWidgets>
#include "aupclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AupClient aupclient;
    aupclient.show();

    return a.exec();
}
