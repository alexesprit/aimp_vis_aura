#include "stdafx.h"

#include "AuraVisualization.h"
#include "PluginData.h"

#define RADIANS(x) ((x) * 180 / M_PI)

AuraVisualization::AuraVisualization(IAIMPCore* core) {
    aimpCore = core;
    aimpCore->AddRef();
}

AuraVisualization::~AuraVisualization() {
    aimpCore->Release();
    aimpCore = nullptr;
}

void WINAPI AuraVisualization::Initialize(int width, int height) {
    Resize(width, height);
    InitializePaintTools(0xFFFFFFFF);
}

void WINAPI AuraVisualization::Finalize() {
    FinalizePaintTools();
}

int WINAPI AuraVisualization::GetFlags() {
    return AIMP_VISUAL_FLAGS_RQD_DATA_WAVE | 
           AIMP_VISUAL_FLAGS_RQD_DATA_SPECTRUM;
}

HRESULT WINAPI AuraVisualization::GetMaxDisplaySize(int *width, int *height) {
    *width = MAX_WIDTH;
    *height = MAX_HEIGTH;
    return S_OK;
}

HRESULT WINAPI AuraVisualization::GetName(IAIMPString **out) {
    aimpCore->CreateObject(IID_IAIMPString, (void**)out);
    (*out)->SetData(VISUALIZATION_NAME, wcslen(VISUALIZATION_NAME));
    return S_OK;
}

void WINAPI AuraVisualization::Click(int x, int y, int button) {
    // do nothing
}

void WINAPI AuraVisualization::Draw(HDC hdc, PAIMPVisualData data) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    graphics.FillRectangle(bgndBrush, 0, 0, visSize.cx, visSize.cy);

    graphics.TranslateTransform(static_cast<float>(visCenter.x), 
                                static_cast<float>(visCenter.y));

    // Draw inner circle
    int rad = innerRadius;
    for (int i = 0; i < SPECTRUM_ANALYZE_COUNT; ++i) {
        // Detecting beats
        // TODO improve check
        float v = data->Spectrum[2][i] / float(MAXCHAR);
        if (v > BEATS_THRESOLD) {
            rad = static_cast<int>((CIRCLE_SCALE_FACTOR + v * CIRCLE_BEATS_FACTOR) * rad);
            break;
        }
    }
    graphics.FillEllipse(circleBrush, -rad, -rad, 2 * rad, 2 * rad);

    float const_var = static_cast<float>(2 * M_PI / AIMP_VISUAL_WAVEFORM_MAX);

    for (float power = 0.5; power < 2.0; power += 0.5) {
        // TODO deal with all this crap
        int powerColor = color;
        if (power == 1.5) {
            powerColor = TransformColor(color, DIM_MAX_WAVE);
        }
        else if (power == 0.5) {
            powerColor = TransformColor(color, DIM_MIN_WAVE);
        }

        dotBrush->SetColor(Color(powerColor));
        wavePen->SetColor(Color(powerColor));

        // TODO change to average value of L and R channels
        // TODO think about drawing separate visualizations
        // for L and R channels if screen width is big enough.

        // Draw lines around the circle
        for (int i = 0; i < AIMP_VISUAL_WAVEFORM_MAX; i += lineStep) {
            float v = data->WaveForm[0][i] / float(MAXSHORT) * power;

            float x = outerRadius * cos(i * const_var);
            float y = outerRadius * sin(i * const_var);
            float x2 = (outerRadius + v * waveAmplitude) * cos(i * const_var);
            float y2 = (outerRadius + v * waveAmplitude) * sin(i * const_var);

            // draw line only one time with power 1.0
            if (power == 1.0) {
                graphics.DrawLine(wavePen, x, y, x2, y2);
            }

            // Draw dots at line vertex (diam = 3)
            graphics.FillEllipse(dotBrush, x2 - 1, y2 - 1, 3.0f, 3.0f);
        }

        // Draw lines between vertexes
        for (int i = 0; i < AIMP_VISUAL_WAVEFORM_MAX; i += waveStep) {
            int j = i + waveStep;
            if (j >= AIMP_VISUAL_WAVEFORM_MAX) {
                j -= AIMP_VISUAL_WAVEFORM_MAX;
            }

            float v1 = data->WaveForm[0][i] / float(MAXSHORT) * power;
            float v2 = data->WaveForm[0][j] / float(MAXSHORT) * power;

            float x1 = (outerRadius + v1 * waveAmplitude)*cos(i * const_var);
            float y1 = (outerRadius + v1 * waveAmplitude)*sin(i * const_var);

            float x2 = (outerRadius + v2 * waveAmplitude)*cos(j * const_var);
            float y2 = (outerRadius + v2 * waveAmplitude)*sin(j * const_var);

            graphics.DrawLine(wavePen, x1, y1, x2, y2);
        }
    }
}

void WINAPI AuraVisualization::Resize(int newWidth, int newHeight) {
    visSize.cx = newWidth;
    visSize.cy = newHeight;

    visCenter.x = newWidth / 2;
    visCenter.y = newHeight / 2;

    int minDimension = min(newWidth, newHeight);

    outerRadius = minDimension * OUTER_RADIUS_PERCENT / 100;
    innerRadius = minDimension * INNER_RADIUS_PERCENT / 100;

    waveAmplitude = minDimension * WAVE_AMPLITUDE_PERCENT / 100;

    waveStep = K_WAVE_STEP / minDimension;
    lineStep = K_LINE_STEP / minDimension;
}

void AuraVisualization::InitializePaintTools(DWORD color) {
    this->color = color;

    bgndBrush = new SolidBrush(Color(TransformColor(color, DIM_BACKGROUND)));
    circleBrush = new SolidBrush(Color(TransformColor(color, DIM_CIRCLE)));

    // Color is changed dynamically
    dotBrush = new SolidBrush(Color(color));
    wavePen = new Pen(Color(color), 1.0f);
}

DWORD AuraVisualization::TransformColor(DWORD color, short light) {
    int a = ((color & 0xFF000000) >> 24);
    int r = ((color & 0xFF0000) >> 16) + light;
    int g = ((color & 0xFF00) >> 8) + light;
    int b = (color & 0xFF) + light;

    b = min(max(0, b), 0xFF);
    g = min(max(0, g), 0xFF);
    r = min(max(0, r), 0xFF);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

void AuraVisualization::FinalizePaintTools() {
    delete wavePen;

    delete dotBrush;
    delete bgndBrush;
    delete circleBrush;
}