#ifndef AUPPROGRESS_H
#define AUPPROGRESS_H

#include <QtWidgets>

class AupProgress : public QWidget
{
public:
    AupProgress(QWidget *parent = nullptr);
    void updateBarChange();

private:
    QProgressBar *update_bar;
};

#endif // AUPPROGRESS_H
