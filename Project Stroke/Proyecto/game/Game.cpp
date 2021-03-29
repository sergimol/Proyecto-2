// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/Transform.h"
#include "../components/HamsterStateMachine.h"
#include "../components/Image.h"
#include "../components/Movement.h"
#include "../components/LightAttack.h"
#include "../components/StrongAttack.h"
#include "../components/Stroke.h"
#include "../components/UI.h"
#include "../components/Animator.h"
#include "../components/Roll.h"
#include "../components/Poison.h"
#include "../components/Pray.h"
#include "../components/Combos.h"
#include "../components/Turret.h"
#include "../components/FollowPlayer.h"
#include "../components/MovementSimple.h"
#include "../components/EnemyAttack.h"
#include "../components/EnemyStateMachine.h"

#include "../components/ControlHandeler.h"

#include "../ecs/ecs.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../ecs/Manager.h"
#include "../utils/Vector2D.h"

SDL_Rect Game::camera_ = { 0,0,1920, 1080 };

Game::Game() {
	mngr_.reset(new Manager());
}

Game::~Game() {
}

void Game::init() {

	SDLUtils::init("Squeak Ship", 1920, 1080, "resources/config/hamsters.resources.json");

	//Imagen de fondo fija
	/*auto* background = mngr_->addEntity();
	background->addComponent<Transform>(
		Vector2D(0, 0),
		Vector2D(), 1920.0f, 1010.0f, 0.0f);

	background->addComponent<Image>(&sdlutils().images().at("background"));*/


	////MATERIAL PARA EL HITO1
	////Keta
	//auto* hamster2 = mngr_->addEntity();
	//hamster2->addComponent<Transform>(
	//	Vector2D(sdlutils().width() / 2.0f - 500, sdlutils().height() / 2.0f + 200),
	//	Vector2D(), 256.0f, 256.0f, 0.0f);
	//hamster2->addComponent<Image>(&sdlutils().images().at("keta"));
	//hamster2->addComponent<HamsterStateMachine>();
	//hamster2->addComponent<LightAttack>(20);
	//hamster2->addComponent<StrongAttack>(30);
	//hamster2->addComponent<Stroke>();
	//hamster2->addComponent<Life>(100);
	//hamster2->addComponent<UI>("keta", 1);

	//players_.push_back(hamster2);

	//auto* hamster3 = mngr_->addEntity();
	//hamster3->addComponent<Transform>(
	//	Vector2D(sdlutils().width() / 2.0f -800, sdlutils().height() / 2.0f + -50),
	//	Vector2D(), 256.0f, 256.0f, 0.0f);
	//hamster3->addComponent<Image>(&sdlutils().images().at("monchi"));
	//hamster3->addComponent<HamsterStateMachine>();
	//hamster3->addComponent<LightAttack>(20);
	//hamster3->addComponent<StrongAttack>(30);
	//hamster3->addComponent<Stroke>();
	//hamster3->addComponent<Life>(100);
	//hamster3->addComponent<UI>("monchi", 2);

	//players_.push_back(hamster3);

	////MATERIAL PARA EL HITO1//

	//Sardinilla
	auto* hamster1 = mngr_->addEntity();
	hamster1->addComponent<Transform>(
		Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 2.0f),
		Vector2D(), 256.0f, 256.0f, 0.0f);
	hamster1->addComponent<EntityAttribs>(100, 0.0, "sardinilla", Vector2D(7, 4.5));
	//hamster1->addComponent<Image>(&sdlutils().images().at("sardinilla"));
	hamster1->addComponent<Animator>(
		&sdlutils().images().at("sardinillaSheet"),
		64,
		64,
		3,
		3,
		220,
		Vector2D(0, 0),
		3
		);

	hamster1->addComponent<HamsterStateMachine>();
	hamster1->addComponent<Movement>();
	hamster1->addComponent<LightAttack>();
	hamster1->addComponent<StrongAttack>();
	hamster1->addComponent<Stroke>();
	hamster1->addComponent<UI>("sardinilla", 0);
	//hamster1->addComponent<Pray>(30, 50);
	//hamster1->addComponent<Roll>();
	//hamster1->addComponent<Turret>();
	hamster1->addComponent<Poison>(5);
	hamster1->addComponent<Combos>();
	hamster1->setGroup<Ally>(true);
	hamster1->addComponent<ControlHandeler>(1);
	players_.push_back(hamster1);
	//Igual luego no lo usammos pero por si aca
	mngr_->setHandler<Hamster1>(hamster1);


	//CLON Sardinilla (P2)
	//auto* hamster2 = mngr_->addEntity();
	//hamster2->addComponent<Transform>(
	//	Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 2.0f),
	//	Vector2D(), 256.0f, 256.0f, 0.0f);
	//hamster2->addComponent<EntityAttribs>(100);
	////hamster1->addComponent<Image>(&sdlutils().images().at("sardinilla"));
	//hamster2->addComponent<Animator>(
	//	&sdlutils().images().at("sardinillaSheet"),
	//	64,
	//	64,
	//	3,
	//	1,
	//	220,
	//	Vector2D(0, 0),
	//	Vector2D(2, 0)
	//	);
	//hamster2->addComponent<HamsterStateMachine>();
	//hamster2->addComponent<Movement>();
	//hamster2->addComponent<LightAttack>();
	//hamster2->addComponent<StrongAttack>();
	//hamster2->addComponent<Stroke>();
	//hamster2->addComponent<UI>("sardinilla", 1);
	////hamster1->addComponent<Pray>(30, 50);
	//hamster2->addComponent<Roll>();
	////hamster1->addComponent<Turret>();
	//hamster2->addComponent<Combos>();
	//hamster2->setGroup<Ally>(true);
	//hamster2->addComponent<ControlHandeler>(2);
	//players_.push_back(hamster2);


	//Enemigo de prueba con la imagen de canelón
	auto* enemy = mngr_->addEntity();
	enemy->addComponent<EntityAttribs>(200, 0.0, "enemy", Vector2D(4.5, 2));
	enemy->addComponent<Transform>(
		Vector2D(sdlutils().width() / 2.0f + 400, sdlutils().height() / 2.0f - 100),
		Vector2D(), 500.0f, 500.0f, 0.0f)->getFlip() = true;
	enemy->addComponent<Image>(&sdlutils().images().at("canelon"));
	enemy->setGroup<Enemy>(true);
	enemy->addComponent<UI>("canelon", 4);


	enemy->addComponent<EnemyStateMachine>();
	enemy->addComponent<EnemyAttack>();
	enemy->addComponent<MovementSimple>();
	enemy->addComponent<FollowPlayer>(players_);
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	//Cargamos tiled

	while (!exit) {


		Uint32 startTime = sdlutils().currRealTime();

		ih().clearState();
		while (SDL_PollEvent(&event))
			ih().update(event);

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->update();
		mngr_->refresh();

		updateCamera();

		sdlutils().clearRenderer();
		loadMap();
		mngr_->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}

void Game::updateCamera() {

	Vector2D camPos;
	int players(0);
	//Cámara sigue a los personajes
	for (Entity* e : players_) {
		auto& playerpos = e->getComponent<Transform>()->getPos();

		// Operación para calcular el punto medio con más jugadores
		camPos = camPos + playerpos;
		players++;
	}

	camera_.x = (camPos.getX() / players) - camera_.w / 2;
	camera_.y = (camPos.getY() / players) - camera_.h / 2;

	// Bordes de la cámara
	/*
	if (camera_.x < 0)
		camera_.x = 0;
	if (camera_.y < 0)
		camera_.y = 0;
	if (camera_.x > camera_.w)
		camera_.x = camera_.w;
	if (camera_.h > camera_.h)
		camera_.y = camera_.h;
	*/

	//std::cout << camera_.x << " " << camera_.y << "\n";
}

void Game::loadMap() {
	//Creamos el mapa
	tmx::Map map;
	//Si se puede cargar
	if (map.load("resources/images/tiled/Mapa.tmx"))
	{
		//Variables que vamos a necesitar
		tmx::Vector2u mapDimensions;	//Guarda las dimensiones del mapa
		tmx::Vector2u tilesDimensions;	//Guarda las dimensiones de las tiles
		Texture* tilesetsArr[2]; int indice = 0;
		//------------------

		//Guardamos las propiedades basicas
		//Dimensiones del mapa
		mapDimensions = map.getTileCount();
		//Dimensiones de los tiles
		tilesDimensions = map.getTileSize();

		//Cargamos los tilesets y guardamos las texturas
		const auto& tilesets = map.getTilesets();
		for (const auto& tileset : tilesets)
		{
			//Guardamos las texturas de los tilesets
			tilesetsArr[indice] = &sdlutils().images().at(tileset.getName());	//El nombre del tileset en Tiled y la textura png DEBEN llamarse igual
			indice++;
		}

		//Recorremos las capas ("Floor", "Cuber")
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			//SI ES UNA CAPA DE OBJETOS, TODAVÍA NO
			//if (layer->getType() == tmx::Layer::Type::Object)
			//{
			//	const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
			//	const auto& objects = objectLayer.getObjects();
			//	for (const auto& object : objects)
			//	{
			//		//do stuff with object properties
			//	}
			//} else

			//SI ES UNA CAPA DE TILES
			indice = 0;	//Recorrerá los tilesets para saber a cual corresponde cada tile
			if (layer->getType() == tmx::Layer::Type::Tile)
			{
				const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
				//read out tile layer properties etc...
				for (int j = 0; j < mapDimensions.y; j++) {
					for (int i = 0; i < mapDimensions.x; i++) {


						//Guardamos el indice global del tile (nos servirá para saber en qué tileset se encuentra)
						auto tileList = tileLayer.getTiles();
						auto globalIndexTile = tileList[i + j * mapDimensions.x].ID;	//filas+columna*elementos_enuna_fila

					//Necesitamos saber a cual de los tilesets pertenece esa posicion
						while (globalIndexTile > tilesets[indice].getLastGID()) {
							indice++;	//Marca la posicion del tileset al que pertenece el tile
						}

						//Calculamos la posicion del tile en la pantalla -> DestRect
						auto x = i * tilesDimensions.x;
						auto y = j * tilesDimensions.y;

						//Calculamos la posición del tile en el tileset -> SrcRect
						int tilesetSize = tilesets[indice].getColumnCount();
						//Calculamos las coordenadas locales del tile
						//Hay que restar el valor del primer tile del tileset a la posicion global
						auto localIndexTile = globalIndexTile - tilesets[indice].getFirstGID();

						auto Srcx = (localIndexTile % tilesets[indice].getColumnCount()) * tilesDimensions.x;
						auto Srcy = (localIndexTile / tilesets[indice].getColumnCount()) * tilesDimensions.y;

						//Sacamos el SDL_SrcRect y SDL_DestRect con el que imprimimos los tiles
						SDL_Rect src;
						src.x = Srcx; src.y = Srcy;
						src.w = src.h = tilesDimensions.x;	//Las tiles son cuadradas

						int scale = 6;

						SDL_Rect dest;
						dest.x = x * scale; dest.y = y * scale;
						dest.w = dest.h = tilesDimensions.x * scale;

						Vector2D renderPos = Vector2D(dest.x - Game::camera_.x, dest.y - Game::camera_.y);
						dest.x = renderPos.getX();
						dest.y = renderPos.getY();

						if (globalIndexTile != 0)
							tilesetsArr[indice]->render(src, dest);
					}
				}
			}
		}


	}
	else
		std::cout << "Lavin que pringao";
}