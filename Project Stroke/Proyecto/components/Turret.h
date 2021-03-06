#pragma once

#include "Ability.h"
#include "EntityAttribs.h"

class Turret : public Ability {
public:
	Turret();

	virtual ~Turret();

	void update() override;

	//Ejecuta la habilidad en funcion del hamster que sea
	void action() override;
	void endAbility() override;

private:
	float cadenceTime_;
	float x_;

	EntityAttribs* attribs_;
	//CONSTANTES
	const int CADENCESHOT = 200;
	const int BULLETSPEED = 25;
	const int CD = 2000;

	//Con esto movemos la bala a la Cara de Monchi
	const int OFFSETX = 20;
	const int OFFSETY = 30;
};
