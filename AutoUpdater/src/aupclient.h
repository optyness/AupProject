#ifndef AUPCLIENT_H
#define AUPCLIENT_H

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
    void clientUpdate();
    void onNetworkResult(QNetworkReply *reply);

private:
    AupInitStatus status;
    QPushButton *updatebtn;
    QNetworkAccessManager *manager;
    QString update_info;
};

#endif // AUPCLIENT_H
