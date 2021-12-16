//
//  IntelBTPatcher.h
//  IntelBTPatcher
//
//  Created by qcwap on 2021/2/8.
//

#ifndef IntelBTPatcher_h
#define IntelBTPatcher_h

#include <Headers/kern_patcher.hpp>

#define DRV_NAME "IntelBTPatcher"

class BluetoothDeviceAddress;

class CIntelBTPatcher {
public:
    bool init();
    void free();
    
    void processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
    
    static UInt64 newProcessEventDataWL(void *that, unsigned char *, unsigned int, unsigned int);
    mach_vm_address_t oldProcessEventDataWL {};
    
    static bool newGetCompleteCodeForCommand(void *that, unsigned short opcode, unsigned char *completecode);

    static IOReturn newGetOpCodeAndEventCode(void *that, unsigned char *a1, unsigned int a2, unsigned short *a3, unsigned char *a4, unsigned char *a5, unsigned char *a6, BluetoothDeviceAddress *a7, unsigned short *a8, bool *a9);
    
    static IOReturn newHCIRequestComplete(void *that);
    
    static IOReturn newFindQueueRequest(void *that, unsigned short arg1, void *addr, unsigned short arg2, bool arg3, void **hciRequestPtr);
    
    mach_vm_address_t oldGetCompleteCodeForCommand {};
    mach_vm_address_t oldGetOpCodeAndEventCode {};
    mach_vm_address_t parentIOBluetoothGetCompleteCodeForCommand {};
    mach_vm_address_t parentIOBluetoothGetOpCodeAndEventCode {};
    mach_vm_address_t funPrintAllHCIRequest {};
    mach_vm_address_t oldHCIRequestComplete {};
    mach_vm_address_t oldFindQueueRequest {};
};

#endif /* IntelBTPatcher_h */
