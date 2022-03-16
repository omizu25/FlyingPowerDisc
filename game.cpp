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
	//タイム初期化
	InitTime();

	// 矩形の初期化
	InitRectangle();

	// 数の初期化
	InitNumber();

	// スコアの初期化
	InitScore();

	//かべ初期化
	InitWall();

	// ディスクの初期化
	InitDisc();

	//エフェクト初期化
	InitEffect();

	//プレイヤーの初期化
	InitPlayer();

	//UIの初期化
	InitUi();

	// ポーズの初期化
	InitPause();

	// メニューの初期化
	InitMenu();

	//UIの配置			置く座標			横幅	縦幅	タイプ			拡大率とフェード	テクスチャの種類
	SetUi(D3DXVECTOR3 (50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 0, D3DXVECTOR3(1.0f, 1.0f, 0.0f),0);	
	SetUi(D3DXVECTOR3 (SCREEN_WIDTH - 50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f), 1);
	//左から出てくるやつ
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 100.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 300.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 500.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	//右から出てくるやつ																				
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 100.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 300.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 500.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	//セット数																							
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 300.0f, 0.0f), 400.0f, 100.0f,4, D3DXVECTOR3(1.0f, 0.0f, 0.0f),4);
	//選択した時間を表示
	//SetTime(GetTimeRule());
	SetTime(99);

	s_gameState = GAMESTATE_START;	// 開始状態に設定

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

	//タイムの終了
	UninitTime();

	// 数の終了
	UninitNumber();

	// スコアの終了
	UninitScore();

	//かべの終了
	UninitWall();

	// 矩形の終了
	UninitRectangle();

	// ディスクの終了
	UninitDisc();

	//エフェクト終了
	UninitEffect();

	// プレイヤーの終了
	UninitPlayer();

	//UIの終了
	UninitUi();

	// ポーズの終了
	UninitPause();

	// メニューの終了
	UninitMenu();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{// Pキーが押された
		// ポーズのリセット
		ResetPause();

		s_bPause = !s_bPause;

		if (s_bPause)
		{// ポーズしている
			// メニューの設定
			SetPause();
		}
	}

	if (s_bPause)
	{// ポーズしている
		// ポーズの更新
		UpdatePause();

		// メニューの更新
		UpdateMenu();
		return;
	}

	// ディスクの更新
	UpdateDisc();

	//エフェクト更新
	UpdateEffect();

	//UIの更新(まだ何もしてない)
	UpdateUi();

	//かべの更新
	UpdateWall();

	//タイムの更新
	UpdateTime();

	// スコアの更新
	UpdateScore();

	// プレイヤーの更新
	UpdatePlayer();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	//かべの描画
	//DrawWall();

	//エフェクト更新
	DrawEffect();

	// 矩形の描画
	DrawRectangle();

	//タイムの描画
	DrawTime();

	//UIの描画
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
