#ifndef AUTOUPDATER_H
#define AUTOUPDATER_H

enum AupInitStatus
    {
        AupOk,
        AupAlreadyInitialized,
        AupRequireAppExit,
    };

typedef void (*aup_callback_fn)();

extern "C" {
    AupInitStatus aup_init(aup_callback_fn callback);
    void aup_shutdown();
    void aup_update_info(const char *update_info);
}

#endif // AUTOUPDATER_H