// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class HamsterStateMachine;
class Image;
class LightAttack;
class StrongAttack;
class Movement;
class Life;
class UI;
class Stroke;
class Animator;
class Pray;
class Roll;

#define _CMPS_LIST_  \
	Transform,\
	HamsterStateMachine, \
	Image,\
	LightAttack, \
	StrongAttack, \
	Movement, \
	Life, \
	Stroke, \
	UI, \
	Animator, \
	Pray, \
	Roll

// groups

struct Enemy; 
struct Ally;
#define _GRPS_LIST_  Enemy, Ally

// handlers
//Creo que hay que hacer un handler por cada player
struct Player;
#define _HDLRS_LIST_ Player
