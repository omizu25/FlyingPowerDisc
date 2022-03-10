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

//マクロ定義
#define MAX_RULE	(3)		//ルールの最大数

//スタティック変数
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	//テクスチャへのポインタ
static int	s_nIdx;	// 矩形のインデックス
static Rule s_Rule[MAX_RULE];	//構造体の取得
static int s_nTime;				//点滅の時間

//============================
// ルール選択画面の初期化
//============================
void InitRule(void)
{
	// 矩形の初期化
	InitRectangle();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//------------------------------
	//	テクスチャの読み込み
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Title_blue.png",
							  &s_pTexture);

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
	//テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 矩形を使うのを止める
	StopUseRectangle(s_nIdx);
}

//============================
// ルール選択画面の更新
//============================
void UpdateRule(void)
{
	s_nTime++;		//タイムの加算
	s_nTime %= 40;	//タイムの初期化

	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == true)
		{//使用しているなら
			//------------------------------
			//	テクスチャの点滅
			//------------------------------
			if (s_nTime >= 20)
			{
				// 矩形の色の設定
				SetColorRectangle(s_nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
			else
			{
				// 矩形の色の設定
				SetColorRectangle(s_nIdx, GetColor(COLOR_WHITE));
			}
		}
	}
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
			rule->fWidth = 50.0f;
			rule->fHeight = 25.0f;
			rule->bUse = true;

			// 矩形の設定
			s_nIdx = SetRectangle(s_pTexture);

			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(s_nIdx, pos, size);

			break;
		}
	}
}