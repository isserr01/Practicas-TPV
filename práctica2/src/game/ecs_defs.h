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

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	ImageWithFrames, \
	StarMotion, \
	Points, \
	Miracle, \
	Health

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	STARS, \
	GHOSTS

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    PACMAN

// Systems list - must have at least one element
//
class StarsSystem;
class RenderSystem;
class PacManSystem;
class GhostSystem;
class GameCtrlSystem;
class CollisionsSystem;
class ImmunitySystem;

#define _SYS_LIST_ \
    StarsSystem, \
	RenderSystem, \
	PacManSystem, \
	GhostSystem, \
	GameCtrlSystem, \
	CollisionsSystem, \
	ImmunitySystem

