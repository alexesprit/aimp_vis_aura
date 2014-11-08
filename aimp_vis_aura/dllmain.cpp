#include "stdafx.h"
#include "AuraVisualizationPlugin.h"

bool APIENTRY DllMain(HMODULE hModule, DWORD  dwReasonForCall, LPVOID lpReserved) {
    switch (dwReasonForCall) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return true;
}

bool WINAPI AIMPPluginGetHeader(IAIMPPlugin **header) {
    auto plugin = new AuraVisualizationPlugin();
    plugin->AddRef();
    *header = plugin;
    return true;
}
