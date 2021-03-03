// dllmain.cpp : Defines the entry point for the DLL application.
#include <cocos2d.h>
#include "MinHook.h"
#include "PlayLayer.h"
#include "GameManager.h"
#include "MenuOptions.h"

DWORD WINAPI my_thread(void* hModule) {
    /*AllocConsole();
    SetConsoleTitleA("zBot Console");
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);*/

    MH_Initialize();
    PlayLayer::mem_init();
    GameManager::mem_init();
    MenuOptions::mem_init();
    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

