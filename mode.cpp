//**************************************************
//
// FPD制作 ( mode.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "fade.h"
#include "game.h"
#include "mode.h"
#include "map.h"
#include "title.h"
#include "rule.h"
#include "playerselect.h"
#include "tutorial.h"
#include "texture.h"

#include <assert.h>

//==================================================
// スタティック変数
//==================================================
namespace
{
MODE	s_mode = MODE_NONE;		// 現在のモード
MODE	s_modeNext = MODE_NONE;	// 次のモード
}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMode(void)
{

}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMode(void)
{
	// タイトルの終了
	UninitTitle();

	//マップ選択の終了
	UninitMap();

	// ゲームの終了
	UninitGame();

	// ルール選択画面の終了
	UninitRule();

	// キャラ選択画面の終了
	UninitCharacter();

	// チュートリアルの終了
	UninitTutorial();

	// ルール選択画面のリセットのセーブ
	ResetSaveRule();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:	// タイトル
		UpdateTitle();
		break;

	case MODE_MAP:		// マップ
		UpdateMap();
		break;

	case MODE_RULE:		// ルール
		UpdateRule();
		break;

	case MODE_GAME:		// ゲーム
		UpdateGame();
		break;

	case MODE_PLAYER:	// プレイヤー選択
		UpdateCharacter();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		UpdateTutorial();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:	// タイトル
		DrawTitle();
		break;

	case MODE_MAP:		// マップ
		DrawMap();
		break;

	case MODE_RULE:		// ルール
		DrawRule();
		break;

	case MODE_GAME:		// ゲーム
		DrawGame();
		break;

	case MODE_PLAYER:	// プレイヤー選択
		DrawCharacter();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		DrawTutorial();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// 次のモードが決まってない
		return;
	}

	if (GetFade() == FADE_NONE)
	{// 何もしていない状態なら
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// フェードイン状態じゃない
		return;
	}

	switch (s_mode)
	{// 現在のモードの終了
	case MODE_TITLE:	// タイトル
		UninitTitle();
		break;

	case MODE_MAP:		// マップ
		UninitMap();
		break;

	case MODE_RULE:		// ルール
		UninitRule();
		break;

	case MODE_GAME:		// ゲーム
		UninitGame();
		break;

	case MODE_PLAYER:	// プレイヤー選択
		UninitCharacter();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		UninitTutorial();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}

	switch (s_modeNext)
	{// 次のモードの初期化
	case MODE_TITLE:	// タイトル
		InitTitle();
		SetLightRed(D3DXVECTOR3(80.0f, 150.0f, 0.0f), TEXTURE_Light_Red);		//左上赤
		SetLightRed(D3DXVECTOR3(330.0f, 550.0f, 0.0f), TEXTURE_Light_Red);		//右下赤
		SetLightBlue(D3DXVECTOR3(560.0f, 350.0f, 0.0f), TEXTURE_Light_Blue);	//右上青
		SetLightBlue(D3DXVECTOR3(70.0f, 450.0f, 0.0f), TEXTURE_Light_Blue);		//左下青
		break;

	case MODE_MAP:		// マップ
		InitMap();
		SetBGMap(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		SetMap(D3DXVECTOR3(200.0f, 300.0f, 0.0f));
		SetMap(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
		SetMap(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
		break;

	case MODE_RULE:		// ルール
		LoadRule();
		InitRule();

		// ルール選択画面の設定
		SetBG(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 150.0f, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 350.0f, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 550.0f, 0.0f));
		break;

	case MODE_GAME:		// ゲーム
		InitGame();
		break;

	case MODE_PLAYER:	// プレイヤー選択
		InitCharacter();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		InitTutorial();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_mode = s_modeNext;	// 現在の画面(モード)を切り替える
	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// 変更
//--------------------------------------------------
void ChangeMode(MODE modeNext)
{
	assert(modeNext >= 0 && modeNext < MODE_MAX);

	s_modeNext = modeNext;
}
