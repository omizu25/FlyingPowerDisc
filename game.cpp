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

	// UIの初期化
	InitUi();

	// スコアの初期化
	InitScore();

	// メニューの初期化
	InitMenu();

	// ポーズの初期化
	InitPause();

	// UIの配置			置く座標			横幅	縦幅	タイプ			拡大率とフェード	テクスチャの種類
	SetUi(D3DXVECTOR3 (50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 0, D3DXVECTOR3(1.0f, 1.0f, 0.0f),0);	
	SetUi(D3DXVECTOR3 (SCREEN_WIDTH - 50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f), 1);
	// 左から出てくるやつ
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 250.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 450.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 650.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// 右から出てくるやつ																				
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 250.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 450.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 650.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// セット数																							
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 100.0f,4, D3DXVECTOR3(1.0f, 0.0f, 0.0f),4);
	
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

	// UIの終了
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
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	// 矩形の描画
	DrawRectangle();

	// かべの描画
	//DrawWall();

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
