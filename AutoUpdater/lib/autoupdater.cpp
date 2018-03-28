#include "autoupdater.h"
#include "Windows.h"

AupInitStatus aup_init(aup_callback_fn callback, void* context)
{
    callback(context);
    return AupRequireAppExit;
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
    //qDebug() << res << outbuf << inbuf << GetLastError();
}
