﻿#include "StrongFollowPlayer.h"
#include "Stroke.h"

StrongFollowPlayer::StrongFollowPlayer() :
	mov_(nullptr), tr_(nullptr), rangeOffsetX_(250), rangeOffsetY_(100), lockedHamState_(nullptr), lockedHamster_(nullptr),
	hamsterTr_(nullptr), hamsId_(-1), enStrongAtk_(nullptr), enAtk_(nullptr), attackCount_(0), hasHitTime_(sdlutils().currRealTime()),
	hitCD_(1500), enmState_(nullptr) {
}

void StrongFollowPlayer::init() {
	Entity* owEntity = owner_->getEntity();

	enmState_ = owEntity->getComponent<EnemyStateMachine>();
	assert(enmState_ != nullptr);

	mov_ = owEntity->getComponent<MovementSimple>();
	assert(mov_ != nullptr);

	tr_ = owEntity->getComponent<Transform>();
	assert(tr_ != nullptr);

	enStrongAtk_ = owEntity->getComponent<EnemyStrongAttack>();
	assert(enStrongAtk_ != nullptr);
	enAtk_ = owEntity->getComponent<EnemyAttack>();
	assert(enAtk_ != nullptr);

	hamsters_ = owEntity->getMngr()->getPlayers();

	lockHamster(); // De momento un hamster concreto para manejar mejor
	assert(lockedHamster_ != nullptr);
	assert(lockedHamState_ != nullptr);
	assert(hamsterTr_ != nullptr);
}


//Fija a un hamster en orden desde el siguiente al último elegido
void StrongFollowPlayer::lockHamster() {
	//Variable que contralará el recorrido de los hamsters
	int start;
	if (hamsId_ + 1 == hamsters_.size()) {
		start = 0;
	}
	else {
		start = hamsId_ + 1;
	}

	//Va comprobando cual es elegible;
	lockedHamster_ = nullptr;
	for (int i = start; i != hamsId_ && lockedHamster_ == nullptr; i++) {
		//Si puede ser elegido
		lockedHamState_ = hamsters_[i]->getComponent<HamsterStateMachine>();
		if (!lockedHamState_->cantBeTargeted()) {
			//Elige hamster
			hamsId_ = i;
			lockedHamster_ = hamsters_[i];
			hamsterTr_ = lockedHamster_->getComponent<Transform>();
		}
		//Si llega al final, da la vuelta
		if (i + 1 == hamsters_.size()) {
			i = 0;
		}
		//Si es -1, entra en el ciclo de ids
		if (hamsId_ == -1)
			hamsId_ = 0;
	}
	//Si ninguno esta activo pone todo a null
	if (lockedHamster_ == nullptr) {
		lockedHamState_ = nullptr;
		hamsterTr_ = nullptr;
		hamsId_ = -1;
	}
}

//Fija a un hamster concreto
void StrongFollowPlayer::lockHamster(int id) {
	lockedHamster_ = hamsters_[id];
	hamsterTr_ = lockedHamster_->getComponent<Transform>();
	lockedHamState_ = lockedHamster_->getComponent<HamsterStateMachine>();
}

//Esta a rango de ataque
bool StrongFollowPlayer::isWithinAttackRange() {
	auto width = tr_->getW();
	auto hamWidth = hamsterTr_->getW();

	auto& hamPos = hamsterTr_->getPos();
	auto& pos = tr_->getPos();
	int hamX = hamPos.getX(),
		hamY = hamPos.getY() + hamsterTr_->getH(),
		x = pos.getX(),
		y = pos.getY() + tr_->getH();

	return((hamX /*+ rangeOffsetX_*/ + hamWidth * 2 >= x + width && hamX + hamWidth - rangeOffsetX_ <= x + width /*+ width / 2*/) &&
		(hamY + rangeOffsetY_ >= y && hamY - rangeOffsetY_ / 10 <= y));
}

void StrongFollowPlayer::behave() {
	if (lockedHamster_ != nullptr) {
		// Cambia el foco si el actual muere o le da un infarto
		auto& state = lockedHamState_->getState();
		if (lockedHamState_->cantBeTargeted()) {
			lockHamster();
		}
		else if (enmState_->getState() != EnemyStates::ENM_STUNNED) { // si no cambia de hamster marcado y no está atrudido
			auto& hamPos = hamsterTr_->getPos();
			auto& pos = tr_->getPos();
			int hamX = hamPos.getX(),
				hamY = hamPos.getY() + hamsterTr_->getH(),
				x = pos.getX(),
				y = pos.getY() + tr_->getH();

			auto width = tr_->getW();
			auto hamWidth = hamsterTr_->getW();
			auto& flip = tr_->getFlip();

			if (x + width / 2 < hamX + hamWidth / 2)
				flip = false;
			else
				flip = true;

			if (!isWithinAttackRange()) {
				// Movimiento del enemigo en base a pos del jugador
				if (y < hamY - rangeOffsetY_ / 10)
					mov_->updateKeymap(MovementSimple::DOWN, true);
				else
					mov_->updateKeymap(MovementSimple::DOWN, false);
				if (y > hamY + rangeOffsetY_)
					mov_->updateKeymap(MovementSimple::UP, true);
				else
					mov_->updateKeymap(MovementSimple::UP, false);


				if (x > hamX + rangeOffsetX_)
					mov_->updateKeymap(MovementSimple::LEFT, true);
				else
					mov_->updateKeymap(MovementSimple::LEFT, false);
				if (x < hamX - rangeOffsetX_ / 2 - tr_->getW() / 2)
					mov_->updateKeymap(MovementSimple::RIGHT, true);
				else
					mov_->updateKeymap(MovementSimple::RIGHT, false);
			}
			else if (attackCount_ < 2 && sdlutils().currRealTime() > hasHitTime_ + hitCD_) { // Si esta a rango, se mueve e intentara atacar
				lightAttack();
			}
			else if (sdlutils().currRealTime() > hasHitTime_ + hitCD_) { // ataque fuerte
				strongAttack(flip);
			}
		}
	}
}

void StrongFollowPlayer::strongAttack(bool f) {
	if (!enStrongAtk_->getAttackStarted() && enStrongAtk_->LaunchAttack()) {

		if (f) {
			mov_->updateKeymap(MovementSimple::RIGHT, true);
			mov_->updateKeymap(MovementSimple::LEFT, false);
		}
			
		else {
			mov_->updateKeymap(MovementSimple::RIGHT, false);
			mov_->updateKeymap(MovementSimple::LEFT, true);
		}


		hasHitTime_ = sdlutils().currRealTime();
		attackCount_ = 0;
	}
}
void StrongFollowPlayer::lightAttack() {
	if (!enStrongAtk_->getAttackStarted()) {
		mov_->updateKeymap(MovementSimple::RIGHT, false);
		mov_->updateKeymap(MovementSimple::LEFT, false);
		mov_->updateKeymap(MovementSimple::DOWN, false);
		mov_->updateKeymap(MovementSimple::UP, false);

		if (enAtk_->LaunchAttack()) {
			hasHitTime_ = sdlutils().currRealTime();
			attackCount_++;
		}
	}
}