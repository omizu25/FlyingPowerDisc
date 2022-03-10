//========================================================
//
//UI処理
//Author:髙野馨將
//
//========================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//====================================
//UI構造体の定義
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	int nType;			//種類
	bool bUse;			//使用しているかどうか
}Ui;
//====================================
//プロトタイプ宣言
//====================================
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void DeleteUi(int nCntUi);
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
Ui * GetUi(void);

#endif