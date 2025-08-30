#include "includes/gui.h"

bool Gui::Initialize() {
	if (!this->InitWindow()) return false;
	return true;
}

bool Gui::InitWindow() {
	WNDCLASS wc = {};

	this->hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = Gui::WindowProc;
	wc.hInstance = this->hInstance;
	wc.lpszClassName = this->className;

	if (!RegisterClass(&wc)) return false;

	this->hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		this->windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		this->width,
		this->height,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if (this->hwnd == nullptr) {
		return false;
	}

	ShowWindow(this->hwnd, SW_SHOWDEFAULT);
	UpdateWindow(this->hwnd);

	if (!this->CreateD3D11Device()) return false;
	if (!this->CreateD3D11RenderTargetView()) return false;

	return true;
}

LRESULT CALLBACK Gui::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) return TRUE;
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;	
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

bool Gui::CreateD3D11Device() {
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = this->CreateSwapChain();

	return D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		deviceFlags,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&this->pSwaipChain,
		&this->pD3D11Device,
		nullptr,
		&this->pD3D11DeviceContext
	) == S_OK;
}

bool Gui::CreateD3D11RenderTargetView() {
	if (this->pSwaipChain == nullptr || this->pD3D11Device == nullptr) return false;

	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer;
	this->pSwaipChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	if (hr != S_OK) return false;

	hr = this->pD3D11Device->CreateRenderTargetView(pBackBuffer, nullptr, &this->pD3D11RenderTargetView);

	return hr == S_OK;
}

DXGI_SWAP_CHAIN_DESC Gui::CreateSwapChain() {
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE;
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = this->hwnd;

	return desc;
}

bool Gui::Destroy() {
	if (this->pD3D11RenderTargetView != nullptr) {
		this->pD3D11RenderTargetView->Release();
		this->pD3D11RenderTargetView = nullptr;
	}

	if (this->pSwaipChain != nullptr) {
		this->pSwaipChain->Release();
		this->pSwaipChain = nullptr;
	}

	if (this->pD3D11DeviceContext != nullptr) {
		this->pD3D11DeviceContext->Release();
		this->pD3D11DeviceContext = nullptr;
	}

	if (this->pD3D11Device != nullptr) {
		this->pD3D11Device->Release();
		this->pD3D11Device = nullptr;
	}

	if (this->hwnd != nullptr) {
		if (DestroyWindow(this->hwnd) == FALSE) return false;
		if (UnregisterClass(this->className, this->hInstance) == FALSE) return false;
		this->hwnd = nullptr;
		this->hInstance = nullptr;
	}

	return true;
}
