#ifndef AUTOUPDATER_H
#define AUTOUPDATER_H

#include <QtCore/QtGlobal>
//#include "auplib_Export.h"

#if defined(AUPLIB_LIBRARY)
#  define auplib_EXPORT Q_DECL_EXPORT
#else
#  define auplib_EXPORT Q_DECL_IMPORT
#endif

enum auplib_EXPORT AupInitStatus
    {
        AupOk,
        AupAlreadyInitialized,
        AupRequireAppExit,
    };

typedef auplib_EXPORT void (*aup_callback_fn)(void*);

extern "C" {
    auplib_EXPORT AupInitStatus aup_init(aup_callback_fn callback, void* context);
    auplib_EXPORT void aup_shutdown();
    auplib_EXPORT void aup_update_info(char *update_info);
}

#endif // AUTOUPDATER_H
