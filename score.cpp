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
#include "time.h"
#include "ui.h"
#include "game.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_UI_TIME = 120;			// UIの描画する時間
const float	NUMBER_WIDTH = 40.0f;		// 数の幅
const float	NUMBER_HEIGHT = 60.0f;		// 数の高さ
const float	NUMBER_UI_WIDTH = 200.0f;	// UIの数の幅
const float	NUMBER_UI_HEIGHT = 300.0f;	// UIの数の高さ
const float	START_POS_Y = 83.0f;		// 始まりのYの位置
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nSet[MAXPLAYER];		// セット数
int s_nPoint[MAXPLAYER];	// ポイント数
int s_nSetIdx[MAXPLAYER];	// セット数の値のインデックス
int s_nPointIdx[MAXPLAYER];	// ポイント数の値のインデックス
int s_nPointUI[MAXPLAYER];	// ポイント数のUI用の値のインデックス
int s_nUITime;				// UIを描画する時間
int s_nCntSet;				// セット数のカウント
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitScore(void)
{
	s_nCntSet = 0;
	s_nUITime = -1;

	D3DXVECTOR3 size = D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f);
	D3DXVECTOR3 sizeUI = D3DXVECTOR3(NUMBER_UI_WIDTH, NUMBER_UI_HEIGHT, 0.0f);

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		s_nSet[nPlayerNo] = 0;
		s_nPoint[nPlayerNo] = 0;

		D3DXVECTOR3 posSet = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
		D3DXVECTOR3 posPoint = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
		D3DXCOLOR color = GetColor(COLOR_WHITE);

		switch (nPlayerNo)
		{
		case 0:
			posSet.x = SCREEN_WIDTH * 0.17f;
			posPoint.x = SCREEN_WIDTH * 0.37f;
			color = GetColor(COLOR_RED);
			break;

		case 1:
			posSet.x = SCREEN_WIDTH * 0.97f;
			posPoint.x = SCREEN_WIDTH * 0.77f;
			color = GetColor(COLOR_BLUE);
			break;

		default:
			assert(false);
			break;
		}

		// 数の設定
		s_nSetIdx[nPlayerNo] = SetNumber(posSet, size, color, s_nSet[nPlayerNo]);
		s_nPointIdx[nPlayerNo] = SetNumber(posPoint, size, color, s_nPoint[nPlayerNo]);


		int nDigit = DigitNumber(s_nPoint[nPlayerNo]);
		D3DXVECTOR3 posUI = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 200.0f + (nPlayerNo * 600.0f) + (NUMBER_UI_WIDTH * (nDigit * 0.5f)), 450.0f, 0.0f);

		s_nPointUI[nPlayerNo] = SetNumber(posUI, sizeUI, color, s_nPoint[nPlayerNo]);
		
		// 数の描画するかどうか
		SetDrawNumber(s_nPointUI[nPlayerNo], false);
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
		StopUseNumber(s_nSetIdx[nPlayerNo]);
		StopUseNumber(s_nPointIdx[nPlayerNo]);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateScore(void)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		if (s_nPoint[nPlayerNo] >= GetPointRule())
		{// ポイント数が指定の値を越えた
			// ポイント数を0にする
			ZeroPointScore(0);
			ZeroPointScore(1);

			// 時間のリセット
			SetTime(GetTimeRule());

			// セット数の加算
			AddSetScore(nPlayerNo, 1);
		}
		if (s_nSet[nPlayerNo] >= GetSetRule())
		{// セット数が指定の値を越えた
			// モードの変更
			ChangeMode(MODE_TITLE);
			s_nCntSet = 0;
		}
	}

	if (s_nUITime >= 0)
	{
		s_nUITime++;

		if (s_nUITime >= MAX_UI_TIME)
		{
			// 数の描画するかどうか
			SetDrawNumber(s_nPointUI[0], false);
			SetDrawNumber(s_nPointUI[1], false);
			s_nUITime = -1;

			// ゲームの状態の設定
			SetGameState(GAMESTATE_START);
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

	s_nSet[nPlayerNo] = 0;

	// 数の変更
	ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);
}

//--------------------------------------------------
// ポイント数を0にする
//--------------------------------------------------
void ZeroPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nPoint[nPlayerNo] = 0;

	// 数の変更
	ChangeNumber(s_nPointIdx[nPlayerNo], s_nPoint[nPlayerNo]);
}

//--------------------------------------------------
// セット数を加算
//--------------------------------------------------
void AddSetScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nSet[nPlayerNo] += nValue;

	// 数の変更
	ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);

	switch (s_nCntSet)
	{// セット数に応じて表示する
	case 0:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 5);
		break;

	case 1:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 6);
		break;

	case 2:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 7);
		break;

	case 3:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 8);
		break;

	default:
		assert(false);
		break;
	}

	s_nCntSet++;	// セット数のカウントを増やす
}

//--------------------------------------------------
// 全てのセット数を加算
// 引数2  : int nValue / 加算する値
//--------------------------------------------------
void AllAddSetScore(int nValue)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		s_nSet[nPlayerNo] += nValue;

		// 数の変更
		ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);
	}

	switch (s_nCntSet)
	{// セット数に応じて表示する
	case 0:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 5);
		break;

	case 1:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 6);
		break;

	case 2:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 7);
		break;

	case 3:
		// セット数の表示
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 8);
		break;

	default:
		assert(false);
		break;
	}

	s_nCntSet++;	// セット数のカウントを増やす
}

//--------------------------------------------------
// ポイント数を加算
//--------------------------------------------------
void AddPointScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nPoint[nPlayerNo] += nValue;

	// 数の変更
	ChangeNumber(s_nPointIdx[nPlayerNo], s_nPoint[nPlayerNo]);

	if (s_nPoint[nPlayerNo] < GetPointRule())
	{// ポイント数が指定の値より下
		for (int i = 0; i < MAXPLAYER; i++)
		{
			// 数の変更
			ChangeNumber(s_nPointUI[i], s_nPoint[i]);

			// 数の描画するかどうか
			SetDrawNumber(s_nPointUI[i], true);
		}
		
		s_nUITime = 0;
	}
}

//--------------------------------------------------
// セット数の取得
//--------------------------------------------------
int GetSetScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return s_nSet[nPlayerNo];
}

//--------------------------------------------------
// ポイント数の取得
//--------------------------------------------------
int GetPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return s_nPoint[nPlayerNo];
}