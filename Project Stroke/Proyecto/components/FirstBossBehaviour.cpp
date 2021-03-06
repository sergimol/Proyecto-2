﻿#include "FirstBossBehaviour.h"
#include "Stroke.h"
#include "SoundManager.h"

FirstBossBehaviour::FirstBossBehaviour() :
	mov_(nullptr), tr_(nullptr), rangeOffsetX_(250), rangeOffsetY_(100), lockedHamState_(nullptr),
	lockedHamster_(nullptr), hamsterTr_(nullptr), anim_(nullptr), hamsId_(-1), attackAvailable_(false),
	waitingTime_(sdlutils().currRealTime()), waitingCD_(4000), stunTime_(0), stunCD_(1500), startBehavior_(false) {
}

void FirstBossBehaviour::init()
{
	Entity* owEntity = owner_->getEntity();
	mov_ = owEntity->getComponent<MovementSimple>();
	assert(mov_ != nullptr);

	tr_ = owEntity->getComponent<Transform>();
	assert(tr_ != nullptr);

	bossAtk_ = owEntity->getComponent<FirstBossAttack>();
	assert(bossAtk_ != nullptr);

	anim_ = owEntity->getComponent<AnimEnemyStateMachine>();
	assert(anim_ != nullptr);

	hamsters_ = owEntity->getMngr()->getPlayers();

	lockHamster(); // De momento un hamster concreto para manejar mejor
	assert(lockedHamster_ != nullptr);
	assert(lockedHamState_ != nullptr);
	assert(hamsterTr_ != nullptr);
	assert(anim_ != nullptr);

	//EMPEZAMOS CON LA CINEMATICA
	anim_->setAnimBool(EnemyStatesAnim::SEQUENCE, true);

}


void FirstBossBehaviour::lockHamster() {
	//Variable que contralará el recorrido de los hamsters
	std::vector<Entity*> posHams;
	for (Entity* ham : hamsters_)
	{
		if (!ham->getComponent<HamsterStateMachine>()->cantBeTargeted())
		{
			posHams.emplace_back(ham);
		}
	}

	if (!posHams.empty()) {

		int ham1 = sdlutils().rand().nextInt(0, posHams.size());
		int ham2 = sdlutils().rand().nextInt(0, posHams.size());

		lockedHamster_ = posHams[ham1];
		hamsterTr_ = lockedHamster_->getComponent<Transform>();
		lockedHamState_ = lockedHamster_->getComponent<HamsterStateMachine>();

		//Comienza el tiempo antes de ataque
		waitingTime_ = sdlutils().currRealTime();
	}
	else
	{
		//Si ninguno esta activo pone todo a null
		lockedHamster_ = nullptr;
		lockedHamState_ = nullptr;
		hamsterTr_ = nullptr;
		hamsterTr_ = nullptr;
	}

}

////Fija a un hamster en orden desde el siguiente al último elegido OUTDATED
//void FirstBossBehaviour::lockHamster() {
//	//Variable que contralará el recorrido de los hamsters
//	int start;
//	if (hamsId_ + 1 == hamsters_.size()) {
//		start = 0;
//	}
//	else {
//		start = hamsId_ + 1;
//	}
//
//	//Va comprobando cual es elegible;
//	lockedHamster_ = nullptr;
//	for (int i = start; i != hamsId_ && lockedHamster_ == nullptr; i++) {
//		//Si puede ser elegido
//		lockedHamState_ = hamsters_[i]->getComponent<HamsterStateMachine>();
//		if (!lockedHamState_->cantBeTargeted()) {
//			//Elige hamster
//			hamsId_ = i;
//			lockedHamster_ = hamsters_[i];
//			hamsterTr_ = lockedHamster_->getComponent<Transform>();
//		}
//		//Si llega al final, da la vuelta
//		if (i + 1 == hamsters_.size()) {
//			i = 0;
//		}
//		//Si es -1, entra en el ciclo de ids
//		if (hamsId_ == -1)
//			hamsId_ = 0;
//	}
//	//Si ninguno esta activo pone todo a null
//	if (lockedHamster_ == nullptr) {
//		lockedHamState_ = nullptr;
//		hamsterTr_ = nullptr;
//		hamsId_ = -1;
//	}
//	else {
//		//Comienza el tiempo antes de ataque
//		waitingTime_ = sdlutils().currRealTime();
//	}
//}

//Fija a un hamster concreto
void FirstBossBehaviour::lockHamster(int id) {
	lockedHamster_ = hamsters_[id];
	hamsterTr_ = lockedHamster_->getComponent<Transform>();
	lockedHamState_ = lockedHamster_->getComponent<HamsterStateMachine>();
}

//Esta a rango de ataque
bool FirstBossBehaviour::isWithinAttackRange()
{
	auto hamCollider = hamsterTr_->getRectCollide();
	auto bossCollider = tr_->getRectCollide();

	int hamX = hamCollider.x,
		x = bossCollider.x;

	auto width = bossCollider.w;
	auto hamWidth = hamCollider.w;

	//return(hamX + hamWidth >= x + 3 * width / 4 && hamX <= x + 1 * width / 4);
	return(x < hamX + hamWidth / 4 && x + width > hamX + hamWidth - hamWidth / 4);
}

void FirstBossBehaviour::behave()
{
	//COMPROBACION DE CINEMATICA
	if (!startBehavior_)
	{
		//FIN DE CINEMATICA
		Entity* owEntity = owner_->getEntity();
		if (anim_->getState() == EnemyStatesAnim::SEQUENCE && owEntity->getComponent<Animator>()->OnAnimationFrameEnd())
		{
			startBehavior_ = true;

			anim_->setAnimBool(EnemyStatesAnim::SEQUENCE, false);

		}
		else if (anim_->getState() == EnemyStatesAnim::SEQUENCE && owEntity->getComponent<Animator>()->OnAnimationFrame(0)) {
			//Empieza la musica del boss
			owEntity->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("Nivel1Boss1_0");
			owEntity->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("handInit");
		}
	}
	//SE ACABA LA CINEMATICA, EMPIEZA EL COMPORTAMIENTO DEL BOSS
	else if (lockedHamster_ != nullptr) {
		// Cambia el foco si el actual muere o le da un infarto
		if (lockedHamState_->cantBeTargeted()) {
			lockHamster();
		}
		else if (!bossAtk_->getAttackStarted()) { // si no cambia de hamster marcado Y no está en medio de un ataque

			auto hamCollider = hamsterTr_->getRectCollide();
			auto bossCollider = tr_->getRectCollide();

			int hamX = hamCollider.x,
				x = bossCollider.x;

			auto width = bossCollider.w;
			auto hamWidth = hamCollider.w;
			auto& flip = tr_->getFlip();

			if (x + width / 2 < hamX + hamWidth / 2)
				flip = false;
			else
				flip = true;

			if (!isWithinAttackRange()) {
				// Movimiento del enemigo en base a pos del jugador

				if (x > hamX + hamWidth / 4)
					mov_->updateKeymap(MovementSimple::LEFT, true);
				else
					mov_->updateKeymap(MovementSimple::LEFT, false);
				if (x + width < hamX + hamWidth - hamWidth / 4)
					mov_->updateKeymap(MovementSimple::RIGHT, true);
				else
					mov_->updateKeymap(MovementSimple::RIGHT, false);

				mov_->updateKeymap(MovementSimple::DOWN, false);
				mov_->updateKeymap(MovementSimple::UP, false);
			}
			else { // Si est� a rango, no necesita moverse e intentara atacar si ha pasado cierto tiempo

				mov_->updateKeymap(MovementSimple::RIGHT, false);
				mov_->updateKeymap(MovementSimple::LEFT, false);
				mov_->updateKeymap(MovementSimple::DOWN, false);
				mov_->updateKeymap(MovementSimple::UP, false);

				if (sdlutils().currRealTime() > waitingTime_ + waitingCD_) {
					bossAtk_->LaunchAttack();
					//lockHamster();
					waitingTime_ = sdlutils().currRealTime();
				}
			}
		}
		else {
			waitingTime_ = sdlutils().currRealTime();
		}
	}
}