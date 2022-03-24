//================================
//
// ルール選択画面のヘッダー
// Author:Sato Teruto
//
//================================
#ifndef _MAP_H_	//このマクロが定義されてなかったら
#define _MAP_H_	//2重インクルード防止のマクロ定義

//インクルード
#include "main.h"

//背景構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているか
	int	nIdx;			// 矩形のインデックス
}MAPBG;

//ルール構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているか
	int	nIdx;			// 矩形のインデックス
}Map;

//プロトタイプ宣言
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos);
void SetBGMap(D3DXVECTOR3 pos);
void FlashTextureMap(int nNumber);
int ChangeSelectMap(void);
int GetSelectMap(void);
#endif
