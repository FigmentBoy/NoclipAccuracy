#pragma once
#include "cocos2d.h"
using namespace cocos2d;
namespace PlayLayer {
	extern bool wouldDie;
	extern float prevX;
	extern int deaths;
	extern int frames;
	extern float totalDelta;

	inline void(__thiscall* update)(cocos2d::CCLayer* self, float delta);
	void __fastcall hkUpdate(cocos2d::CCLayer* self, void*, float delta);

	inline int(__thiscall* resetLevel)(void* self);
	int __fastcall hkResetLevel(void* self);

	inline bool(__thiscall* levelComplete)(void* self);
	bool __fastcall hkLevelComplete(void* self);

	inline int(__thiscall* death)(void* self, void* go, void* powerrangers);
	int __fastcall hkDeath(void* self, void*, void* go, void* powerrangers);

	inline bool(__thiscall* init)(CCLayer* self, void* GJGameLevel);
	bool __fastcall initHook(CCLayer* self, int edx, void* GJGameLevel);

	void mem_init();
}