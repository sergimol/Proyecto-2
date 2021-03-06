#include "EntityAttribs.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "Animator.h"
#include "EnemyBehaviour.h"
#include "EnemyMother.h"
#include "Transition.h"
#include "GravityEntity.h"
#include "AnimEnemyStateMachine.h"
#include "Shadow.h"
#include "MenuButton.h"
#include "MenuButtonManager.h"
#include "MenuControlHandler.h"
#include "SoundManager.h"
#include "MapMngr.h"
#include "EnemyMother.h"
#include "FinalBossManager.h"

EntityAttribs::EntityAttribs() :
	health_(100),
	maxHealth_(100),
	velocity_(Vector2D(7, 3.5)),
	maxVelocity_(Vector2D(7, 3.5)),
	strokeResist_(0.0),
	attackRange_(0.0),
	cadence_(0.0),
	damage_(20),
	ignoreMargin_(false),
	marginToAttack(70),

	critProbability_(0.05),
	maxCrit_(0.2),
	critDamage_(1.0),

	poisonDamage_(2),
	canBePoisoned_(true),
	poisonProbability_(0.0),
	canPoison_(poisonProbability_ > 0),
	poisoned_(false),
	poisonTime_(sdlutils().currRealTime()),
	poisonCD_(7000),
	timeLastUpdate_(sdlutils().currRealTime()),
	updateCD_(1500),

	invincibility_(false),
	afterDamageInvul_(false),
	damageInvulTime_(sdlutils().currRealTime()),

	hms_(nullptr),
	hmsText_(nullptr),
	enmState_(nullptr),
	tr_(nullptr),
	state_(nullptr),

	allDead(false),
	alredyDied(false)
{}

EntityAttribs::EntityAttribs(int life, float range, std::string id, Vector2D speed, int number, float poisonProb, int dmg, int marg) :
	playerNumber_(number),
	id_(id),
	health_(life),
	maxHealth_(life),
	velocity_(speed),
	maxVelocity_(speed),
	strokeResist_(0.0),
	attackRange_(range),
	cadence_(0.0),
	damage_(dmg),
	ignoreMargin_(false),
	marginToAttack(marg),

	critProbability_(0.05),
	maxCrit_(0.2),
	critDamage_(1.5),

	poisonDamage_(2),
	canBePoisoned_(true),
	poisonProbability_(poisonProb),
	canPoison_(poisonProbability_ > 0),
	poisoned_(false),
	poisonTime_(sdlutils().currRealTime()),
	poisonCD_(7000),
	timeLastUpdate_(sdlutils().currRealTime()),
	updateCD_(1500),

	invincibility_(false),
	afterDamageInvul_(false),
	damageInvulTime_(sdlutils().currRealTime()),

	hms_(nullptr),
	hmsText_(nullptr),
	enmState_(nullptr),
	tr_(nullptr),
	state_(nullptr),

	allDead(false),
	alredyDied(false)
{}

EntityAttribs::EntityAttribs(int life, float range, std::string id, Vector2D speed, int number, float poisonProb, int dmg, bool igMargin, bool invincibilty, bool canBPois) :
	playerNumber_(number),
	id_(id),
	health_(life),
	maxHealth_(life),
	velocity_(speed),
	maxVelocity_(speed),
	strokeResist_(0.0),
	attackRange_(range),
	cadence_(0.0),
	damage_(dmg),
	ignoreMargin_(igMargin),
	marginToAttack(70),

	critProbability_(0.05),
	maxCrit_(0.2),
	critDamage_(1.5),

	poisonDamage_(2),
	canBePoisoned_(canBPois),
	poisonProbability_(poisonProb),
	canPoison_(poisonProbability_ > 0),
	poisoned_(false),
	poisonTime_(sdlutils().currRealTime()),
	poisonCD_(7000),
	timeLastUpdate_(sdlutils().currRealTime()),
	updateCD_(1500),

	invincibility_(invincibilty),
	afterDamageInvul_(false),
	damageInvulTime_(sdlutils().currRealTime()),

	hms_(nullptr),
	hmsText_(nullptr),
	enmState_(nullptr),
	tr_(nullptr),
	state_(nullptr)
{}

EntityAttribs::~EntityAttribs() {
	if (state_->getState() == GameStates::RUNNING && entity_->hasGroup<Enemy>() && entity_->getMngr()->getHandler<Map>() != nullptr)
		entity_->getMngr()->getHandler<Map>()->getComponent<MapMngr>()->reduceNumberEnemyRoom();
}

void EntityAttribs::init() {
	// Estados si es jugador o enemigo
	if (!entity_->hasGroup<Enemy>()) {
		hms_ = entity_->getComponent<HamsterStateMachine>();
		//assert(hms_ != nullptr);
	}
	else {
		enmState_ = entity_->getComponent<EnemyStateMachine>();
		//assert(enmState_ != nullptr);
	}
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	state_ = entity_->getMngr()->getHandler<StateMachine>()->getComponent<GameStates>();
	assert(state_ != nullptr);
}

void EntityAttribs::update() {
	if (state_->getState() == GameStates::RUNNING) {
		//Timer de invencibilidad
		if (afterDamageInvul_ && sdlutils().currRealTime() > damageInvulTime_ + INVINCIBLECD) {
			afterDamageInvul_ = false;
		}
		//Timer de envenenamiento
		if (poisoned_ && health_ > poisonDamage_) {
			//cada x segundos
			if (sdlutils().currRealTime() >= timeLastUpdate_ + updateCD_) {
				//Animacion de hit 
				if (entity_->getComponent<AnimEnemyStateMachine>() != nullptr)
					entity_->getComponent<AnimEnemyStateMachine>()->setAnimBool(EnemyStatesAnim::HITTED, true);

				//Daño por veneno
				recieveDmg(poisonDamage_);
				timeLastUpdate_ = sdlutils().currRealTime();

			}
			if (sdlutils().currRealTime() >= poisonTime_ + poisonCD_) {
				poisoned_ = false;
			}
		}
		//PONER AQUI ONANIMATIONEND cuando furrule 
		if (allDead && sdlutils().currRealTime() > deadTime + waitAfterDeath) {
				auto cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>();
				if (cam->getCamState() == State::BossCat) {
					cam->changeCamState(State::Players);
					cam->setGoToCat(false);
					cam->setGoToTracker(false);
				}

				allDead = false;

			entity_->getMngr()->getHandler<LevelHandlr>()->getComponent<Transition>()->changeScene("hasMuerto", true, 0);
		}
	}
}

//Resta el da�o y devuelve true si ha muerto
bool EntityAttribs::recieveDmg(int dmg) {
	health_ -= dmg;

	//Timer de invulnerabilidad
	damageInvulTime_ = sdlutils().currRealTime();
	afterDamageInvul_ = true;
	//Actualizamos la healthBar
	if (entity_->hasComponent<UI>())
		entity_->getComponent<UI>()->bar(-dmg);
	if(id_ == "angel")
		entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("angelHit");
	//Si la vida ha bajado de 0...
	if (health_ <= 0) {
		if (hms_ != nullptr) {
			if (id_ == "angel")
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("angelDep");
			hms_->getState() = HamStates::DEAD;
			auto& ents = entity_->getMngr()->getPlayers();

			int i = 0;
			while (i < ents.size()) {
				if (entity_ == ents[i]) {
					entity_->getMngr()->getHandler<Mother>()->getComponent<EnemyMother>()->cleanListHam(i);
					i = ents.size();
				}
				++i;
			}
		}
		else if (enmState_ != nullptr) {
			enmState_->getState() = EnemyStates::ENM_DEAD;
			//TODO
			//aqui distingimos la meurte de un enemigo como tal
			//posibilidades, ambushing o attacking
			EnemyBehaviour* eB = entity_->getComponent<EnemyBehaviour>();
			if (eB != nullptr)
				eB->die();
			else {
				auto* boss = entity_->getMngr()->getHandler<FinalBoss>()->getComponent<FinalBossManager>();
				if (boss != nullptr && boss->getHand() != entity_ && boss->getFist() != entity_)
					entity_->setActive(false);
			}
		}
		//Actualizamos UI
		if (entity_->hasComponent<UI>())
			entity_->getComponent<UI>()->dep("2");
		//Actualizamos UI
		if (entity_->hasComponent<HeartUI>())
			entity_->getComponent<HeartUI>()->dep();

		health_ = 0;
		//Desactivamos la entidad
		if(!alredyDied)
			die();

		//entity_->setActive(false);

		return true;
	}
	else
		return false;
}

void EntityAttribs::die() {
	//Ponemos la camara en estatico
	//entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->changeCamState(State::Static)
	alredyDied = true;
	//Creamos una entidad
	Entity* e = entity_->getMngr()->addEntity();

	//Le metemos un transform para su posicion
	auto* traux = e->addComponent<Transform>(tr_->getPos(), Vector2D(0, 0), tr_->getW(), tr_->getH(), 0, 0, tr_->getFlip(), tr_->getScaleW(), tr_->getScaleH());

	traux->setFloor(tr_->getFloor());
	traux->setZ(tr_->getZ());
	traux->setGravity(e->addComponent<Gravity>());

	int tamX, tamY = 0;

	if (id_ == "sardinilla" || id_ == "monchi" || id_=="angel" || id_ == "soldier1" || id_ == "soldier2" || id_ == "bicho" || id_ == "naranja") {
		tamX = tamY = 86;
	}
	else if (id_ == "canelon" || id_ == "canelonDemon" || id_ == "monosinpatico" || id_ == "rata" || id_ == "ratafachera") {
		tamX = tamY = 128;
	}
	else if (id_ == "keta") {
		tamX = tamY = 100;
	}
	else if (id_ == "pirulo1" || id_ == "pirulo2") {
		tamX = 96;
		tamY = 186;
	}
	else if (id_ == "piruloGordo") {
		tamX = 128;
		tamY = 140;
	}
	if(id_ != "pirulo1" && id_ != "pirulo2" && id_ != "piruloGordo") e->addComponent<Shadow>(false, true);
	//Y reproducimos la animacion de muerto
	e->addComponent<Animator>(&sdlutils().images().at(id_ + "Sheet"),
		tamX,
		tamY,
		3,
		3,
		220,
		Vector2D(0, 0),
		3);

	e->getComponent<Animator>()->play(sdlutils().anims().at(id_ + "_death"));

	//TODO WHY ¿?
	//Si la persona que muere es un hamster...
	if (entity_->hasGroup<Ally>()) {

		//Ponemos su UI a 'Muerto'
		e->addComponent<UI>(id_, entity_->getComponent<UI>()->getPosUI())->dep("2");
		hms_->getState() = HamStates::DEAD;
		//Desactivamos el componente del hasmter vivo
		entity_->getComponent<Animator>()->setActive(false);
		entity_->getComponent<Shadow>()->setActive(false);

		//TODO arreglar camara, y demas objetos que den problemas con el como se esta desactivando el hamster,
		//hacerlo a mano cada vez que os den problemas porque desactivar la entidad del hamster 
		//NO ES UNA OPCION

		auto hams_ = entity_->getMngr()->getPlayers();
		allDead = true;
		for (Entity* e : hams_) {
			auto sta = e->getComponent<HamsterStateMachine>()->getState();
			if (sta != HamStates::DEAD && sta != HamStates::INFARCTED) {
				allDead = false;
			}
		}
		if (allDead) {
			deadTime = sdlutils().currRealTime();
		}
	}
	else {
		/*	if (entity_->getMngr()->getHandler<Boss>() == entity_) {

		}
		else */
		if (entity_->hasComponent<FinalBossAttack>() || entity_->hasComponent<FinalBossPunch>()) {
			auto* boss = entity_->getMngr()->getHandler<FinalBoss>()->getComponent<FinalBossManager>();
			if (boss != nullptr && (boss->getHand() == entity_ || boss->getFist() == entity_)) {
				boss->die();
				entity_->getMngr()->setHandler<FinalBoss>(nullptr);
				entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("handDep");
				entity_->getMngr()->getHandler<LevelHandlr>()->getComponent<Transition>()->changeScene("Level3Boss", true, 2);
			}
		}
		//solamente para los enemigos
		//entity_->getMngr()->getHandler<Map>()->getComponent<MapMngr>()->reduceNumberEnemyRoom();	//Reduce el numero total de enemigos que hay en una sala
		e->addComponent<Dying>();
		//e->getComponent<Transform>()->setGravity(e->addComponent<Gravity>());
		enmState_->getState() = EnemyStates::ENM_DEAD;
		if (id_ == "soldier1" || id_ == "soldier2")
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("soldierDep");
		else if (id_ == "bicho" || id_ == "rata" || id_ == "naranja")
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("dep");
		else if (id_ == "calcetin") {
			entity_->getMngr()->setHandler<Boss>(nullptr);
			entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->setcShake(false);
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("handDep");
			Mix_FadeOutMusic(2000);
			entity_->getMngr()->getHandler<LevelHandlr>()->getComponent<Transition>()->changeScene("Level2", true, 5);
		}

		if(entity_->isActive()) entity_->setActive(false);
	}
}

void EntityAttribs::setLife(int life) {
	int dmg = health_ - life;
	health_ = life;
	if (entity_->hasComponent<UI>())
		entity_->getComponent<UI>()->bar(-dmg);
}

//Sana 'hp' unidades
void EntityAttribs::heal(int hp) {
	if (health_ + hp >= maxHealth_) {
		hp = maxHealth_ - health_;
		health_ = maxHealth_;
	}
	else {
		health_ += hp;
	}
	if (entity_->hasComponent<UI>())
		entity_->getComponent<UI>()->bar(hp);
}

void EntityAttribs::addCritProbability(float probability) {

	critProbability_ += probability;

	if (critProbability_ > maxCrit_) {
		critProbability_ = maxCrit_;
	}
}

//Comienza el envenenamiento
void EntityAttribs::poison() {
	if (canBePoisoned_) {
		//int SDL_SetTextureColorMod(hmsText_, 96, 227, 70);
		poisonTime_ = sdlutils().currRealTime();
		poisoned_ = true;
	}
}

void EntityAttribs::onResume() {
	// Recalcula los timers para tener en cuenta la pausa
	damageInvulTime_ += sdlutils().currRealTime() - damageInvulTime_;
	timeLastUpdate_ += sdlutils().currRealTime() - timeLastUpdate_;
	poisonTime_ += sdlutils().currRealTime() - poisonTime_;
}