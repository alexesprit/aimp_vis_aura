#include "stdafx.h"

#include "AuraVisualization.h"
#include "AuraVisualizationPlugin.h"
#include "PluginData.h"

HRESULT WINAPI AuraVisualizationPlugin::Initialize(IAIMPCore* core) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    auto visualization = new AuraVisualization(core);
    core->RegisterExtension(IID_IAIMPServiceVisualizations, visualization);

    return S_OK;
}

HRESULT WINAPI AuraVisualizationPlugin::Finalize() {
    GdiplusShutdown(gdiplusToken);
    return S_OK;
}

PWCHAR WINAPI AuraVisualizationPlugin::InfoGet(int index) {
    switch (index) {
        case AIMP_PLUGIN_INFO_NAME:
            return PLUGIN_NAME;
        case AIMP_PLUGIN_INFO_AUTHOR:
            return PLUGIN_AUTHOR;
        case AIMP_PLUGIN_INFO_SHORT_DESCRIPTION:
            return PLUGIN_SHORT_DESC;
        default:
            return nullptr;
    }
}

DWORD WINAPI AuraVisualizationPlugin::InfoGetCategories() {
    return AIMP_PLUGIN_CATEGORY_VISUALS;
}

void WINAPI AuraVisualizationPlugin::SystemNotification(int notifyId, IUnknown* data) {

}
