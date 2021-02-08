//
//  IntelBTPatcher.c
//  IntelBTPatcher
//
//  Created by qcwap on 2021/2/8.
//

#include <mach/mach_types.h>

kern_return_t IntelBTPatcher_start(kmod_info_t * ki, void *d);
kern_return_t IntelBTPatcher_stop(kmod_info_t *ki, void *d);

kern_return_t IntelBTPatcher_start(kmod_info_t * ki, void *d)
{
    return KERN_SUCCESS;
}

kern_return_t IntelBTPatcher_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
