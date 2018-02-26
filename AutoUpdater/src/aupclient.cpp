#include "aupclient.h"
//#include "autoupdater.cpp"
#include <QtWidgets>

AupClient::AupClient(QWidget *parent) : QWidget(parent)
{
    resize(500,70);
    QHBoxLayout *layout = new QHBoxLayout();
    QPushButton *checkbtn = new QPushButton("Check for updates");
    updatebtn = new QPushButton("Restart & Update");
    QLineEdit *versionline = new QLineEdit("0.1");
    versionline->setReadOnly(true);

    layout->addWidget(versionline);
    layout->addWidget(checkbtn);
    layout->addWidget(updatebtn);
    setLayout(layout);
    updatebtn->setEnabled(false);
    connect(checkbtn, &QPushButton::clicked,
            this, &AupClient::updateCheck);
}

void AupClient::updateCheck()
{
    //проверка наличия обновления через сервис
    status = aup_init(&prepareUpdate);
}


void prepareUpdate()
{

}
