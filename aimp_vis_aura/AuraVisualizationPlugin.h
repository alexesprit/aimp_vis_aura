#pragma once

#include "apiPlugin.h"
#include "IUnknownImpl.h"

class AuraVisualizationPlugin : public IUnknownImpl<IAIMPPlugin> {
public:
    virtual HRESULT WINAPI Initialize(IAIMPCore* core) override;
    virtual HRESULT WINAPI Finalize() override;

    virtual PWCHAR WINAPI InfoGet(int infoIndex) override;
    virtual DWORD WINAPI InfoGetCategories() override;

    virtual void WINAPI SystemNotification(int notifyId, IUnknown* data) override;
};

