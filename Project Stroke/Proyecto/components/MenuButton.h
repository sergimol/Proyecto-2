#pragma once
// This file is part of the course TPV2@UCM - Samir Genaim

#include "../ecs/Component.h"
#include "../ecs/Entity.h"

#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "EntityAttribs.h"
#include "GameStates.h"


class MenuButton : public Component {
public:
	MenuButton(std::string n, Vector2D position, int stateNum);
	virtual ~MenuButton() {};

	void render() override;
	void init() override;

	void selected();
	void pressed();
	void exited();

	//Getters auxiliares
	//inline int getPosUI() { return position; };
	SDL_Rect getRect() const& { return dest_; };

	void setSelectable(bool s);
	bool selectable() { return selectable_; };

	string getName() { return buttonName_; };
private:
	//Nombre del personaje que contiene la UI, todo en minusculas
	std::string buttonName_;
	//Numero del boton en el menu
	int stateNumber_;

	//Texturas del boton
	Texture* button_;
	Texture* buttonEnter_;
	Texture* buttonPressed_;

	GameStates* state_;

	//DestRects
	SDL_Rect dest_;
	//Posiciones de los destRects
	Vector2D renderCoords_;

	bool buttonSelected_;
	bool selectable_ = true;
};
