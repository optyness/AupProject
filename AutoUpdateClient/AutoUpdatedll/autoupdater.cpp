#include "autoupdater.h"
#include "Windows.h"
#include <tchar.h>
#include <QDebug>

AupInitStatus aup_init(aup_callback_fn callback, void* context)
{
    int BUFSIZE = 512;
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("REQUIRE_UPDATE");
    TCHAR inbuf[512];
    DWORD lpBytesRead;

    BOOL fSuccess = FALSE;
    fSuccess = CallNamedPipe(
            lpNamedPipeName, // адрес имени канала
            outbuf,     // адрес буфера для записи
            (lstrlen(outbuf)+1)*sizeof(TCHAR),  // размер буфера для записи
            inbuf,      // адрес буфера для чтения
            BUFSIZE*sizeof(TCHAR),   // размер буфера для чтения
            &lpBytesRead,     // адрес переменной для записи
                               // количества прочитанных байт данных
            10000        // время ожидания в миллисекундах
            );
//    if(fSuccess){
//        qDebug() << "good" << endl;
//    }else{
//        qDebug() << "error" << GetLastError() << endl;
//    }
    if(!_tcscmp(inbuf,"UPDATE_READY")){
        callback(context);
        return AupRequireAppExit;
    }else{
        return AupOk;
    }
}

void aup_update_info(char *update_info){
    int BUFSIZE = 512;
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    //LPTSTR outbuf = TEXT("Message from client");
    TCHAR inbuf[512];
    DWORD lpBytesRead;
    CallNamedPipe(
            lpNamedPipeName, // адрес имени канала
            update_info,     // адрес буфера для записи
            (lstrlen(update_info)+1)*sizeof(TCHAR),  // размер буфера для записи
            inbuf,      // адрес буфера для чтения
            BUFSIZE*sizeof(TCHAR),   // размер буфера для чтения
            &lpBytesRead,     // адрес переменной для записи
                               // количества прочитанных байт данных
            20000        // время ожидания в миллисекундах
            );
}

void aup_start_update()
{
    int BUFSIZE = 512;
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("START_UPDATE");
    //TCHAR inbuf[512];
    DWORD cbWritten = 0;
    BOOL fSuccess = FALSE;
    HANDLE hPipe = CreateFile(
               lpNamedPipeName,   // pipe name
               GENERIC_READ |  // read and write access
               GENERIC_WRITE,
               0,              // no sharing
               NULL,           // default security attributes
               OPEN_EXISTING,  // opens existing pipe
               0,              // default attributes
               NULL);          // no template file
//    if(hPipe != INVALID_HANDLE_VALUE){
//        qDebug() << "good" << endl;
//    }else{
//        qDebug() << GetLastError() << endl;
//    }
    fSuccess = WriteFile(
             hPipe,        // handle to pipe
             outbuf,     // buffer to write from
             (lstrlen(outbuf)+1)*sizeof(TCHAR), // number of bytes to write
             &cbWritten,   // number of bytes written
             NULL);        // not overlapped I/O
//    if(fSuccess){
//        qDebug() << "good" << endl;
//    }else{
//        qDebug() << GetLastError() << endl;
//    }
    CloseHandle(hPipe);
}
