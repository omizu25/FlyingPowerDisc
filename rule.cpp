//================================
//
// ルール選択画面の処理
// Author:Sato Teruto
//
//================================

//インクルード
#include "rule.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"

//マクロ定義
#define MAX_RULE	(3)				//ルールの最大数
#define MAX_FLASH	(80)			//点滅の往復時間
#define HALF_FLASH	(MAX_FLASH / 2)	//点滅の切り替え時間

//スタティック変数
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	//テクスチャへのポインタ
static Rule s_Rule[MAX_RULE];	//構造体の取得
static int s_nTime;				//点滅の時間
static int s_nSelect;			//選択中の番号

//============================
// ルール選択画面の初期化
//============================
void InitRule(void)
{
	// 矩形の初期化
	InitRectangle();

	// テクスチャの取得
	s_pTexture = GetTexture(TEXTURE_Title_blue);

	//------------------------------
	//	構造体の初期化
	//------------------------------
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		rule->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		rule->fWidth = 0.0f;	//幅
		rule->fHeight = 0.0f;	//高さ
		rule->bUse = false;		//使用していない状態
	}
}

//============================
// ルール選択画面の終了
//============================
void UninitRule(void)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		// 矩形を使うのを止める
		StopUseRectangle(rule->nIdx);
	}
}

//============================
// ルール選択画面の更新
//============================
void UpdateRule(void)
{
	s_nTime++;				//タイムの加算
	s_nTime %= MAX_FLASH;	//タイムの初期化

	//選択番号の切り替え
	int nNumber = ChangeSelect();

	//テクスチャの点滅
	FlashTexture(nNumber);
}

//============================
// ルール選択画面の描画
//============================
void DrawRule(void)
{
	// 矩形の描画
	DrawRectangle();
}

//============================
// ルール選択画面の設定
//============================
void SetRule(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == false)
		{//使用していないなら
			//構造体の設定
			rule->pos = pos;
			rule->fWidth = 200.0f;
			rule->fHeight = 100.0f;
			rule->bUse = true;

			// 矩形の設定
			rule->nIdx = SetRectangle(s_pTexture);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(rule->nIdx, pos, size);

			break;
		}
	}
}

//============================
// テクスチャの点滅
//============================
void FlashTexture(int nNumber)
{
	if (s_nTime >= HALF_FLASH)
	{
		// 矩形の色の設定
		SetColorRectangle(s_Rule[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// 矩形の色の設定
		SetColorRectangle(s_Rule[nNumber].nIdx, GetColor(COLOR_WHITE));
	}
}

//============================
// 選択番号の切り替え
//============================
int ChangeSelect(void)
{
	// 矩形の色の設定
	SetColorRectangle(s_Rule[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W))
	{//Wキーが押されたとき
		if (s_nSelect >= 1 && s_nSelect <= MAX_RULE)
		{//0未満にならないなら
			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_S))
	{//Sキーが押されたとき
		if (s_nSelect >= 0 && s_nSelect < (MAX_RULE - 1))
		{//最大数を超えならないなら
			s_nSelect++;
		}
	}

	return s_nSelect;
}