#ifndef AUPCLIENT_H
#define AUPCLIENT_H

#include <QtWidgets>
#include <QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include "AutoUpdatedll/autoupdater.h"

class AupClient : public QWidget
{
    Q_OBJECT
public:
    explicit AupClient(QWidget *parent = nullptr);
    static void prepareUpdate(void *context);
    bool requireExit();
signals:

public slots:
    void onUpdateCheck();
    void onClientUpdate();
    void onNetworkResult(QNetworkReply *reply);
    void onClientExit();

private:
    AupInitStatus status;
    QPushButton *updatebtn;
    QNetworkAccessManager *manager;
    QString update_info;
    bool exitFlag;
};

#endif // AUPCLIENT_H
