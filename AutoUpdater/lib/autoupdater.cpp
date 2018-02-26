#include "autoupdater.h"

AupInitStatus aup_init(aup_callback_fn callback)
{
    callback();
    return AupRequireAppExit;
}

//AutoUpdater::AutoUpdater()
//{

//}
