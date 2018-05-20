#include <QApplication>
#include <QtWidgets>
#include <QProgressBar>
#include <QHBoxLayout>
#include <Windows.h>
#include <tchar.h>

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

//    Sleep(5000);

//    TCHAR keyName[] = "SOFTWARE\\AutoUpdateClient";
//    DWORD bufferSize = MAX_PATH;
//    TCHAR extract_path[MAX_PATH];
//    RegGetValue(HKEY_LOCAL_MACHINE,
//                keyName,
//                "InstallDir",
//                RRF_RT_ANY,
//                NULL,
//                extract_path,
//                &bufferSize);

//    _tcscat(extract_path, "\\AutoUpdateClient.exe");
//    QProcess *update_process = new QProcess;
//    update_process->start(extract_path);
//    QApplication::exit();

    return a.exec();
}
