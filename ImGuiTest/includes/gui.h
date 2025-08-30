#pragma once
#include "backends/imgui_impl_win32.h"
#include <d3d11.h>
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Gui {
private:
	HWND hwnd = nullptr;
	HINSTANCE hInstance = nullptr;
	ID3D11Device* pD3D11Device = nullptr;
	ID3D11DeviceContext* pD3D11DeviceContext = nullptr;
	IDXGISwapChain* pSwaipChain = nullptr;
	ID3D11RenderTargetView* pD3D11RenderTargetView = nullptr;

	const wchar_t* windowTitle = L"ImGui Test";
	const wchar_t* className = L"ImGuiTestWindow";
	const int width = 640;
	const int height = 480;
	UINT resizeWidth = 0, resizeHeight = 0;

	bool InitWindow();
	bool CreateD3D11Device();
	bool CreateD3D11RenderTargetView();
	DXGI_SWAP_CHAIN_DESC CreateSwapChain();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Gui() = default;
	~Gui() = default;

	bool Initialize();
	bool Destroy();
	HWND GetHwnd() const { return this->hwnd; }
	ID3D11Device* GetD3D11Device() const { return this->pD3D11Device; }
	ID3D11DeviceContext* GetD3D11DeviceContext() const { return this->pD3D11DeviceContext; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return this->pD3D11RenderTargetView; }
	IDXGISwapChain* GetSwapChain() const { return this->pSwaipChain; }
};
