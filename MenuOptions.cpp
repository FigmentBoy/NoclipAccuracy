#include "MenuOptions.h"
#include "MinHook.h"

bool __fastcall MenuOptions::initHook(void* self)
{
    bool result = MenuOptions::init(self);

    MenuOptions::addToggle(
        self,
        "Noclip Accuracy by Figment",
        "noclip",
        "Turns on noclip and informs you how accurate your noclip run is"
    );

    return result;
}

void MenuOptions::mem_init() {
    size_t base = reinterpret_cast<size_t>(GetModuleHandle(0));
    MH_CreateHook(
        (PVOID)(base + 0x1DE8F0),
        MenuOptions::initHook,
        (PVOID*)&MenuOptions::init);

    MenuOptions::addToggle = reinterpret_cast<decltype(MenuOptions::addToggle)>(base + 0x1DF6B0);
}