//**************************************************
//
// FPD制作 ( game.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "rectangle.h"
#include "disc.h"
#include "ui.h"
#include "pause.h"
#include "menu.h"
#include "wall.h"
#include "effect.h"
#include "time.h"
#include "rule.h"
#include "number.h"
#include "score.h"
#include "bg.h"
#include "result.h"
#include "cursor.h"

#include <assert.h>

//==================================================
// スタティック変数
//==================================================
namespace
{
GAMESTATE	s_gameState = GAMESTATE_NONE;	// ゲームの状態
int			s_nCounterState;				// 状態管理カウンター
bool		s_bPause = false;				// ポーズ中かどうか [してる  : true してない  : false]
}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	// ルールの読み込み
	LoadRule();

	// 矩形の初期化
	InitRectangle();

	// 背景の初期化
	InitBG();

	// タイムの初期化
	InitTime();

	// 数の初期化
	InitNumber();

	// かべの初期化
	InitWall();

	// エフェクトの初期化
	InitEffect();

	// ディスクの初期化
	InitDisc();

	// プレイヤーの初期化
	InitPlayer();

	// プレイヤーの設定
	Player *pPlayer = GetPlayer();
	SetPlayer(D3DXVECTOR3((float)PLAYER_POS_X + PLAYERSIZE, (float)SCREEN_HEIGHT * 0.6f, 0.0f), pPlayer->nType, true, PLAYERSIZE);
	pPlayer++;
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYER_POS_X - PLAYERSIZE, (float)SCREEN_HEIGHT * 0.6f, 0.0f), pPlayer->nType, false, PLAYERSIZE);

	// UIの初期化
	InitUi();

	// スコアの初期化
	InitScore();

	// メニューの初期化
	InitMenu();

	// カーソル初期化
	InitCursor();

	// ポーズの初期化
	InitPause();

	// リザルトの初期化
	InitResult();

	// UIの配置			置く座標			横幅	縦幅	タイプ			拡大率とフェード	テクスチャの種類
	SetUi(D3DXVECTOR3 (125.0f, 350.0f, 0.0f), 150.0f, 50.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f),0);	
	SetUi(D3DXVECTOR3 (SCREEN_WIDTH - 125.0f, 350.0f, 0.0f), 150.0f, 50.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f), 1);
	// 左から出てくるやつ
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 250.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 450.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 650.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// 右から出てくるやつ																				
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 250.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 450.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 650.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// セット数																							
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f,4, D3DXVECTOR3(1.0f, 0.0f, 0.0f),4);
	
	//サウンド開始
	PlaySound(SOUND_LABEL_BGM);

	// 選択した時間を表示
	SetTime(GetTimeRule());

	s_gameState = GAMESTATE_NONE;	// 何もしていない状態に設定

	s_nCounterState = 0;			// カウンターの初期化

	s_bPause = false;				// ポーズ解除
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	// サウンドの停止
	StopSound();

	// 背景の終了
	UninitBG();

	// タイムの終了
	UninitTime();

	// 数の終了
	UninitNumber();

	// スコアの終了
	UninitScore();

	// かべの終了
	UninitWall();

	// 矩形の終了
	UninitRectangle();

	// ディスクの終了
	UninitDisc();

	// エフェクトの終了
	UninitEffect();

	// プレイヤーの終了
	UninitPlayer();

	// リザルトの終了
	UninitResult();

	// UIの終了
	UninitUi();

	// ポーズの終了
	UninitPause();

	// メニューの終了
	UninitMenu();

	// カーソルの終了
	UninitCursor();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{// Pキーが押された
		if (s_gameState == GAMESTATE_NORMAL)
		{// 通常状態の時
			s_bPause = !s_bPause;

			if (s_bPause)
			{// ポーズしてる
				// メニューの設定
				SetPause();
			}
			else
			{// ポーズしてない
				// ポーズのリセット
				ResetPause();
			}
		}
	}

	if (s_bPause)
	{// ポーズしている
		// ポーズの更新
		UpdatePause();

		// メニューの更新
		UpdateMenu();

		// カーソルの更新
		UpdateCursor();
		return;
	}

	// ディスクの更新
	UpdateDisc();

	// エフェクトの更新
	UpdateEffect();

	// UIの更新
	UpdateUi();

	// かべの更新
	UpdateWall();

	// タイムの更新
	UpdateTime();

	// スコアの更新
	UpdateScore();

	// プレイヤーの更新
	UpdatePlayer();

	// リザルトの更新
	UpdateResult();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	// 矩形の描画
	DrawRectangle();

	// タイムの描画
	DrawTime();

	// UIの描画
	DrawUi();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	assert(state >= 0 && state < GAMESTATE_MAX);

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
