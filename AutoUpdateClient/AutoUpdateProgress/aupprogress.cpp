#include "aupprogress.h"
#include <tchar.h>
#define BUFSIZE 512

AupProgress::AupProgress(QWidget *parent) : QWidget(parent)
{
    setFixedSize(350,50);
    update_bar = new QProgressBar;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(update_bar);
    setLayout(layout);
}

void AupProgress::updateBarChange()
{
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("PROGRESS_PENDING");
    TCHAR inbuf[BUFSIZE] = "Not started";
    DWORD cbBytesRead = 0, cbWritten = 0;
    BOOL wSuccess = FALSE;
    BOOL rSuccess = FALSE;


    HANDLE g_hPipe = g_hPipe = CreateFile(
                   lpNamedPipeName,   // pipe name
                   GENERIC_READ |  // read and write access
                   GENERIC_WRITE,
                   0,              // no sharing
                   NULL,           // default security attributes
                   OPEN_EXISTING,  // opens existing pipe
                   0,              // default attributes
                   NULL);          // no template file

    while(!_tcscmp(inbuf,"UPDATE_FINISHED")){
        Sleep(1000);
        wSuccess = WriteFile(
                 g_hPipe,        // handle to pipe
                 outbuf,     // buffer to write from
                 (lstrlen(outbuf)+1)*sizeof(TCHAR), // number of bytes to write
                 &cbWritten,   // number of bytes written
                 NULL);        // not overlapped I/O
        rSuccess = ReadFile(
                 g_hPipe,        // handle to pipe
                 inbuf,    // buffer to receive data
                 BUFSIZE*sizeof(TCHAR), // size of buffer
                 &cbBytesRead, // number of bytes read
                 NULL);        // not overlapped I/O
        if(update_bar->value() < 98)
            update_bar->setValue(update_bar->value() + 11);
    }

    TCHAR keyName[] = "SOFTWARE\\AutoUpdateClient";
    DWORD bufferSize = MAX_PATH;
    TCHAR extract_path[MAX_PATH];
    RegGetValue(HKEY_LOCAL_MACHINE,
                keyName,
                "InstallDir",
                RRF_RT_ANY,
                NULL,
                extract_path,
                &bufferSize);

    _tcscat_s(extract_path, "\\AutoUpdateClient.exe");
    QProcess *update_process = new QProcess;
    update_process->start(extract_path);
    QApplication::quit();
}
