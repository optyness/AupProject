#include "autoupdater.h"
#include "Windows.h"
#include <tchar.h>
#include <QDebug>

#define BUFSIZE 512
HANDLE g_hPipe = INVALID_HANDLE_VALUE;

AupInitStatus aup_init(aup_callback_fn callback, void* context)
{
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("REQUIRE_UPDATE");
    TCHAR inbuf[BUFSIZE];
    DWORD cbBytesRead = 0, cbWritten = 0;
    BOOL wSuccess = FALSE;
    BOOL rSuccess = FALSE;
    if(g_hPipe == INVALID_HANDLE_VALUE)
        g_hPipe = CreateFile(
                   lpNamedPipeName,   // pipe name
                   GENERIC_READ |  // read and write access
                   GENERIC_WRITE,
                   0,              // no sharing
                   NULL,           // default security attributes
                   OPEN_EXISTING,  // opens existing pipe
                   0,              // default attributes
                   NULL);          // no template file

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
//    fSuccess = CallNamedPipe(
//            lpNamedPipeName, // адрес имени канала
//            outbuf,     // адрес буфера для записи
//            (lstrlen(outbuf)+1)*sizeof(TCHAR),  // размер буфера для записи
//            inbuf,      // адрес буфера для чтения
//            BUFSIZE*sizeof(TCHAR),   // размер буфера для чтения
//            &lpBytesRead,     // адрес переменной для записи
//                               // количества прочитанных байт данных
//            10000        // время ожидания в миллисекундах
//            );
    if(!_tcscmp(inbuf,"UPDATE_READY")){
        callback(context);
        return AupRequireAppExit;
    }else{
        return AupOk;
    }
}

void aup_update_info(char *update_info)
{
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    TCHAR inbuf[BUFSIZE];
    DWORD lpBytesRead;
    DWORD cbWritten = 0;
    BOOL wSuccess = FALSE;
    if(g_hPipe == INVALID_HANDLE_VALUE)
        g_hPipe = CreateFile(
                   lpNamedPipeName,   // pipe name
                   GENERIC_READ |  // read and write access
                   GENERIC_WRITE,
                   0,              // no sharing
                   NULL,           // default security attributes
                   OPEN_EXISTING,  // opens existing pipe
                   0,              // default attributes
                   NULL);          // no template file

    wSuccess = WriteFile(
             g_hPipe,        // handle to pipe
             update_info,     // buffer to write from
             (lstrlen(update_info)+1)*sizeof(TCHAR), // number of bytes to write
             &cbWritten,   // number of bytes written
             NULL);        // not overlapped I/O
//    CallNamedPipe(
//            lpNamedPipeName, // адрес имени канала
//            update_info,     // адрес буфера для записи
//            (lstrlen(update_info)+1)*sizeof(TCHAR),  // размер буфера для записи
//            inbuf,      // адрес буфера для чтения
//            BUFSIZE*sizeof(TCHAR),   // размер буфера для чтения
//            &lpBytesRead,     // адрес переменной для записи
//                               // количества прочитанных байт данных
//            20000        // время ожидания в миллисекундах
//            );
}

void aup_start_update()
{
    LPCTSTR lpNamedPipeName = TEXT("\\\\.\\pipe\\AupInfo");
    LPTSTR outbuf = TEXT("START_UPDATE");
    DWORD cbWritten = 0;
    BOOL wSuccess = FALSE;
    if(g_hPipe == INVALID_HANDLE_VALUE)
        g_hPipe = CreateFile(
                   lpNamedPipeName,   // pipe name
                   GENERIC_READ |  // read and write access
                   GENERIC_WRITE,
                   0,              // no sharing
                   NULL,           // default security attributes
                   OPEN_EXISTING,  // opens existing pipe
                   0,              // default attributes
                   NULL);          // no template file

    wSuccess = WriteFile(
             g_hPipe,        // handle to pipe
             outbuf,     // buffer to write from
             (lstrlen(outbuf)+1)*sizeof(TCHAR), // number of bytes to write
             &cbWritten,   // number of bytes written
             NULL);        // not overlapped I/O
}

void aup_shutdown()
{
    CloseHandle(g_hPipe);
}
