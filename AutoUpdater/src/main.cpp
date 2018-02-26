#include <QApplication>
#include <QtWidgets>
#include "aupclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AupClient aupclient;
    aupclient.show();
//    QWidget w;
//    w.resize(500,70);
//    w.show();
//    QHBoxLayout *layout = new QHBoxLayout();
//    QPushButton *checkbtn = new QPushButton("Check for updates");
//    QPushButton *updatebtn = new QPushButton("Restart & Update");
//    QLineEdit *versionline = new QLineEdit("0.1");
//    versionline->setReadOnly(true);

//    layout->addWidget(versionline);
//    layout->addWidget(checkbtn);
//    layout->addWidget(updatebtn);
//    w.setLayout(layout);

    return a.exec();
}
