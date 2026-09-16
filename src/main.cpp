#include <windows.h>
#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "system_timer.hpp"
#include "process_manager.hpp"

// Datos de DirectX
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string res(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &res[0], size, NULL, NULL);
    return res;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. Activar optimizaciones de backend al iniciar
    SystemTimer timer;
    timer.SetResolution(1);
    ProcessManager::ForceHighPerformancePower();
    ProcessManager::PurgeSystemMemory();

    // 2. Registrar clase de ventana de Windows
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"LatencyXUI", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"LatencyX - Game Optimizer", WS_OVERLAPPEDWINDOW, 100, 100, 800, 500, NULL, NULL, wc.hInstance, NULL);

    // 3. Inicializar Direct3D
    if (!CreateDeviceD3D(hwnd)) {
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // 4. Inicializar contexto de Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Estilo visual oscuro profesional
    ImGui::StyleColorsDark();

    // Inicializar Backends de Win32 y DX11
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Variables de estado para la interfaz
    std::vector<ProcessEntry> processList = ProcessManager::GetRunningProcesses();
    int selectedProcessIndex = 0;
    char searchFilter[128] = "";
    std::string statusMessage = "Sistema optimizado y listo.";

    // 5. Bucle principal de la aplicación
    bool done = false;
    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        // Iniciar el frame de ImGui
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Ventana principal de LatencyX
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
        ImGui::Begin("LatencyX - Dashboard de Rendimiento", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        ImGui::Text("Estado del Sistema: Activo (Timer 1ms | RAM Optimizada)");
        ImGui::Separator();

        // Botón para refrescar la lista de procesos activos
        if (ImGui::Button("Escanear Procesos Activos")) {
            processList = ProcessManager::GetRunningProcesses();
            statusMessage = "Lista de procesos actualizada.";
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(250);
        ImGui::InputText("Filtrar", searchFilter, IM_ARRAYSIZE(searchFilter));

        // Filtrar procesos en tiempo real
        std::vector<ProcessEntry> filteredList;
        for (const auto& p : processList) {
            std::string nameStr = WStringToString(p.name);
            std::string lowerName = nameStr, lowerQuery = searchFilter;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

            if (lowerQuery.empty() || lowerName.find(lowerQuery) != std::string::npos) {
                filteredList.push_back(p);
            }
        }

        ImGui::Text("Selecciona el juego o proceso a optimizar:");
        
        // Listbox con los procesos filtrados
        if (ImGui::BeginListBox("##ProcessList", ImVec2(-FLT_MIN, 250))) {
            for (size_t i = 0; i < filteredList.size(); i++) {
                std::string displayLabel = WStringToString(filteredList[i].name) + " (PID: " + std::to_string(filteredList[i].pid) + ")";
                bool isSelected = (selectedProcessIndex == (int)i);
                if (ImGui::Selectable(displayLabel.c_str(), isSelected)) {
                    selectedProcessIndex = (int)i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        // Botón de Aplicar Optimización de Hardware
        if (ImGui::Button("¡Optimizar Proceso Seleccionado!", ImVec2(250, 40))) {
            if (!filteredList.empty() && selectedProcessIndex < (int)filteredList.size()) {
                DWORD targetPID = filteredList[selectedProcessIndex].pid;
                if (ProcessManager::OptimizeProcess(targetPID)) {
                    statusMessage = "¡Éxito! Prioridad alta y núcleos aislados aplicados al PID: " + std::to_string(targetPID);
                } else {
                    statusMessage = "Error: Ejecuta la aplicación como Administrador.";
                }
            }
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.4f, 1.0f), "Estado: %s", statusMessage.c_str());

        ImGui::End();

        // Renderizado
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.08f, 0.08f, 0.1f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // V-Sync activado
    }

    // Limpieza
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Funciones auxiliares de configuración de DirectX 11 y Win32
bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}