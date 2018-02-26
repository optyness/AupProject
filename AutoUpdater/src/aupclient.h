#ifndef AUPCLIENT_H
#define AUPCLIENT_H

#include <QWidget>
#include <QPushButton>
#include "autoupdater.h"

class AupClient : public QWidget
{
    Q_OBJECT
public:
    explicit AupClient(QWidget *parent = nullptr);

signals:

public slots:
    void updateCheck();

private:
    AupInitStatus status;
    QPushButton *updatebtn;
};

void prepareUpdate();

#endif // AUPCLIENT_H
