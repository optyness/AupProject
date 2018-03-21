#ifndef AUPCLIENT_H
#define AUPCLIENT_H

//#include <QWidget>
#include <QtWidgets>
#include <QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include "autoupdater.h"

class AupClient : public QWidget
{
    Q_OBJECT
public:
    explicit AupClient(QWidget *parent = nullptr);
    static void prepareUpdate(void *context);
signals:

public slots:
    void updateCheck();
    void onNetworkResult(QNetworkReply *reply);

private:
    AupInitStatus status;
    QPushButton *updatebtn;
    QNetworkAccessManager *manager;
    QSharedMemory sharedMemory;
};

#endif // AUPCLIENT_H
