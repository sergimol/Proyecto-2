#include "AnimEnemyStateMachine.h"
#include "EntityAttribs.h"
#include "Animator.h"
#include "Knockback.h"

void AnimEnemyStateMachine::init()
{
	anim = entity_->getComponent<Animator>();
	assert(anim != nullptr);
	ent = entity_->getComponent<EntityAttribs>();
	assert(ent != nullptr);
	id = ent->getId();
	gState_ = entity_->getMngr()->getHandler<StateMachine>()->getComponent<GameStates>();
	assert(gState_ != nullptr);
}

void AnimEnemyStateMachine::update()
{
	if (gState_->getState() == GameStates::RUNNING) {
		HandleAnimState();
		CheckAnimState();
	}
}


//Maneja las transiciones entre los estados
void AnimEnemyStateMachine::HandleAnimState()
{
	//idle
	if (idle && !lAttack && !move)
		currentState = EnemyStatesAnim::IDLE;
	//move
	if (move && !lAttack && !idle)
		currentState = EnemyStatesAnim::MOVE;
	//quedarse en el suelo para los bosses (va delante del hit para cortarlo)
	if (onfloor)
		currentState = EnemyStatesAnim::ONFLOOR;
	//light attack
	if (lAttack /*&& !sAttack*/)
		currentState = EnemyStatesAnim::ATTACK;
	//strong attack
	if (sAttack && !lAttack)
		currentState = EnemyStatesAnim::STRONGATTACK;
	
	//hitted
	if (hit)
		currentState = EnemyStatesAnim::HITTED;
	//stunned
	if (stun)
		currentState = EnemyStatesAnim::STUNNED;
	if (up)
		currentState = EnemyStatesAnim::UP;
	if (sequence)
		currentState = EnemyStatesAnim::SEQUENCE;
	if(shield)
		currentState = EnemyStatesAnim::SHIELD;
}

//Cambia las animaciones dependiendo del estado del hamster
void AnimEnemyStateMachine::CheckAnimState()
{

	//Si es diferente al anterior hay que cambiar la animacion
	if (lastState != currentState)
	{
		//Depende del estado playeamos una animacion u otra
		switch (currentState)
		{
		case EnemyStatesAnim::IDLE:
			anim->play(sdlutils().anims().at(id + "_idle"));
			break;
		case EnemyStatesAnim::MOVE:
			anim->play(sdlutils().anims().at(id + "_move"));
			break;
		case EnemyStatesAnim::JUMPUP:
			anim->play(sdlutils().anims().at(id + "_idle")); //CAMBIAR
			break;
		case EnemyStatesAnim::JUMPDOWN:
			anim->play(sdlutils().anims().at(id + "_idle")); //CAMBIAR
			break;
		case EnemyStatesAnim::ATTACK:
			anim->play(sdlutils().anims().at(id + "_attack"));
			break;
		case EnemyStatesAnim::STRONGATTACK:
			anim->play(sdlutils().anims().at(id + "_strongattack"));
			break;
		case EnemyStatesAnim::HITTED:
			anim->play(sdlutils().anims().at(id + "_hit"));
			break;
		case EnemyStatesAnim::STUNNED:
			anim->play(sdlutils().anims().at(id + "_stun"));
			break;
		case EnemyStatesAnim::DEAD:
			anim->play(sdlutils().anims().at(id + "_death"));
			break;
		case EnemyStatesAnim::ONFLOOR:
			anim->play(sdlutils().anims().at(id + "_floor"));
			break;
		case EnemyStatesAnim::UP:
			anim->play(sdlutils().anims().at(id + "_up"));
			break;
		case EnemyStatesAnim::SEQUENCE:
			anim->play(sdlutils().anims().at(id + "_sequence"));
			break;		
		case EnemyStatesAnim::SHIELD:
			anim->play(sdlutils().anims().at(id + "_shield"));
			break;
		}
	}

	//Recogemos
	lastState = currentState;

}

//PONE A TRUE O FALSE LOS BOOLEANOS DE ANIMACION
void AnimEnemyStateMachine::setAnimBool(EnemyStatesAnim h, bool b)
{

	switch (h)
	{
	case EnemyStatesAnim::IDLE:
		idle = b;
		break;
	case EnemyStatesAnim::MOVE:
		move = b;
		break;
	case EnemyStatesAnim::JUMPUP:
		idle = b;
		break;
	case EnemyStatesAnim::JUMPDOWN:
		idle = b;
		break;
	case EnemyStatesAnim::ATTACK:
		lAttack = b;
	case EnemyStatesAnim::STRONGATTACK:
		sAttack = b;
		break;
	case EnemyStatesAnim::HITTED:
		hit = b;
		break;
	case EnemyStatesAnim::STUNNED:
		stun = b;
		break;
	case EnemyStatesAnim::DEAD:
		idle = b;
		break;
	case EnemyStatesAnim::ONFLOOR:
		onfloor = b;
		break;
	case EnemyStatesAnim::UP:
		up = b;
		break;
	case EnemyStatesAnim::SEQUENCE:
		sequence = b;
		break;
	case EnemyStatesAnim::SHIELD:
		shield = b;
		break;
	}
}