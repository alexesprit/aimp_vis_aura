#pragma once

#include "apiCore.h"
#include "apiVisuals.h"
#include "IUnknownImpl.h"

class AuraVisualization : 
    public IUnknownImpl<IAIMPExtensionEmbeddedVisualization> {
public:
    AuraVisualization(IAIMPCore* core);
    ~AuraVisualization();

    virtual void WINAPI Initialize(int width, int height) override;
    virtual void WINAPI Finalize() override;

    virtual int WINAPI GetFlags() override;
    virtual HRESULT WINAPI GetMaxDisplaySize(int *width, int *height) override;
    virtual HRESULT WINAPI GetName(IAIMPString **out) override;

    virtual void WINAPI Click(int x, int y, int button) override;
    virtual void WINAPI Draw(HDC dc, PAIMPVisualData data) override;
    virtual void WINAPI Resize(int newWidth, int newHeight) override;

    void OnColorSchemeChanged(DWORD color);
private:
    void InitializePaintTools();
    void FinalizePaintTools();
   
    DWORD TransformColor(DWORD color, short light);

    Pen* wavePen;
    SolidBrush* dotBrush;
    SolidBrush* bgndBrush;
    SolidBrush* circleBrush;

    DWORD visColor;
    const short DIM_MIN_WAVE = -128;
    const short DIM_MAX_WAVE = -64;
    const short DIM_BACKGROUND = -192;
    const short DIM_CIRCLE = -96;

    IAIMPCore* aimpCore;

    // Okey, Full HD
    const int MAX_WIDTH = 1920;
    const int MAX_HEIGTH = 1080;

    const float BEATS_THRESOLD = 0.25f;
    const short SPECTRUM_ANALYZE_COUNT = 4;

    const float CIRCLE_BEATS_FACTOR = 0.15f;
    const float CIRCLE_SCALE_FACTOR = 1.05f;

    // step minDimension / k + b
    const short K_LINE_STEP = -100;
    const short K_WAVE_STEP = -200;
    const short B_LINE_STEP = 12;
    const short B_WAVE_STEP = 24;
    short lineStep;
    short waveStep;

    // radius = minDimension * percent / 100
    const USHORT OUTER_RADIUS_PERCENT = 40;
    const USHORT INNER_RADIUS_PERCENT = 20;
    const USHORT WAVE_AMPLITUDE_PERCENT = 20;

    float CONST_VALUE = static_cast<float>(2 * M_PI / AIMP_VISUAL_WAVEFORM_MAX);

    SIZE visSize;
    POINT visCenter;
    short outerRadius;
    short innerRadius;
    short waveAmplitude;
};

