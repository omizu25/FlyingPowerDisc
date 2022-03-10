//========================================================
//
//UI
//Author:ûüì]
//
//========================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//====================================
//UI\¢ÌÌè`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//Êu
	float fWidth;		//
	float fHeight;		//³
	int nType;			//íÞ
	bool bUse;			//gpµÄ¢é©Ç¤©
}Ui;
//====================================
//vg^Cvé¾
//====================================
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void DeleteUi(int nCntUi);
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
Ui * GetUi(void);

#endif