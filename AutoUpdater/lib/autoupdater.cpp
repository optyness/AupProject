#include "autoupdater.h"

AupInitStatus aup_init(aup_callback_fn callback, void* context)
{
    callback(context);
    return AupRequireAppExit;
}
