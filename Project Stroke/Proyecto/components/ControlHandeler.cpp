#include "ControlHandeler.h"
#include <cmath>
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
void ControlHandeler::init() {

	mov_ = entity_->getComponent<Movement>();
	assert(mov_ != nullptr);

	//En vez de construilo solamente deberia de tner que irse a donde estuviese guardado lso controles y coger el mapeado segun lo que le pida

	if (player_ == 1) {
		keymap.insert({ UP, SDL_SCANCODE_UP });
		keymap.insert({ DOWN, SDL_SCANCODE_DOWN });
		keymap.insert({ LEFT, SDL_SCANCODE_LEFT });
		keymap.insert({ RIGHT, SDL_SCANCODE_RIGHT });
		keymap.insert({ SPACE, SDL_SCANCODE_SPACE }); //el salto aun no esta includio
	}
	else if (player_ == 2) {
		keymap.insert({ UP, SDL_SCANCODE_W });
		keymap.insert({ DOWN, SDL_SCANCODE_S });
		keymap.insert({ LEFT, SDL_SCANCODE_A });
		keymap.insert({ RIGHT, SDL_SCANCODE_D });
		keymap.insert({ SPACE, SDL_SCANCODE_SPACE }); //el salto aun no esta includio
	}

}


//el update recibe todos los input de SDL los filtra y envia la respuesta a la clase
void ControlHandeler::update() {

	//la parte para MOVEMENT
	if (ih().isKeyDown(keymap.at(UP))) //aqui es donde ahcemos nuestro keymap
		mov_->updateKeymap(Movement::UP, true);
	else if (ih().isKeyUp(keymap.at(UP)))
		mov_->updateKeymap(Movement::UP, false);
	if (ih().isKeyDown(keymap.at(DOWN)))
		mov_->updateKeymap(Movement::DOWN, true);
	else if (ih().isKeyUp(keymap.at(DOWN)))
		mov_->updateKeymap(Movement::DOWN, false);
	if (ih().isKeyDown(keymap.at(RIGHT)))
		mov_->updateKeymap(Movement::RIGHT, true);
	else if (ih().isKeyUp(keymap.at(RIGHT)))
		mov_->updateKeymap(Movement::RIGHT, false);
	if (ih().isKeyDown(keymap.at(LEFT)))
		mov_->updateKeymap(Movement::LEFT, true);
	else if (ih().isKeyUp(keymap.at(LEFT)))
		mov_->updateKeymap(Movement::LEFT, false);



}