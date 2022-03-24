//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,
	SOUND_LABEL_TITLE2,
	SOUND_LABEL_BGM,
	SOUND_LABEL_FIGHT,
	SOUND_LABEL_KANSEI,
	SOUND_LABEL_PERFECT,
	SOUND_LABEL_ROUND1,
	SOUND_LABEL_ROUND2,
	SOUND_LABEL_ROUND3,
	SOUND_LABEL_RUN,
	SOUND_LABEL_SHOT,
	SOUND_LABEL_WALLHIT,
	SOUND_LABEL_YOUWIN,
	SOUND_LABEL_ENTER,
	SOUND_LABEL_SELECT,
	SOUND_LABEL_FIVE,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
