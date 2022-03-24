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
#include "cursor.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float		MENU_WIDTH = 300.0f;					// メニューの幅
const float		MENU_HEIGHT = 100.0f;					// メニューの高さ
const float		MENU_LEFT = SCREEN_WIDTH * 0.3f;		// メニューの左端
const float		MENU_RIGHT = SCREEN_WIDTH * 0.7f;		// メニューの右端
const float		MENU_TOP = SCREEN_HEIGHT * 0.35f;		// メニューの上端
const float		MENU_BOTTOM = SCREEN_HEIGHT * 0.85f;	// メニューの下端
const D3DXCOLOR	BG_COLOR(0.0f, 0.0f, 0.0f, 0.6f);		// 背景の色
const D3DXCOLOR	FRAME_COLOR(0.5f, 0.5f, 1.0f, 1.0f);	// フレームの色
const float		CURSOR_SIZE = 75.0f;					// カーソルのサイズ

typedef enum
{
	MENU_GAME = 0,	// ゲーム
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
int	s_nIdxCursor;	// カーソルの配列のインデックス
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
	// カーソルの終了
	UninitCursor();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePause(void)
{
	// 入力
	Input();

	// カーソルの更新
	UpdateCursor();
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
	{// メニュー
		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = MENU_LEFT;
		menu.fRight = MENU_RIGHT;
		menu.fTop = MENU_TOP;
		menu.fBottom = MENU_BOTTOM;
		menu.fWidth = MENU_WIDTH;
		menu.fHeight = MENU_HEIGHT;
		menu.bSort = true;

		menu.texture[MENU_GAME] = TEXTURE_Pose_Resume;
		menu.texture[MENU_TITLE] = TEXTURE_Pose_Quit;

		FrameArgument Frame;
		Frame.bUse = true;
		Frame.col = FRAME_COLOR;
		Frame.texture = TEXTURE_NONE;

		// メニューの設定
		s_nIdxMenu = SetMenu(menu, Frame);
	}

	{// カーソル
		// カーソル初期化
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MENU_MAX;
		cursor.fPosX = SCREEN_WIDTH * 0.35f;
		cursor.fTop = MENU_TOP;
		cursor.fBottom = MENU_BOTTOM;
		cursor.fWidth = CURSOR_SIZE;
		cursor.fHeight = CURSOR_SIZE;
		cursor.texture = TEXTURE_Disc;
		cursor.nSelect = s_nSelectMenu;
		cursor.bRotation = true;

		// カーソルの設定
		s_nIdxCursor = SetCursor(cursor);
	}
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetPause(void)
{
	// メニューのリセット
	ResetMenu(s_nIdxMenu);

	// カーソルのリセット
	ResetCursor(s_nIdxCursor);
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

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_NUMPAD5) ||
		GetJoypadTrigger(JOYKEY_UP))
	{// Wキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

		// カーソルの位置の変更
		ChangePosCursor(s_nIdxCursor, s_nSelectMenu);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_NUMPAD2) ||
		GetJoypadTrigger(JOYKEY_DOWN))
	{// Sキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

		// カーソルの位置の変更
		ChangePosCursor(s_nIdxCursor, s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAME:		// ゲーム
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
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
