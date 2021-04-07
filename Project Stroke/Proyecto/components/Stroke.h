// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

#include "HamsterStateMachine.h"
#include "Ability.h"
//#include "Transform.h"

class Stroke: public Component {
public:
	Stroke() :
			hms_(nullptr), ab_(nullptr) {
	}
	virtual ~Stroke() {
	}
	void init() override;

	void update() override;

	void increaseChance(int n, bool fromAbility);

	void decreaseChance();

	void infarctHamster();

	//DEBUG ------ INFARTA AL HAMSTER
	void infarct() { chance_ = 100; };

private:
	HamsterStateMachine* hms_;
	Ability* ab_;
	Transform* tr_;

	const int UPDATETIME = 5000, // Tiempo entre comprobaciones de infarto
		MAXAB = 65, // M�ximo de probabilidad de infarto que se puede recibir por habilidades
		MAXCHANCE = 35, // M�ximo de probabilidad de infarto que se puede recibir por salto y ataque
		TIMETODECREASE = 10000, // Tiempo que debe pasar para empezar a reducir el infarto
		TIMEBETWEENDECREASES = 2500, // Tiempo entre reducciones de la probabilidad
		DECREASEPERCENTAGE = 35; // Porcentaje que se reduce por acierto en el minijuego

	// Probabilidad de que haya un infarto
	int chance_ = 1,
		chanceFromAb_ = 0,
		// Tiempo de la �ltima comprobaci�n
		timeLastUpdate_ = 0,
		// Tiempo del �ltimo aumento
		timeLastIncrease_ = 0,
		// Tiempo de la �ltima reducci�n
		timeLastDecrease_ = 0;
	// Generador de n�meros aleatorios
	RandomNumberGenerator& r_ = sdlutils().rand();
};

