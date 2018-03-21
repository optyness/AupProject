#include "aupclient.h"

AupClient::AupClient(QWidget *parent) : QWidget(parent), sharedMemory("AupInfo")
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

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &AupClient::onNetworkResult);
}

void AupClient::updateCheck()
{
    //проверка наличия обновления через сервис
    QUrl url("http://localhost/aupinfo.json");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);

    status = aup_init(&AupClient::prepareUpdate, this);
}

void AupClient::onNetworkResult(QNetworkReply *reply)
{
    if(reply->error()){
        qDebug() << "Error";
        qDebug() << reply->errorString();
    }else{
        QFile *file = new QFile(QCoreApplication::applicationDirPath() + "/result.json");
        if(file->open(QFile::WriteOnly)){
            file->write(reply->readAll());
            file->close();
        }
    }

    int BUFSIZE = 512;
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("Message from client");
    TCHAR inbuf[512];
    DWORD lpBytesRead;
    bool res = CallNamedPipe(
            lpNamedPipeName, // адрес имени канала
            outbuf,     // адрес буфера для записи
            (lstrlen(outbuf)+1)*sizeof(TCHAR),  // размер буфера для записи
            inbuf,      // адрес буфера для чтения
            BUFSIZE*sizeof(TCHAR),   // размер буфера для чтения
            &lpBytesRead,     // адрес переменной для записи
                               // количества прочитанных байт данных
            20000        // время ожидания в миллисекундах
            );
    qDebug() << res << outbuf << inbuf << GetLastError();

}

void AupClient::prepareUpdate(void *context)
{
    static_cast<AupClient*>(context)->updatebtn->setEnabled(true);
}
