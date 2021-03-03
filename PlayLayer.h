#pragma once
#include "cocos2d.h"

namespace PlayLayer {
	extern bool wouldDie;
	extern int deaths;
	extern int frames;

	inline void(__thiscall* update)(cocos2d::CCLayer* self, float delta);
	void __fastcall hkUpdate(cocos2d::CCLayer* self, void*, float delta);

	inline int(__thiscall* resetLevel)(void* self);
	int __fastcall hkResetLevel(void* self);

	inline bool(__thiscall* levelComplete)(void* self);
	bool __fastcall hkLevelComplete(void* self);

	inline int(__thiscall* death)(void* self, void* go);
	int __fastcall hkDeath(void* self, void* go);

	void mem_init();
}