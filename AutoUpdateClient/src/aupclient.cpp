#include "aupclient.h"

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
    connect(updatebtn, &QPushButton::clicked,
            this, &AupClient::clientUpdate);

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &AupClient::onNetworkResult);
}

void AupClient::updateCheck()
{
    QUrl url("http://localhost/aupinfo.json");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
    //TO DO: apply updater concept
    //aup_update_info();
    status = aup_init(&AupClient::prepareUpdate, this);
}

void AupClient::onNetworkResult(QNetworkReply *reply)
{
    //QString update_info;
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    }else{
        QFile *file = new QFile(QCoreApplication::applicationDirPath() + "/info.json");
        if(file->open(QFile::WriteOnly)){
//            file->write(reply->readAll());
//            file->close();
            QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
            update_info = json.object().value("version").toString() + " " +
                    json.object().value("url").toString();
            qDebug() << update_info;

        }
    }
    aup_update_info(update_info.toLocal8Bit().data());//.toLocal8Bit().constData()
}

void AupClient::prepareUpdate(void *context)
{
    static_cast<AupClient*>(context)->updatebtn->setEnabled(true);
}

void AupClient::clientUpdate()
{

}
