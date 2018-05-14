#include <QApplication>
//#include <QtWidgets>
#include <QProgressBar>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget *w = new QWidget;
    w->setFixedSize(350,50);
    QHBoxLayout *layout = new QHBoxLayout;
    QProgressBar *update_bar = new QProgressBar;
    layout->addWidget(update_bar);
    w->setLayout(layout);

    w->show();

    return a.exec();
}
