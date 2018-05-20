#include "aupclient.h"

AupClient::AupClient(QWidget *parent) : QWidget(parent), exitFlag(false)
{
    resize(500,70);
    QHBoxLayout *layout = new QHBoxLayout();
    QPushButton *checkbtn = new QPushButton("Check for updates");
    updatebtn = new QPushButton("Restart & Update");
    QLineEdit *versionline = new QLineEdit("1.0.2");
    versionline->setReadOnly(true);

    layout->addWidget(versionline);
    layout->addWidget(checkbtn);
    layout->addWidget(updatebtn);
    setLayout(layout);
    updatebtn->setEnabled(false);
    connect(checkbtn, &QPushButton::clicked,
            this, &AupClient::onUpdateCheck);
    connect(updatebtn, &QPushButton::clicked,
            this, &AupClient::onClientUpdate);

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &AupClient::onNetworkResult);
    connect(qApp,&QApplication::aboutToQuit,
            this,&AupClient::onClientExit);

    status = aup_init(&AupClient::prepareUpdate, this);
    if(status == AupRequireAppExit){
        onClientUpdate();
        exitFlag = true;
    }
}

void AupClient::onUpdateCheck()
{
    QUrl url("http://localhost/aupinfo.json");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
    //TO DO: apply updater concept
    //aup_update_info();
}

void AupClient::onNetworkResult(QNetworkReply *reply)
{
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    }else{
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        update_info = json.object().value("version").toString() + " " +
                json.object().value("url").toString() + " " +
                json.object().value("name").toString();
    }
    aup_update_info(update_info.toLocal8Bit().data());//.toLocal8Bit().constData()
    status = aup_init(&AupClient::prepareUpdate, this);
}

void AupClient::onClientExit()
{
    aup_shutdown();
}

void AupClient::prepareUpdate(void *context)
{
    static_cast<AupClient*>(context)->updatebtn->setEnabled(true);
}

bool AupClient::requireExit()
{
    return exitFlag;
}

void AupClient::onClientUpdate()
{
    QProcess *update_process = new QProcess;
    update_process->start("./AutoUpdateProgress/AutoUpdateProgress.exe");
    aup_start_update();
    QApplication::quit();
}
