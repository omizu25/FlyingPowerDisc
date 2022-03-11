//================================
//
// ルール選択画面のヘッダー
// Author:Sato Teruto
//
//================================
#ifndef _RULE_H_	//このマクロが定義されてなかったら
#define _RULE_H_	//2重インクルード防止のマクロ定義

//インクルード
#include "main.h"

//スイッチ構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているか
	int	nIdx;			// 矩形のインデックス
}Switch;

//ルール構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているか
	int	nIdx;			// 矩形のインデックス
}Rule;

//プロトタイプ宣言
void InitRule(void);
void UninitRule(void);
void UpdateRule(void);
void DrawRule(void);
void SetRule(D3DXVECTOR3 pos);
void SetSwitchLeft(D3DXVECTOR3 pos);
void SetSwitchRight(D3DXVECTOR3 pos);
void FlashTexture(int nNumber);
void AddRule(int nNumber);
void SubRule(int nNumber);
int ChangeSelect(void);

#endif