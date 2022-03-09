//==================================================
//
// FPD制作 ( game.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include <assert.h>

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		// ゲームの状態
static int				s_nCounterState;					// 状態管理カウンター
static bool				s_bPause = false;					// ポーズ中かどうか [してる  : true してない  : false]

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	s_gameState = GAMESTATE_START;		// 開始状態に設定

	s_nCounterState = 0;				// カウンターの初期化

	s_bPause = false;					// ポーズ解除

	InitPlayer();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	UninitPlayer();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START, 0))
	{// Pキーが押されたかどうか
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{// ポーズ中
		return;
	}

	switch (s_gameState)
	{
	case GAMESTATE_NONE:		// 何もしていない状態
		
		break;

	case GAMESTATE_START:		// 開始状態
		UpdatePlayer();
		break;

	case GAMESTATE_NORMAL:		// 通常状態
		
		break;

	case GAMESTATE_END:			// 終了状態

		break;

	case GAMESTATE_RESULT:		// リザルト状態

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	if (s_bPause)
	{// ポーズ中
		return;
	}
	DrawPlayer();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;
	s_nCounterState = 0;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
GAMESTATE GetGameState(void)
{
	return s_gameState;
}

//--------------------------------------------------
// ポーズの有効無効設定
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}
