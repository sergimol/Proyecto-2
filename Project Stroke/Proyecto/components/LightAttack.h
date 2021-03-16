﻿#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../ecs/Manager.h"
#include "Life.h"
#include "Transform.h"
#include "HamsterStateMachine.h"

class LightAttack : public Component {
public:
	LightAttack(float dmg);

	virtual ~LightAttack() {}

	void init() override;
	void update() override;
	bool CheckCollisions(const SDL_Rect& rectPlayer);

	//Establece el daño del light attack
	inline void setDmg(float dmg) { dmg_ = dmg; }

private:
	Transform* tr_;
	HamsterStateMachine* hms_;
	short unsigned int w_, h_;
	long unsigned int cooldown_, time_;
	SoundEffect& attackSound_, &hitSound_;
	float dmg_ = 0;
}
;