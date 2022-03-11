//**************************************************
//
// FPG制作 ( pause.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "input.h"
#include "fade.h"
#include "menu.h"
#include "game.h"
#include "mode.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float		MENU_WIDTH = 500.0f;					// メニューの幅
const float		MENU_HEIGHT = 150.0f;					// メニューの高さ
const float		MENU_LEFT = SCREEN_WIDTH * 0.25f;		// メニューの左端
const float		MENU_RIGHT = SCREEN_WIDTH * 0.75f;		// メニューの右端
const float		MENU_TOP = SCREEN_HEIGHT * 0.1f;		// メニューの上端
const float		MENU_BOTTOM = SCREEN_HEIGHT * 0.9f;		// メニューの下端
const D3DXCOLOR	BG_COLOR(0.0f, 0.0f, 0.0f, 0.6f);		// 背景の色
const D3DXCOLOR	FRAME_COLOR(0.5f, 1.0f, 0.5f, 1.0f);	// フレームの色

typedef enum
{
	MENU_GAME = 0,	// ゲーム
	MENU_RETRY,		// リトライ
	MENU_TITLE,		// タイトル
	MENU_MAX
}MENU;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nSelectMenu;	// 選ばれているメニュー
int	s_nIdxMenu;		// 使っているメニューの番号
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void Input(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPause(void)
{
	s_nSelectMenu = 0;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPause(void)
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePause(void)
{
	// 入力
	Input();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPause(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetPause(void)
{
	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = MENU_LEFT;
	menu.fRight = MENU_RIGHT;
	menu.fTop = MENU_TOP;
	menu.fBottom = MENU_BOTTOM;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	menu.texture[MENU_GAME] = TEXTURE_Back_to_game;
	menu.texture[MENU_RETRY] = TEXTURE_Retry;
	menu.texture[MENU_TITLE] = TEXTURE_Back_to_title;
	
	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = FRAME_COLOR;
	Frame.texture = TEXTURE_NONE;

	// メニューの設定
	s_nIdxMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetPause(void)
{
	// メニューのリセット
	ResetMenu(s_nIdxMenu);
}

namespace
{
//--------------------------------------------------
// 入力
//--------------------------------------------------
void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W))
	{// Wキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

	}
	else if (GetKeyboardTrigger(DIK_S))
	{// Sキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAME:		// ゲーム
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
			break;

		case MENU_RETRY:	// リトライ
			ChangeMode(MODE_GAME);
			break;

		case MENU_TITLE:	// タイトル
			ChangeMode(MODE_TITLE);
			break;

		default:
			assert(false);
			break;
		}

		// 選択肢の決定
		DecisionOption();
	}
}
}// namespaceはここまで
