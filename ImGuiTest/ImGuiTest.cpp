#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"
#include "includes/gui.h"
#include <iostream>

int main()
{
	Gui gui = Gui();
	
	if (!gui.Initialize()) {
		std::cerr << "Failed to initialize GUI." << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ID3D11Device* device = gui.GetD3D11Device();
	ID3D11DeviceContext* context = gui.GetD3D11DeviceContext();
	ID3D11RenderTargetView* rtv = gui.GetRenderTargetView();
	IDXGISwapChain* swapChain = gui.GetSwapChain();
	ImGui_ImplWin32_Init(gui.GetHwnd());
	ImGui_ImplDX11_Init(device, context);

	while (true) {
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Hello, ImGui!");

			if (ImGui::Button("Click Me")) {
				std::cout << "Button Clicked!" << std::endl;
			}

			ImGui::End();
		}

		// Render
		context->OMSetRenderTargets(1, &rtv, nullptr);
		const float clearColor[4] = { 0.f, 0.f , 0.f, 1.f };
		context->ClearRenderTargetView(rtv, clearColor);
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swapChain->Present(0, 0);
		
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (!gui.Destroy()) {
		std::cerr << "Failed to destroy GUI." << std::endl;
		return -1;
	}
	return 0;
}
