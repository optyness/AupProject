#include <iostream>
#include <Windows.h>
#include <serviceinit.h>

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;

#define service_name TEXT("AutoUpdateService")
TCHAR servicePath[MAX_PATH];

int addcmdMessage(const char* text)
{
    return printf(text);
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
    int error;

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

    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus (serviceStatusHandle, &serviceStatus);

    error = InitService();
    if (error){
        serviceStatus.dwCurrentState    = SERVICE_STOPPED;
        serviceStatus.dwWin32ExitCode   = -1;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
        return;
    }

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
    addcmdMessage("Success install service");
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
    addcmdMessage("Success remove service");
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
//        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();
        int err = GetLastError();
//        FormatMessage(
//                FORMAT_MESSAGE_ALLOCATE_BUFFER |
//                FORMAT_MESSAGE_FROM_SYSTEM |
//                FORMAT_MESSAGE_IGNORE_INSERTS,
//                NULL,
//                dw,
//                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//                (LPTSTR) &lpMsgBuf,
//                0, NULL );
//        addcmdMessage((LPSTR)lpMsgBuf);
//        std::cout << (LPCTSTR)lpMsgBuf;
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
        //LocalFree(lpMsgBuf);
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
                    NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }else{
        ChangeServiceConfig(
                    hService,
                    SERVICE_NO_CHANGE,
                    SERVICE_DEMAND_START,
                    SERVICE_NO_CHANGE,
                    NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    GetModuleFileName(NULL, servicePath, MAX_PATH);

    if(argc - 1 == 0) {
        SERVICE_TABLE_ENTRY ServiceTable[1];
        ServiceTable[0].lpServiceName = service_name;
        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)serviceMain;

        if(!StartServiceCtrlDispatcher(ServiceTable)){
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
    }else if(_tcscmp(argv[argc-1], _T("--help")) == 0){
        addcmdMessage("\n--help         - show command list"
                      "\n--install      - install service"
                      "\n--uninstall    - uninstall service"
                      "\n--start        - start service "
                      "\n--installstart - install and start service"
                      "\n--stop         - stop service"
                      "\n--autostart    - enable autostart"
                      "\n--noautostart  - disable autostart");
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
