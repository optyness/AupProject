#include <QApplication>
#include <QtWidgets>
#include "aupprogress.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AupProgress progressWindow;
    progressWindow.show();

    return a.exec();
}
