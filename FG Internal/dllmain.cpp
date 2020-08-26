#include <Windows.h>
#include <stdio.h>
#include <thread>

#include "memory.h"
#include "render.h"

HMODULE my_module;
void init() {

	render::load();

	while (true) {
		if (GetAsyncKeyState(VK_DELETE))
			break;
		if (GetAsyncKeyState(VK_END))
			exit(0);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20ms);
	}

	render::unload();
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ms);
	}
	FreeLibraryAndExitThread(my_module, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		my_module = hModule;
		if (hModule)
			DisableThreadLibraryCalls(hModule);
		auto h = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), hModule, 0, nullptr);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

