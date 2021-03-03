#include "PlayLayer.h"
#include "MinHook.h"
bool PlayLayer::wouldDie = false;
int PlayLayer::frames = 0;
int PlayLayer::deaths = 0;

void WriteBytes(void* location, std::vector<BYTE> bytes) {
	DWORD old_prot;
	VirtualProtect(location, bytes.size(), PAGE_EXECUTE_READWRITE, &old_prot);

	memcpy(location, bytes.data(), bytes.size());

	VirtualProtect(location, bytes.size(), old_prot, &old_prot);
}

using namespace cocos2d;

void __fastcall PlayLayer::hkUpdate(cocos2d::CCLayer* self, void* edx, float delta) {
	frames += 1;
	
	if (wouldDie) {
		wouldDie = false;
		deaths += 1;
	}

	return update(self, delta);
}

int __fastcall PlayLayer::hkResetLevel(void* self) {
	frames = 0;
	deaths = 0;
	wouldDie = false;

	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(100000);

	return resetLevel(self);
}



bool __fastcall PlayLayer::hkLevelComplete(void* self) {
	std::string text = std::string("Accuracy: ") + std::to_string(100*((float)(frames - deaths) / (float)frames)) + "%";
	CCLabelBMFont* textObj = CCLabelBMFont::create(text.c_str(), "Resources\\goldFont-uhd.fnt");

	textObj->setZOrder(1000);
	textObj->setTag(100000);
	
	auto window = CCDirector::sharedDirector()->getWinSize();

	textObj->setPositionX(window.width / 2);
	textObj->setPositionY(window.height - 40);
	CCDirector::sharedDirector()->getRunningScene()->addChild(textObj);

	return PlayLayer::levelComplete(self);
}

int __fastcall PlayLayer::hkDeath(void* self, void* go) {
	size_t base = (size_t)GetModuleHandle(0);

	uintptr_t address = base + 0x20A8Ba;

	wouldDie = true;
	__asm {
		mov eax,address
		jmp eax
	}
}

void PlayLayer::mem_init() {
	size_t base = (size_t)GetModuleHandle(0);

	MH_CreateHook(
		(PVOID)(base + 0x20A1A0),
		PlayLayer::hkDeath,
		(LPVOID*)&PlayLayer::death);

	MH_CreateHook(
		(PVOID)(base + 0x20BF00),
		PlayLayer::hkResetLevel,
		(LPVOID*)&PlayLayer::resetLevel);

	MH_CreateHook(
		(PVOID)(base + 0x2029C0),
		PlayLayer::hkUpdate,
		(LPVOID*)&PlayLayer::update);

	MH_CreateHook(
		(PVOID)(base + 0x1FD3D0),
		PlayLayer::hkLevelComplete,
		(LPVOID*)&PlayLayer::levelComplete);
}