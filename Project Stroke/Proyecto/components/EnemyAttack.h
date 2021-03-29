﻿#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../ecs/Manager.h"
#include "EntityAttribs.h"
#include "Transform.h"
#include "HamsterStateMachine.h"
#include "../game/Game.h"

class EnemyAttack : public Component {
public:
	EnemyAttack();

	virtual ~EnemyAttack() {}

	void init() override;
	void update() override;
	void render() override;

	bool CheckCollisions(const SDL_Rect& rectPlayer, bool finCombo);
	void LaunchAttack();

private:
	Transform* tr_;
	SDL_Rect attRect_;
	short unsigned int w_, h_;
	long unsigned int cooldown_, time_;
	SoundEffect& attackSound_, &hitSound_;
}
;