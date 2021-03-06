// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class HamsterStateMachine;
class Image;
class LightAttack;
class StrongAttack;
class Movement;
class MovementInChase;
class EntityAttribs;
class UI;
class Stroke;
class Animator;
class Pray;
class Combos;
class Roll;
class Turret;
class WarCry;
class DisableOnExit;
class BulletHit;
class ControlHandler;
class MovementSimple;
class FollowPlayer;
class EnemyAttack;
class EnemyStateMachine;
class Poison;
class Ability;
class GetItem;
class Item;
class Cloud;
class MapMngr;
class EnemyStun;
class Stun;
class Gravity;
class DeadBody;
class Knockback;
class HeartUI;
class TileRender;
class PossesionGame;
class GhostCtrl;
class ContactDamage;
class AnimHamsterStateMachine;
class AnimEnemyStateMachine;
class KeyGame;
class HeightObject;
class Camera;
class Swallow;
class EnemyBehaviour;
class Behavior;
class InfarctedBody;
class ReanimationGame;
class Dying;
class FirstBossBehaviour;
class FirstBossAttack;
class EnemyStrongAttack;
class StrongFollowPlayer;
class BackGround;
class CollisionDetec;
class Parallax;
class ImageSecuence;
class Transition;
class TriggerScene;
class MenuButton;
class MenuButtonManager;
class GameStates;
class ShadowFollow;
class Shadow;
class EnemyMother;
class SoundManager;
class GravityEntity;
class CatMovement;
class MenuControlHandler;
class StartChase;
class Obstacle;
class FinalBossManager;
class FinalBossAttack;
class FinalBossPunch;
class TimeTrap;
class MicroOndasManager;
class MenuIndicator;
class dialogos;
class Subtitulos;
class MenuAnim;
class ObstacleMoveable;
class LifeTime;
class ReanimationAlone;
class Creditos;
class TriggerMusic;

#define _CMPS_LIST_  \
	Transform,\
	HamsterStateMachine, \
	Image,\
	LightAttack, \
	StrongAttack, \
	Movement, \
	MovementInChase, \
	EntityAttribs, \
	Stroke, \
	UI, \
	Animator, \
	Pray, \
	Combos, \
	Roll, \
	Turret, \
	WarCry, \
	DisableOnExit, \
	BulletHit, \
	Poison, \
	ControlHandler, \
	MovementSimple,\
	EnemyAttack, \
	Cloud, \
	Item, \
	GetItem, \
	EnemyStateMachine, \
	EnemyStun, \
	Stun, \
	MapMngr, \
	Ability, \
	Gravity, \
	DeadBody, \
	Knockback, \
	HeartUI, \
	MapMngr,\
	TileRender, \
	GhostCtrl, \
	HeightObject, \
	PossesionGame,\
	ContactDamage,\
	Swallow,\
	AnimHamsterStateMachine,\
	AnimEnemyStateMachine,\
	KeyGame,\
	HeightObject,\
	Camera, \
	EnemyBehaviour, \
	Behavior, \
	Camera, \
	InfarctedBody, \
	ReanimationGame, \
	Dying, \
	FirstBossAttack, \
	FirstBossBehaviour, \
	FollowPlayer, \
	BackGround, \
	CollisionDetec, \
	ImageSecuence, \
	StrongFollowPlayer, \
	EnemyStrongAttack, \
	Parallax, \
	MenuButton,\
	MenuButtonManager,\
	Transition, \
	GameStates, \
	EnemyMother, \
	ShadowFollow, \
	TriggerScene, \
	Shadow,\
	SoundManager, \
	GravityEntity, \
	CatMovement, \
	MenuControlHandler,\
	StartChase, \
	Obstacle, \
	FinalBossManager, \
	FinalBossAttack, \
	FinalBossPunch,\
	TimeTrap,\
	MicroOndasManager, \
	MenuIndicator,\
	dialogos,\
	Subtitulos,\
	MenuAnim,\
	ObstacleMoveable, \
	LifeTime, \
	ReanimationAlone, \
	TriggerMusic, \
	Creditos	

// groups

struct Enemy;
struct Ally;
struct Bullet_group;
struct Item_group;
struct Dead;
struct Trap;

#define _GRPS_LIST_  Enemy, Ally, Bullet_group, Item_group, Dead, Trap

// handlers
//Creo que hay que hacer un handler por cada player
struct Hamster1;
struct Hamster2;
struct Hamster3;
struct Hamster4;
struct Camera__;
struct Map;
struct LevelHandlr;
struct StateMachine;
struct Mother;
struct PauseMenu;
struct SoundManager;
struct MainMenu;
struct Boss;
struct FinalBoss;
struct Cat_;
struct PlayerQuantityMenu;
struct HamsterSelectionMenu;
struct OptionsMenu;
struct dialogosMngr;
struct TriggetCat;
#define _HDLRS_LIST_ Hamster1 , \
					Hamster2, \
					Hamster3, \
					Hamster4, \
					Camera__, \
					Map, \
					StateMachine, \
					LevelHandlr, \
					Mother, \
					PauseMenu,\
					PlayerQuantityMenu, \
					HamsterSelectionMenu, \
					SoundManager, \
					Boss, \
					FinalBoss, \
					Cat_, \
					MainMenu, \
					OptionsMenu, \
					TriggetCat, \
					dialogosMngr

//systems

class HamsterSystem;

#define _SYS_LIST_ HamsterSystem