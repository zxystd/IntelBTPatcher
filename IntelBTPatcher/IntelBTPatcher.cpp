//
//  IntelBTPatcher.cpp
//  IntelBTPatcher
//
//  Created by qcwap on 2021/2/8.
//

#include <Headers/kern_api.hpp>
#include <Headers/kern_util.hpp>
#include <Headers/plugin_start.hpp>

#include "IntelBTPatcher.hpp"

static CIntelBTPatcher ibtPatcher;
static CIntelBTPatcher *callbackIBTPatcher = nullptr;

static const char *bootargOff[] {
    "-ibtcompatoff"
};

static const char *bootargDebug[] {
    "-ibtcompatdbg"
};

static const char *bootargBeta[] {
    "-ibtcompatbeta"
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal | LiluAPI::AllowInstallerRecovery | LiluAPI::AllowSafeMode,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::MountainLion,
    KernelVersion::BigSur,
    []() {
        ibtPatcher.init();
    }
};

static const char *IntelBTPatcher_IOBluetoothFamily[] { "/System/Library/Extensions/IOBluetoothFamily.kext/Contents/MacOS/IOBluetoothFamily" };

static KernelPatcher::KextInfo IntelBTPatcher_IOBluetoothInfo {
    "com.apple.iokit.IOBluetoothFamily",
    IntelBTPatcher_IOBluetoothFamily,
    1,
    {true, true},
    {},
    KernelPatcher::KextInfo::Unloaded
};

bool CIntelBTPatcher::init()
{
    DBGLOG(DRV_NAME, "%s", __PRETTY_FUNCTION__);
    callbackIBTPatcher = this;
    lilu.onKextLoadForce(&IntelBTPatcher_IOBluetoothInfo, 1,
    [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
        callbackIBTPatcher->processKext(patcher, index, address, size);
    }, this);
    return true;
}

void CIntelBTPatcher::free()
{
    DBGLOG(DRV_NAME, "%s", __PRETTY_FUNCTION__);
}

void CIntelBTPatcher::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size)
{
    DBGLOG(DRV_NAME, "%s", __PRETTY_FUNCTION__);
    if (IntelBTPatcher_IOBluetoothInfo.loadIndex == index) {
        DBGLOG(DRV_NAME, "%s", IntelBTPatcher_IOBluetoothInfo.id);

        KernelPatcher::RouteRequest processEventDataWLrequest {
                "__ZN25IOBluetoothHostController18ProcessEventDataWLEPhjj",
            newProcessEventDataWL,
            oldProcessEventDataWL
        };
        patcher.routeMultiple(index, &processEventDataWLrequest, 1, address, size);
        if (patcher.getError() == KernelPatcher::Error::NoError) {
            DBGLOG(DRV_NAME, "routed %s", processEventDataWLrequest.symbol);
        } else {
            SYSLOG(DRV_NAME, "failed to resolve %s, error = %d", processEventDataWLrequest.symbol, patcher.getError());
            patcher.clearError();
        }
        
        KernelPatcher::RouteRequest getCompleteCodeCommandrequest {
                "__ZN31BroadcomBluetoothHostController25GetCompleteCodeForCommandEtPh",
            newGetCompleteCodeForCommand,
            oldGetCompleteCodeForCommand
        };
        patcher.routeMultiple(index, &getCompleteCodeCommandrequest, 1, address, size);
        if (patcher.getError() == KernelPatcher::Error::NoError) {
            DBGLOG(DRV_NAME, "routed %s", getCompleteCodeCommandrequest.symbol);
        } else {
            SYSLOG(DRV_NAME, "failed to resolve %s, error = %d", getCompleteCodeCommandrequest.symbol, patcher.getError());
            patcher.clearError();
        }
        
        KernelPatcher::RouteRequest getOpCodeAndEventCodeRequest {
            "__ZN31BroadcomBluetoothHostController21GetOpCodeAndEventCodeEPhjPtS0_S0_S0_P22BluetoothDeviceAddressS1_Pb",
            newGetOpCodeAndEventCode,
            oldGetOpCodeAndEventCode
        };
        patcher.routeMultiple(index, &getOpCodeAndEventCodeRequest, 1, address, size);
        if (patcher.getError() == KernelPatcher::Error::NoError) {
            DBGLOG(DRV_NAME, "routed %s", getOpCodeAndEventCodeRequest.symbol);
        } else {
            SYSLOG(DRV_NAME, "failed to resolve %s, error = %d", getOpCodeAndEventCodeRequest.symbol, patcher.getError());
            patcher.clearError();
        }
        
        KernelPatcher::RouteRequest hciRequestCompleteRequest {
            "__ZN21IOBluetoothHCIRequest8CompleteEv",
            newHCIRequestComplete,
            oldHCIRequestComplete
        };
        patcher.routeMultiple(index, &hciRequestCompleteRequest, 1, address, size);
        if (patcher.getError() == KernelPatcher::Error::NoError) {
            DBGLOG(DRV_NAME, "routed %s", hciRequestCompleteRequest.symbol);
        } else {
            SYSLOG(DRV_NAME, "failed to resolve %s, error = %d", hciRequestCompleteRequest.symbol, patcher.getError());
            patcher.clearError();
        }
        
        KernelPatcher::RouteRequest findQueueRequestRequest {
        "__ZN25IOBluetoothHostController17FindQueuedRequestEtP22BluetoothDeviceAddresstbPP21IOBluetoothHCIRequest",
            newFindQueueRequest,
            oldFindQueueRequest
        };
        patcher.routeMultiple(index, &findQueueRequestRequest, 1, address, size);
        if (patcher.getError() == KernelPatcher::Error::NoError) {
            DBGLOG(DRV_NAME, "routed %s", findQueueRequestRequest.symbol);
        } else {
            SYSLOG(DRV_NAME, "failed to resolve %s, error = %d", findQueueRequestRequest.symbol, patcher.getError());
            patcher.clearError();
        }
        
        parentIOBluetoothGetCompleteCodeForCommand = patcher.solveSymbol(index, "__ZN25IOBluetoothHostController25GetCompleteCodeForCommandEtPh");
        parentIOBluetoothGetOpCodeAndEventCode = patcher.solveSymbol(index, "__ZN25IOBluetoothHostController21GetOpCodeAndEventCodeEPhjPtS0_S0_S0_P22BluetoothDeviceAddressS1_Pb");
        funPrintAllHCIRequest = patcher.solveSymbol(index, "__ZN25IOBluetoothHostController19PrintAllHCIRequestsEv");
        
    }
}

typedef void (*FuncPrintAllHCIRequest)(uint64_t *);

static int getHCIRequestCommandOpCode(uint64_t *hciRequestPtr)
{
    if (*(uint64_t *)((uint8_t *)hciRequestPtr + 0xA98))
        return *(uint16_t *)((uint8_t *)hciRequestPtr + 0x894);
    return 0;
}

IOReturn CIntelBTPatcher::newHCIRequestComplete(void *that)
{
    IOReturn ret = FunctionCast(newHCIRequestComplete, callbackIBTPatcher->oldHCIRequestComplete)(that);
    DBGLOG(DRV_NAME, "%s opcode: 0x%04x Completed", __FUNCTION__, getHCIRequestCommandOpCode((uint64_t *)that));
    return ret;
}

IOReturn CIntelBTPatcher::newFindQueueRequest(void *that, unsigned short arg1, void *addr, unsigned short arg2, bool arg3, void **hciRequestPtr)
{
    IOReturn ret = FunctionCast(newFindQueueRequest, callbackIBTPatcher->oldFindQueueRequest)(that, arg1, addr, arg2, arg3, hciRequestPtr);
    if (ret != 0 && arg1 == 0x2019)
        ret = FunctionCast(newFindQueueRequest, callbackIBTPatcher->oldFindQueueRequest)(that, arg1, addr, 0xffff, arg3, hciRequestPtr);
    DBGLOG(DRV_NAME, "%s ret: %d arg1: 0x%04x arg2: 0x%04x arg3: %d ptr: %p", __FUNCTION__, ret, arg1, arg2, arg3, *hciRequestPtr);
    return ret;
}

UInt64 CIntelBTPatcher::newProcessEventDataWL(void *that, unsigned char *data, unsigned int len, unsigned int unk)
{
    int i = 0;
    DBGLOG(DRV_NAME, "%s ===begin===", __FUNCTION__);
    
    uint64_t *hciRequestPtr = *(uint64_t **)((uint8_t *)that + 0xD8);
    
    while (hciRequestPtr) {
        DBGLOG(DRV_NAME, "%s on queue %d: 0x%04x", __FUNCTION__, i, getHCIRequestCommandOpCode(hciRequestPtr));
        hciRequestPtr = *(uint64_t **)((uint8_t *)hciRequestPtr + 0x870);
        i++;
    }
    
    DBGLOG(DRV_NAME, "%s ===end===", __FUNCTION__);
    
    UInt64 oriResult = FunctionCast(newProcessEventDataWL, callbackIBTPatcher->oldProcessEventDataWL)(that, data, len, unk);
    DBGLOG(DRV_NAME, "%s result=%ld", __FUNCTION__, oriResult);
    
    return oriResult;
}

bool CIntelBTPatcher::newGetCompleteCodeForCommand(void *that, unsigned short opcode, unsigned char *completecode)
{
    bool ret = FunctionCast(newGetCompleteCodeForCommand, callbackIBTPatcher->oldGetCompleteCodeForCommand)(that, opcode, completecode);
    DBGLOG(DRV_NAME, "%s opcode: 0x%04x ret: %d completecode: 0x%x", __FUNCTION__, opcode, ret, completecode ? *completecode : -1);
    return ret;
}

IOReturn CIntelBTPatcher::newGetOpCodeAndEventCode(void *that, unsigned char *a1, unsigned int a2, unsigned short *a3, unsigned char *a4, unsigned char *a5, unsigned char *a6, BluetoothDeviceAddress *a7, unsigned short *a8, bool *a9)
{
    return FunctionCast(newGetOpCodeAndEventCode, callbackIBTPatcher->oldGetOpCodeAndEventCode)(that, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}
