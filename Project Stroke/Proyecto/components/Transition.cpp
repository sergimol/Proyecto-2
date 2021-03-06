#include "Transition.h"
#include "Transform.h"
#include "ImageSecuence.h"
#include "EnemyMother.h"
#include "../ecs/Camera.h"
#include "MenuButton.h"
#include "Creditos.h"
#include "MenuIndicator.h"
#include "MenuButtonManager.h"
#include "SoundManager.h"
#include "MapMngr.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/Camera.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

void Transition::init() {
	tx_->setBlendMode(SDL_BLENDMODE_BLEND);

	blackRect.x = 0;
	blackRect.y = 0;
	blackRect.w = sdlutils().width();
	blackRect.h = sdlutils().height();

	state_ = entity_->getMngr()->getHandler<StateMachine>()->getComponent<GameStates>();
	assert(state_ != nullptr);
}

void Transition::update() {
	if (state_->getState() == GameStates::RUNNING || state_->getState() == GameStates::CONTROLS || state_->getState() ==  GameStates::MAINMENU) {
		if (fadingOut)
			fadeOut();

		if (fadingIn)
			fadeIn();
	}
}

void Transition::render() {
	if (fadingIn || fadingOut) {
		tx_->render(blackRect);
	}
}

void Transition::fadeOut() {

	// Comprueba si hay textura
	if (tx_) {
		// Setea el alpha de la textura
		tx_->setAlpha(alpha);
	}

	// Actualiza el alpha de la textura
	if (alpha < SDL_ALPHA_OPAQUE) {
		alphaCalc += FADE_SPEED * (SDL_GetTicks() / 1000);
		alpha = alphaCalc;
	}

	// Si el alpha supera 255, lo iguala a 255;
	if (alpha >= SDL_ALPHA_OPAQUE) {
		alpha = SDL_ALPHA_OPAQUE;
		alphaCalc = (float)SDL_ALPHA_OPAQUE;
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("nextPage");
		if (change)
			sceneTransition();
		startFadeOut();
	}
}

void Transition::fadeIn() {
	//FADE IN
	if (alpha == SDL_ALPHA_OPAQUE) {
		if ( numTReference > 0) {
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("stopTutorial");
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("transition");
			numTReference--;
		}
	}
	// Comprueba si hay textura
	if (tx_) {
		// Setea el alpha de la textura
		tx_->setAlpha(alpha);
	}

	// Actualiza el alpha de la textura
	if (alpha > SDL_ALPHA_TRANSPARENT) {
		alphaCalc -= FADE_SPEED * (SDL_GetTicks() / 1000);
		alpha = alphaCalc;
	}

	// Si el alpha esta por debajo de 0, lo iguala a 0;
	if (alpha <= SDL_ALPHA_TRANSPARENT) {
		alpha = SDL_ALPHA_TRANSPARENT;
		alphaCalc = (float)SDL_ALPHA_TRANSPARENT;
		fadingIn = false;
		if (numTReference == 0)
		{
			if (nameScene_ == "Level1" && playeasonido) {
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("Nivel1GameVersion");
			}
			else if (nameScene_ == "Level2" && playeasonido) {
				sdlutils().setTutorialDone();
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("birds");
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("HamstersNivel2GameVersion");
			}
			else if (nameScene_ == "Level3" && playeasonido) {
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("HamstersNivel4GameVersion");
			}
			else if (nameScene_ == "Level3Boss" && playeasonido) {
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("HamstersNivel4_Boss2");
			}
			/*else if (nameScene_ == "Level3Micro" && playeasonido) {
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("HamstersNivel4_Boss2");
			}*/
			else if (nameScene_ == "final" && playeasonido) {
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("Nivel1GameVersion");
			}
			playeasonido = !playeasonido;
		}
	}
}

void Transition::changeScene(string nameScene, bool changeMap, int numTransitions) {
	auto& ents = entity_->getMngr()->getPlayers();
	for (Entity* e : ents) {
		e->getComponent<Transform>()->setFloor(0);
		e->getComponent<Transform>()->setZ(0);
	}
	entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->setHeightMap(0);
	change = true;
	changeMap_ = changeMap;
	nameScene_ = nameScene;
	numTReference = numT = numTransitions;
	startFadeIn();
}

void Transition::sceneTransition() {
	change = false;
	if (changeMap_) {
		//Y eliminamos TODO
		for (Entity* e : entity_->getMngr()->getEntities()) {

			if (e->getMngr()->getHandler<Camera__>() != e
				&& e->getMngr()->getHandler<StateMachine>() != e && e->getMngr()->getHandler<Mother>() != e &&
				e->getMngr()->getHandler<Map>() != e && e->getMngr()->getHandler<SoundManager>() != e)
				//La elimino
				e->setActive(false);
		}

		//Elimino los efectos del nivel anterior
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->StopBossSounds();
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->StopTutorial();


		for (Entity* e : entity_->getMngr()->getTiles())
			e->setActive(false);

		for (Entity* e : entity_->getMngr()->getMapH())
			e->setActive(false);

		for (Entity* e : entity_->getMngr()->getBgs())
			e->setActive(false);

		for (Entity* e : entity_->getMngr()->getFgs())
			e->setActive(false);

		for (Entity* e : entity_->getMngr()->getWavesObjects())
			e->setActive(false);

		for (Entity* e : entity_->getMngr()->getTraps())
			e->setActive(false);


		entity_->getMngr()->refreshFrontGround();
		entity_->getMngr()->refreshTiles();
		entity_->getMngr()->refreshMapHeight();
		entity_->getMngr()->refreshParallax();
		entity_->getMngr()->refreshForeground();


		entity_->getMngr()->refreshDeadBodies();
		entity_->getMngr()->refreshEnemies();
		entity_->getMngr()->refreshItems();
		entity_->getMngr()->refreshObstacles();
		entity_->getMngr()->refreshPlayers();
		entity_->getMngr()->refreshWavesObjects();
		entity_->getMngr()->refreshTraps();
		entity_->getMngr()->refresh();

		entity_->getMngr()->getHandler<Mother>()->getComponent<EnemyMother>()->resetNumPlayers();

		auto* map = entity_->getMngr()->getHandler<Map>()->getComponent<MapMngr>();
		map->clearColliders();

		map->resetTriggerList();
	}

	//Y creamos uno nuevo
	auto* images = entity_->getMngr()->addFrontGround();
	images->addComponent<ImageSecuence>(nameScene_);

}

void Transition::createMap() {
	//Y creamos uno nuevo
	if (nameScene_ == "final") {
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->resetNumInts();
		entity_->addComponent<Creditos>();
		// Desbloqueo de angel
		sdlutils().unlockAngel();
		auto hms = entity_->getMngr()->getHandler<HamsterSelectionMenu>()->getComponent<MenuButtonManager>();
		hms->getButtons()[4][0]->getComponent<MenuButton>()->setSelectable(true);
		hms->unlockAngelBackground();
	}
	else if (nameScene_ == "hasMuerto") {
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->resetNumInts();
		entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->resetCamera();
		//Vuelve a renderizar el menu
		state_->setState(GameStates::MAINMENU);

		//Eliminamos a todos los hamsters
		entity_->getMngr()->getHandler<Map>()->getComponent<MapMngr>()->clearHamstersVector();

		sdlutils().setHamstersChosen(0);
		sdlutils().setHamstersToChoose(0);

		auto* hsm = entity_->getMngr()->getHandler<HamsterSelectionMenu>()->getComponent<MenuButtonManager>();

		auto& i = hsm->getIndicators();
		auto& but = hsm->getButtons();
		for (int h = 1; h < i.size(); h++) {
			i[h]->setActive(false);
		}

		for (int j = 0; j < but.size(); ++j) {
			auto mb = but[j][0]->getComponent<MenuButton>();
			if (mb->getName() != "angel" || sdlutils().angelUnlocked())
				mb->setSelectable(true);
		}

		i[0]->getComponent<MenuIndicator>()->reset();

		entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->changeCamState(State::Players);
	}
	else if (changeMap_) {
		if (nameScene_ == "Level3") {
			auto cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>();
			cam->changeCamState(State::Players);
			cam->setGoToCat(false);
			cam->setGoToTracker(false);
		}
		auto* mapa = entity_->getMngr()->getHandler<Map>();
		mapa->getComponent<MapMngr>()->loadNewMap("resources/images/tiled/" + nameScene_ + ".tmx");

		//Metemos al mapa en el Handler de Map
		//entity_->getMngr()->setHandler<Map>(mapa);

		auto cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>();

		cam->changeCamState(State::Players);
		cam->setMap(mapa->getComponent<MapMngr>());
	}
}

bool Transition::isFadingOut() {
	return fadingOut;
}

bool Transition::isFading() {
	return fadingIn || fadingOut;
}