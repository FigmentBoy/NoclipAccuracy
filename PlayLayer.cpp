#include "PlayLayer.h"
#include "GameManager.h"
#include "MinHook.h"
bool PlayLayer::wouldDie = false;
int PlayLayer::frames = 0;
int PlayLayer::deaths = 0;
float PlayLayer::totalDelta = 0;
float PlayLayer::prevX = 0;

void WriteBytes(void* location, std::vector<BYTE> bytes) {
	DWORD old_prot;
	VirtualProtect(location, bytes.size(), PAGE_EXECUTE_READWRITE, &old_prot);

	memcpy(location, bytes.data(), bytes.size());

	VirtualProtect(location, bytes.size(), old_prot, &old_prot);
}


bool __fastcall PlayLayer::initHook(CCLayer* self, int edx, void* GJGameLevel) {
	size_t base = (size_t)GetModuleHandle(0);

	prevX = 0;

	if (GameManager::getGameVariable(*(void**)(base + 0x3222d0), "noclip")) {
		std::string text = std::string("Accuracy: 100%");
		CCLabelBMFont* textObj = CCLabelBMFont::create(text.c_str(), "Resources\\goldFont-uhd.fnt");

		textObj->setZOrder(1000);
		textObj->setTag(100000);

		textObj->setScale(0.5);
		auto size = textObj->getScaledContentSize();

		textObj->setPositionX(size.width);
		textObj->setPositionY(size.height);

		self->addChild(textObj);
	}
	return init(self, GJGameLevel);
}

void __fastcall PlayLayer::hkUpdate(cocos2d::CCLayer* self, void* edx, float delta) {
	void* player1 = *(void**)((char*)self + 0x224);
	float x = *(float*)((size_t)player1 + 0x67c);

	if (x != prevX) {
		frames += 1;
		totalDelta += delta;
	}

	size_t base = (size_t)GetModuleHandle(0);

	if (GameManager::getGameVariable(*(void**)(base + 0x3222d0), "noclip")) {
		std::string text = std::string("Accuracy: ") + std::to_string(100 * ((float)(frames - deaths) / (float)frames)) + "%";
		if (frames == 0) {
			text = std::string("Accuracy: 100%");
		}

		CCLabelBMFont* textObj = (CCLabelBMFont*)self->getChildByTag(100000);
		textObj->setCString(text.c_str());
	}


	if (wouldDie) {
		wouldDie = false;
		if (totalDelta >= 0.1) {
			deaths += 1;
		}
	}

	prevX = x;
	return update(self, delta);
}

int __fastcall PlayLayer::hkResetLevel(void* self) {
	int res = resetLevel(self);
	void* player1 = *(void**)((char*)self + 0x224);
	prevX = *(float*)((size_t)player1 + 0x67c);
	frames = 0;
	totalDelta = 0;
	deaths = 0;
	wouldDie = false;

	return res;
}



bool __fastcall PlayLayer::hkLevelComplete(void* self) {
	std::string text = std::string("Accuracy: ") + std::to_string(100*((float)(frames - deaths) / (float)frames)) + "%";
	CCLabelBMFont* textObj = CCLabelBMFont::create(text.c_str(), "Resources\\goldFont-uhd.fnt");

	textObj->setZOrder(1000);
	textObj->setTag(100000);
	
	auto window = CCDirector::sharedDirector()->getWinSize();

	textObj->setPositionX(window.width / 2);
	textObj->setPositionY(window.height - 40);

	size_t base = (size_t)GetModuleHandle(0);

	

	return PlayLayer::levelComplete(self);
}

int __fastcall PlayLayer::hkDeath(void* self, void*, void* go, void* powerrangers) {
	size_t base = (size_t)GetModuleHandle(0);

	if (GameManager::getGameVariable(*(void**)(base + 0x3222d0), "noclip")) {

		uintptr_t address = base + 0x20A8Ba;

		wouldDie = true;
		__asm {
			mov eax, address
			jmp eax
		}
	}
	else {
		return death(self, go, powerrangers);
	}
	
}

void PlayLayer::mem_init() {
	size_t base = (size_t)GetModuleHandle(0);

	MH_CreateHook(
		(PVOID)(base + 0x01FB780),
		PlayLayer::initHook,
		(LPVOID*)&PlayLayer::init);

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