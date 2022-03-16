//**************************************************
//
// FPG制作 ( score.h )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "score.h"
#include "player.h"
#include "number.h"
#include "color.h"
#include "rule.h"
#include "mode.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float	NUMBER_WIDTH = 40.0f;	// 数の幅
const float	NUMBER_HEIGHT = 60.0f;	// 数の高さ
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	nSet[MAXPLAYER];		// セット数
int nPoint[MAXPLAYER];		// ポイント数
int nSetIdx[MAXPLAYER];		// セット数の値のインデックス
int nPointIdx[MAXPLAYER];	// ポイント数の値のインデックス
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitScore(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f);

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		nSet[nPlayerNo] = 0;
		nPoint[nPlayerNo] = 0;

		D3DXVECTOR3 posSet = D3DXVECTOR3(0.0f, NUMBER_HEIGHT, 0.0f);
		D3DXVECTOR3 posPoint = D3DXVECTOR3(0.0f, NUMBER_HEIGHT, 0.0f);

		switch (nPlayerNo)
		{
		case 0:
			posSet.x = SCREEN_WIDTH * 0.2f;
			posPoint.x = SCREEN_WIDTH * 0.3f;
			break;

		case 1:
			posSet.x = SCREEN_WIDTH * 0.8f;
			posPoint.x = SCREEN_WIDTH * 0.7f;
			break;

		default:
			assert(false);
			break;
		}

		// 数の設定
		nSetIdx[nPlayerNo] = SetNumber(posSet, size, GetColor(COLOR_BLUE), nSet[nPlayerNo]);
		nPointIdx[nPlayerNo] = SetNumber(posPoint, size, GetColor(COLOR_RED), nPoint[nPlayerNo]);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitScore(void)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		// 数を使うのを止める
		StopUseNumber(nSetIdx[nPlayerNo]);
		StopUseNumber(nPointIdx[nPlayerNo]);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateScore(void)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		if (nPoint[nPlayerNo] >= GetPointRule())
		{// ポイント数が指定の値を越えた
			// ポイント数を0にする
			ZeroPointScore(nPlayerNo);
		
			// セット数の加算
			AddSetScore(nPlayerNo, 1);
		}

		if (nSet[nPlayerNo] >= GetSetRule())
		{// セット数が指定の値を越えた
			// セット数を0にする
			ZeroSetScore(nPlayerNo);
		
			// モードの変更
			ChangeMode(MODE_TITLE);
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawScore(void)
{

}

//--------------------------------------------------
// セット数を0にする
//--------------------------------------------------
void ZeroSetScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	nSet[nPlayerNo] = 0;

	// 数の変更
	ChangeNumber(nSetIdx[nPlayerNo], nSet[nPlayerNo]);
}

//--------------------------------------------------
// ポイント数を0にする
//--------------------------------------------------
void ZeroPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	nPoint[nPlayerNo] = 0;

	// 数の変更
	ChangeNumber(nPointIdx[nPlayerNo], nPoint[nPlayerNo]);
}

//--------------------------------------------------
// セット数を加算
//--------------------------------------------------
void AddSetScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	nSet[nPlayerNo] += nValue;

	// 数の変更
	ChangeNumber(nSetIdx[nPlayerNo], nSet[nPlayerNo]);
}

//--------------------------------------------------
// ポイント数を加算
//--------------------------------------------------
void AddPointScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	nPoint[nPlayerNo] += nValue;

	// 数の変更
	ChangeNumber(nPointIdx[nPlayerNo], nPoint[nPlayerNo]);
}

//--------------------------------------------------
// セット数の取得
//--------------------------------------------------
int GetSetScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return nSet[nPlayerNo];
}

//--------------------------------------------------
// ポイント数の取得
//--------------------------------------------------
int GetPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return nPoint[nPlayerNo];
}