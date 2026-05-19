// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// Components list - must have at least one element
//
struct Transform;
struct Image;
struct ImageWithFrames;
struct StarMotion;
struct Points;
struct Miracle;
struct Health;
struct Weak;

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	ImageWithFrames, \
	StarMotion, \
	Points, \
	Miracle, \
	Health, \
	Weak

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	FRUITS, \
	GHOSTS

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    PACMAN

// Systems list - must have at least one element
//
class FruitSystem;
class RenderSystem;
class PacManSystem;
class GhostSystem;
class GameCtrlSystem;
class CollisionsSystem;
class ImmunitySystem;
class SafeSystem;

#define _SYS_LIST_ \
    FruitSystem, \
	RenderSystem, \
	PacManSystem, \
	GhostSystem, \
	GameCtrlSystem, \
	CollisionsSystem, \
	ImmunitySystem, \
	SafeSystem

