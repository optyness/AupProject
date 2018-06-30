#include "serviceinit.h"

#define VERSIZE 12
#define BUFSIZE 512
HANDLE hPipe = INVALID_HANDLE_VALUE;

class Logger {
public:
    Logger(const char* file){
        outputFile.open(file);
    }
    ~Logger(){
        outputFile.close();
    }
    void addMessage(const char* text){
        outputFile << text << std::endl;
    }
    void addMessage(const int num){
        outputFile << num << std::endl;
    }
private:
    std::ofstream outputFile;
};

BOOL InitService(){

    PROCESS_INFORMATION pi;
    LPSTR  lpszPipeName = "\\\\.\\pipe\\AupInfo";
    TCHAR pchRequest[BUFSIZE];
    TCHAR* pchReply = "UNDEFINED";

    BOOL fSuccess = FALSE;
    DWORD cbBytesRead = 0, cbWritten = 0;

    Logger logger("C:\\result.txt");

    TCHAR inst_path[MAX_PATH];
    GetEnvironmentVariable("PROGRAMFILES", inst_path, MAX_PATH);
    _tcscat_s(inst_path, "\\update.exe");
    logger.addMessage(inst_path);

    //    size_t found = string(servicePath).find_last_of("\\");
    //    _tcsncpy(path,servicePath,found);
    //    found = string(path).find_last_of("\\");
    //    _tcsncpy(extract_path,path,++found);
    //    _tcscat(path, _T("\\update.exe"));

    logger.addMessage("START SERVICE");
    hPipe = CreateNamedPipe(
                lpszPipeName,
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                512,
                512,
                0,
                NULL
                );
    ConnectNamedPipe(hPipe, NULL);
    logger.addMessage("CREATE PIPE");

    for(;;){

        if(!ReadFile(hPipe,          // handle to pipe
               pchRequest,          // buffer to receive data
               BUFSIZE*sizeof(TCHAR), // size of buffer
               &cbBytesRead,        // number of bytes read
               NULL                 // not overlapped I/O){
        )){
            int err = GetLastError();
            logger.addMessage(err);
            if(err == 109){
                DisconnectNamedPipe(hPipe);
                ConnectNamedPipe(hPipe, NULL);
            }else{
                return 1;
            }
        }

        if(!_tcscmp(pchRequest,"REQUIRE_UPDATE")){
            logger.addMessage(pchRequest);
            if(GetFileAttributes(inst_path) != DWORD(-1)){
                pchReply = "UPDATE_READY";
                logger.addMessage(pchReply);
                fSuccess = WriteFile(
                         hPipe,        // handle to pipe
                         pchReply,     // buffer to write from
                         (lstrlen(pchReply)+1)*sizeof(TCHAR), // number of bytes to write
                         &cbWritten,   // number of bytes written
                         NULL);        // not overlapped I/O
            }else{
                pchReply = "UPDATE_NOT_READY";
                logger.addMessage(pchReply);
                fSuccess = WriteFile(
                         hPipe,        // handle to pipe
                         pchReply,     // buffer to write from
                         (lstrlen(pchReply)+1)*sizeof(TCHAR), // number of bytes to write
                         &cbWritten,   // number of bytes written
                         NULL);        // not overlapped I/O
            }
        }else if(!_tcscmp(pchRequest,"START_UPDATE")){
            logger.addMessage(pchRequest);
            DWORD bufferSize = MAX_PATH;
            TCHAR extract_path[MAX_PATH];
            TCHAR keyName[] = "SOFTWARE\\";
            _tcscat_s(keyName, "AutoUpdateClient");
            RegGetValue(HKEY_LOCAL_MACHINE,
                        keyName,
                        "InstallDir",
                        RRF_RT_ANY,
                        NULL,
                        extract_path,
                        &bufferSize);
            // additional information
            TCHAR cmdArg[MAX_PATH];
            _tcscpy_s(cmdArg, "/S /D=");
            _tcscat_s(cmdArg, extract_path);

            logger.addMessage(cmdArg);
            STARTUPINFO si;
            //PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            if(!CreateProcess(
                inst_path,      // the path
                cmdArg,         // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory
                &si,            // Pointer to STARTUPINFO structure
                &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
            )){
                int err = GetLastError();
                logger.addMessage(err);
            }
    //            addLogMessage("PROGRESS");
        }else if(!_tcscmp(pchRequest,"PROGRESS_PENDING")){
            DWORD exitCode = 0;
            GetExitCodeProcess(pi.hProcess, &exitCode);
            if(exitCode == STILL_ACTIVE){

            }else{
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }else{
            //-------------message with update information
            logger.addMessage(pchRequest);
            DWORD bufferSize = VERSIZE;
            TCHAR currentVersion[VERSIZE];
            TCHAR keyName[BUFSIZE] = "SOFTWARE\\";
            logger.addMessage("1 point");
            std::istringstream iss(pchRequest);
            std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                  std::istream_iterator<std::string>{}};

            _tcscat_s(keyName, tokens[2].c_str());
            logger.addMessage("2 point");
            RegGetValue(HKEY_LOCAL_MACHINE,
                        keyName,
                        "Version",
                        RRF_RT_ANY,
                        NULL,
                        currentVersion,
                        &bufferSize);
    //            addLogMessage(currentVersion);

    //            ostringstream stream;
    //            stream << inst_path << "\\" << tokens[2].c_str() << ".exe";

    //            _tcscpy(inst_path, stream.str().c_str());

    //            DWORD infoSize = 0;
    //            void *fileInfo;
    //            VS_FIXEDFILEINFO *verInfo;
    //            LPBYTE lpBuffer  = NULL;
    //            UINT size = 0;
    //            DWORD verSize = GetFileVersionInfoSize(inst_path,
    //                                   &infoSize);
    //            GetFileVersionInfo(inst_path,
    //                               infoSize,
    //                               verSize,
    //                               fileInfo);
    //            VerQueryValue(fileInfo,
    //                          "\\",
    //                          (LPVOID*)&verInfo,
    //                          &size);
    //            VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;

    //            int revision = HIWORD(verInfo->dwProductVersionLS);
    //            int build = LOWORD(verInfo->dwProductVersionLS);

    //            ofstream outputFile("C:\\result.txt",ios_base::app);
    //            outputFile << HIWORD(verInfo->dwProductVersionMS) << endl;
    //            outputFile << LOWORD(verInfo->dwProductVersionMS) << endl;
    //            outputFile.close();

    //            delete[] fileInfo;

            logger.addMessage("CHECK_DOWNLOAD");
            if(_tcscmp(tokens[0].c_str(),currentVersion) > 0){
    //                if(GetFileAttributes(inst_path) == DWORD(-1)){
                logger.addMessage("START_DOWNLOAD");
                    HRESULT hr = URLDownloadToFile(NULL, _T(tokens[1].c_str()),
                            inst_path, 0, NULL);
                    logger.addMessage("FINISH_DOWNLOAD");
    //                }
            }
        }
        //DisconnectNamedPipe(hPipe);
    }
    CloseHandle(hPipe);
    return 0;
}
