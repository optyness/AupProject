#include <iostream>
#include <Windows.h>
#include <fstream>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
HANDLE hPipe = INVALID_HANDLE_VALUE;

#define BUFSIZE 512
#define VERSIZE 12
#define service_name TEXT("AutoUpdateService")
TCHAR servicePath[MAX_PATH];

int addcmdMessage(const char* text)
{
    return printf(text);
}

int addLogMessage(const char* text)
{
    ofstream outputFile("C:\\result.txt",ios_base::app);
    outputFile << text << endl;
    //outputFile << _tcscat(path, "\\update.exe") << _tcscat(" /D=", path);
    outputFile.close();
    return 0;
}

void controlHandler(DWORD request){
    switch(request){
        case SERVICE_CONTROL_STOP:
            addcmdMessage("Stopped.");

            serviceStatus.dwWin32ExitCode = 0;
            serviceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus (serviceStatusHandle, &serviceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            addcmdMessage("Shutdown.");

            serviceStatus.dwWin32ExitCode = 0;
            serviceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus (serviceStatusHandle, &serviceStatus);
            return;

        default:
        break;
    }

    SetServiceStatus (serviceStatusHandle, &serviceStatus);

    return;
}

void serviceMain(int argc, char** argv){
    //int error;
    int i = 0;

    serviceStatus.dwServiceType    = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState    = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted  = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode   = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint     = 0;
    serviceStatus.dwWaitHint      = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(service_name, (LPHANDLER_FUNCTION)controlHandler);
    if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0){
        return;
    }

//    error = InitService();
//    if (error){
//        serviceStatus.dwCurrentState    = SERVICE_STOPPED;
//        serviceStatus.dwWin32ExitCode   = -1;
//        SetServiceStatus(serviceStatusHandle, &serviceStatus);
//        return;
//    }

    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus (serviceStatusHandle, &serviceStatus);

    LPSTR  lpszPipeName = "\\\\.\\pipe\\AupInfo";
    TCHAR pchRequest[BUFSIZE];
    TCHAR* pchReply = "UNDEFINED";

    BOOL fSuccess = FALSE;
    DWORD cbBytesRead = 0, cbWritten = 0;

    ofstream outputFile("C:\\result.txt");
    outputFile.close();

    TCHAR inst_path[MAX_PATH];
    GetEnvironmentVariable("PROGRAMFILES", inst_path, MAX_PATH);
    _tcscat(inst_path, "\\update.exe");
    addLogMessage(inst_path);

//    size_t found = string(servicePath).find_last_of("\\");
//    _tcsncpy(path,servicePath,found);
//    found = string(path).find_last_of("\\");
//    _tcsncpy(extract_path,path,++found);
//    _tcscat(path, _T("\\update.exe"));

    addLogMessage("START SERVICE");
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
    addLogMessage("CREATE PIPE");


    for(;;){

        if(!ReadFile(hPipe,          // handle to pipe
               pchRequest,          // buffer to receive data
               BUFSIZE*sizeof(TCHAR), // size of buffer
               &cbBytesRead,        // number of bytes read
               NULL                 // not overlapped I/O){
        )){
            int err = GetLastError();
            ofstream outputFile("C:\\result.txt",ios_base::app);
            outputFile << err << endl;
            outputFile.close();
            if(err == 109){
                DisconnectNamedPipe(hPipe);
                ConnectNamedPipe(hPipe, NULL);
            }else{
                return;
            }
        }

        if(!_tcscmp(pchRequest,"REQUIRE_UPDATE")){
            addLogMessage(pchRequest);
            if(GetFileAttributes(inst_path) != DWORD(-1)){
                pchReply = "UPDATE_READY";
                addLogMessage(pchReply);
                fSuccess = WriteFile(
                         hPipe,        // handle to pipe
                         pchReply,     // buffer to write from
                         (lstrlen(pchReply)+1)*sizeof(TCHAR), // number of bytes to write
                         &cbWritten,   // number of bytes written
                         NULL);        // not overlapped I/O
            }else{
                pchReply = "UPDATE_NOT_READY";
                addLogMessage(pchReply);
                fSuccess = WriteFile(
                         hPipe,        // handle to pipe
                         pchReply,     // buffer to write from
                         (lstrlen(pchReply)+1)*sizeof(TCHAR), // number of bytes to write
                         &cbWritten,   // number of bytes written
                         NULL);        // not overlapped I/O
            }
        }else if(!_tcscmp(pchRequest,"START_UPDATE")){
            addLogMessage(pchRequest);

            DWORD bufferSize = MAX_PATH;
            TCHAR extract_path[MAX_PATH];
            TCHAR keyName[] = "SOFTWARE\\";
            _tcscat(keyName, "AutoUpdateClient");
            RegGetValue(HKEY_LOCAL_MACHINE,
                        keyName,
                        "InstallDir",
                        RRF_RT_ANY,
                        NULL,
                        extract_path,
                        &bufferSize);
            // additional information
            TCHAR cmdArg[MAX_PATH];
            _tcscpy(cmdArg, "/S /D=");
            _tcscat(cmdArg, extract_path);

            addLogMessage(cmdArg);
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            // set the size of the structures
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            //ZeroMemory(&pi, sizeof(pi));
            if(!CreateProcess(
                inst_path,// the path
                cmdArg,        // Command line
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
                ofstream outputFile("C:\\result.txt",ios_base::app);
                outputFile << err << endl;
                outputFile.close();
            }
            addLogMessage("PROGRESS");
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }else{
            //-------------message with update information
            addLogMessage(pchRequest);
            DWORD bufferSize = VERSIZE;
            TCHAR currentVersion[VERSIZE];
            TCHAR keyName[] = "SOFTWARE\\";

            istringstream iss(pchRequest);
            vector<string> tokens{istream_iterator<string>{iss},
                                  istream_iterator<string>{}};

            _tcscat(keyName, tokens[2].c_str());
            RegGetValue(HKEY_LOCAL_MACHINE,
                        keyName,
                        "Version",
                        RRF_RT_ANY,
                        NULL,
                        currentVersion,
                        &bufferSize);
            addLogMessage(currentVersion);

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

            if(_tcscmp(tokens[0].c_str(),currentVersion) > 0){
//                if(GetFileAttributes(inst_path) == DWORD(-1)){
                    HRESULT hr = URLDownloadToFile(NULL, _T(tokens[1].c_str()),
                            inst_path, 0, NULL);
//                }
            }
        }
        //DisconnectNamedPipe(hPipe);
    }
    CloseHandle(hPipe);
    return;
}

int installService(){
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if(!hSCManager){
        addcmdMessage("Error: Can't open Service Control Manager");
        return -1;
    }

    SC_HANDLE hService = CreateService(
        hSCManager,
        service_name,
        service_name,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        servicePath,
        NULL, NULL, NULL, NULL, NULL
    );

    if(!hService){
        int err = GetLastError();
        switch(err){
            case ERROR_ACCESS_DENIED:
                addcmdMessage("Error: ERROR_ACCESS_DENIED");
                break;
            case ERROR_CIRCULAR_DEPENDENCY:
                addcmdMessage("Error: ERROR_CIRCULAR_DEPENDENCY");
                break;
            case ERROR_DUPLICATE_SERVICE_NAME:
                addcmdMessage("Error: ERROR_DUPLICATE_SERVICE_NAME");
                break;
            case ERROR_INVALID_HANDLE:
                addcmdMessage("Error: ERROR_INVALID_HANDLE");
                break;
            case ERROR_INVALID_NAME:
                addcmdMessage("Error: ERROR_INVALID_NAME");
                break;
            case ERROR_INVALID_PARAMETER:
                addcmdMessage("Error: ERROR_INVALID_PARAMETER");
                break;
            case ERROR_INVALID_SERVICE_ACCOUNT:
                addcmdMessage("Error: ERROR_INVALID_SERVICE_ACCOUNT");
                break;
             case ERROR_SERVICE_EXISTS:
                addcmdMessage("Error: ERROR_SERVICE_EXISTS");
                break;
            default:
                addcmdMessage("Error: Undefined");
        }
        CloseServiceHandle(hSCManager);
        return -1;
    }
    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);
    addcmdMessage("Success install service!");
    return 0;
}

int removeService(){
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if(!hSCManager){
        addcmdMessage("Error: Can't open Service Control Manager");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, service_name, SERVICE_STOP | DELETE);
    if(!hService){
        addcmdMessage("Error: Can't remove service");
        CloseServiceHandle(hSCManager);
        return -1;
    }

    DeleteService(hService);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    addcmdMessage("Success remove service!");
    return 0;
}

int startAupService() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if(!hSCManager){
        addcmdMessage("Error: Can't open Service Control Manager");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, service_name, SERVICE_START);
    if(!StartService(hService, 0, NULL)){
        CloseServiceHandle(hSCManager);
        //addLogMessage("Error: Can't start service");
        int err = GetLastError();
        switch(err){
            case ERROR_ACCESS_DENIED:
                addcmdMessage("Error: ERROR_ACCESS_DENIED");
                break;
            case ERROR_INVALID_HANDLE:
                addcmdMessage("Error: ERROR_INVALID_HANDLE");
                break;
            case ERROR_PATH_NOT_FOUND:
                addcmdMessage("Error: ERROR_PATH_NOT_FOUND");
                break;
            case ERROR_SERVICE_ALREADY_RUNNING:
                addcmdMessage("Error: ERROR_SERVICE_ALREADY_RUNNING");
                break;
            case ERROR_SERVICE_DATABASE_LOCKED:
                addcmdMessage("Error: ERROR_SERVICE_DATABASE_LOCKED");
                break;
            case ERROR_SERVICE_DEPENDENCY_DELETED:
                addcmdMessage("Error: ERROR_SERVICE_DEPENDENCY_DELETED");
                break;
            case ERROR_SERVICE_DEPENDENCY_FAIL:
                addcmdMessage("Error: ERROR_SERVICE_DEPENDENCY_FAIL");
                break;
            case ERROR_SERVICE_DISABLED:
                addcmdMessage("Error: ERROR_SERVICE_DISABLED");
                break;
            case ERROR_SERVICE_LOGON_FAILED:
                addcmdMessage("Error: ERROR_SERVICE_LOGON_FAILED");
                break;
            case ERROR_SERVICE_MARKED_FOR_DELETE:
                addcmdMessage("Error: ERROR_SERVICE_MARKED_FOR_DELETE");
                break;
            case ERROR_SERVICE_NO_THREAD:
                addcmdMessage("Error: ERROR_SERVICE_NO_THREAD");
                break;
            case ERROR_SERVICE_REQUEST_TIMEOUT:
                addcmdMessage("Error: ERROR_SERVICE_REQUEST_TIMEOUT");
                break;
            default:
                addcmdMessage("Unknown error:");
                printf(" %d", err);
                break;
        }
        return -1;
    }
    //addLogMessage("Start service");
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return 0;
}

int stopAupService(){
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if(!hSCManager){
        addcmdMessage("Error: Can't open Service Control Manager");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, service_name, SERVICE_STOP);

    ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return 0;
}

int setEnableAutoStart(bool flag){
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if(!hSCManager){
        addcmdMessage("Error: Can't open Service Control Manager");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, service_name, SERVICE_CHANGE_CONFIG);
    if(flag){
        ChangeServiceConfig(
                    hService,
                    SERVICE_NO_CHANGE,
                    SERVICE_AUTO_START,
                    SERVICE_NO_CHANGE,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                    );
    }else{
        ChangeServiceConfig(
                    hService,
                    SERVICE_NO_CHANGE,
                    SERVICE_DEMAND_START,
                    SERVICE_NO_CHANGE,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                    );
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return 0;
}

int main(int argc, _TCHAR* argv[])
{
    GetModuleFileName(NULL, servicePath, MAX_PATH);

    if(argc - 1 == 0) {
        SERVICE_TABLE_ENTRY ServiceTable[1];
        ServiceTable[0].lpServiceName = service_name;
        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)serviceMain;

        if(!StartServiceCtrlDispatcher(ServiceTable)) {
            addcmdMessage("Error: StartServiceCtrlDispatcher");
            int err = GetLastError();
            switch(err){
            case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
                addcmdMessage("ERROR_FAILED_SERVICE_CONTROLLER_CONNECT");
                break;
            case ERROR_INVALID_DATA:
                addcmdMessage("ERROR_INVALID_DATA");
                break;
            case ERROR_SERVICE_ALREADY_RUNNING:
                addcmdMessage("ERROR_SERVICE_ALREADY_RUNNING");
                break;
            default:
                addcmdMessage("Undefined error");
                break;
            }
        }
    }else if(_tcscmp(argv[argc-1], _T("--install")) == 0){
        installService();
    }else if(_tcscmp(argv[argc-1], _T("--uninstall")) == 0){
        removeService();
    }else if(_tcscmp(argv[argc-1], _T("--start")) == 0){
        startAupService();
    }else if(_tcscmp(argv[argc-1], _T("--installstart")) == 0){
        installService();
        startAupService();
    }else if(_tcscmp(argv[argc-1], _T("--stop")) == 0){
        stopAupService();
    }else if(_tcscmp(argv[argc-1], _T("--autostart")) == 0){
        setEnableAutoStart(true);
    }else if(_tcscmp(argv[argc-1], _T("--noautostart")) == 0){
        setEnableAutoStart(false);
    }
}
