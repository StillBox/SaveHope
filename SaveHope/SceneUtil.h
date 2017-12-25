#pragma once
#ifndef SCENEUTIL_H
#define SCENEUTIL_H

//
//Math
//

#define EPSILON 0.001f
#define MAXSIZE	256
#define HALFSIZE 128

//
//Player State
//

//Common

#define D_STAY	 0
#define D_MOVE	 1
#define D_JUMP	 2
#define D_DASH	 3

#define U_STAY	10
#define U_MOVE	11
#define U_JUMP	12
#define U_DASH	13

#define L_STAY	20
#define L_MOVE	21
#define L_JUMP	22
#define L_DASH	23

#define R_STAY	30
#define R_MOVE	31
#define	R_JUMP	32
#define R_DASH	33

#define SMILE	40
#define SHOCK	41
#define CRY		42
#define HAPPY	43

#define WAIT	90

//
//Item
//

#define ITEM_BLUE	1
#define ITEM_GOLD	2

//Special

//Shooter

#define PERFECT		200
#define GOOD		201
#define BAD			202
#define MISS		203

#define POISON		214
#define SMOKE		215
#define FIRE		216
#define HIDDEN		217
#define NOEFFECT	218

#define COMBO		220

//Snake

#define DEATH		300
#define D_OFF		301
#define U_OFF		302
#define L_OFF		303
#define R_OFF		304

#define ADDTIME		310

//Upstairs

#define NORMAL_SPACE	428
#define NORMAL_FLOOR	429
#define NORMAL_DSTAIR	430
#define NORMAL_USTAIR	431

#define TARGET			498
#define CEILING			499

#endif // !SCENEUTIL_H
