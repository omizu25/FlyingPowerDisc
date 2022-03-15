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
	D3DXVECTOR3 scale;	//拡大率
	float nCntTime;		//出現時間
	float fWidth;		//幅
	float fHeight;		//高さ
	int nType;			//種類
	int nTexture;		//テクスチャ番号
	bool bUse;			//使用しているかどうか
	bool bSwitch;		//出てくるか消えていくか
}Ui;
//====================================
//プロトタイプ宣言
//====================================
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, D3DXVECTOR3 scale, int nTexture);
Ui * GetUi(void);

#endif