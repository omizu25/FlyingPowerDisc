//**************************************************
//
// タイトル画面 ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "rectangle.h"
#include "title.h"
#include "color.h"
#include "sound.h"
#include "texture.h"
#include "cursor.h"
#include "player.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_LIGHT = 4;				// ライトの最大数
const int	HALF_LIGHT = MAX_LIGHT / 2;	// ライトの半分
const float	TITLE_WIDTH = 600.0f;		// タイトルの幅
const float	TITLE_HEIGHT = 600.0f;		// タイトルの高さ
const float	MENU_WIDTH = 300.0f;		// メニューの幅
const float	MENU_HEIGHT = 80.0f;		// メニューの高さ
const float	CURSOR_SIZE = 50.0f;		// カーソルのサイズ
const int	MAX_FLASH_RED = 80;						// 点滅にかかる時間(赤)
const int	HALF_FLASH_RED = MAX_FLASH_RED / 2;		// 点滅にかかる時間(赤)の半分
const int	MAX_FLASH_BLUE = 120;					// 点滅にかかる時間(青)
const int	HALF_FLASH_BLUE = 80;	// 点滅にかかる時間(青)の半分


typedef enum
{
	MENU_GAME = 0,	// ゲーム
	MENU_RULE,		// ルール
	MENU_MAP,		// マップ選択
	MENU_PLAYER,	// プレイヤー選択
	MENU_TUTORIAL,	// チュートリアル
	MENU_MAX
}MENU;

//ライト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているか
	int	nIdx;			// 矩形のインデックス
}Light;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdxBG;				// 背景の矩形のインデックス
int	s_nIdx;					// 矩形のインデックス
int	s_nSelectMenu;			// 選ばれているメニュー
int	s_nIdxUseMenu;			// 使っているメニューの番号
int	s_nIdxCursor;			// カーソルの配列のインデックス
int s_nFlashTimeRed;		// 点滅用の時間(赤)
int s_nFlashTimeBlue;		// 点滅用の時間(青)
Light s_Light[MAX_LIGHT];	// ライト構造体の取得
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
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_TITLE2);

	// 矩形の初期化
	InitRectangle();

	// ライト構造体の初期化
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		light->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		light->fWidth = 0.0f;	//幅
		light->fHeight = 0.0f;	//高さ
		light->bUse = false;	//使用していない状態
	}

	s_nSelectMenu = 0;

	{// 背景
		// 矩形の設定
		s_nIdxBG = SetRectangle(TEXTURE_BG);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	// ライトの設定
	SetLightRed(D3DXVECTOR3(80.0f, 100.0f, 0.0f), TEXTURE_Light_Red);		//左上赤
	SetLightRed(D3DXVECTOR3(330.0f, 550.0f, 0.0f), TEXTURE_Light_Red);		//右下赤
	SetLightBlue(D3DXVECTOR3(545.0f, 350.0f, 0.0f), TEXTURE_Light_Blue);	//右上青
	SetLightBlue(D3DXVECTOR3(70.0f, 450.0f, 0.0f), TEXTURE_Light_Blue);		//左下青

	{// ロゴ
		// 矩形の設定
		s_nIdx = SetRectangle(TEXTURE_Title_Logo);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdx, pos, size);
	}

	{// プレイヤー
		// プレイヤーの初期化
		InitPlayer();

		Player *pPlayer = GetPlayer();

		// プレイヤーの設定
		SetPlayer(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.815f, 0.0f), pPlayer->nType, true, PLAYERSIZE * 1.4f);
		
		pPlayer++;

		// プレイヤーの設定
		SetPlayer(D3DXVECTOR3(SCREEN_WIDTH * 0.475f, SCREEN_HEIGHT * 0.815f, 0.0f), pPlayer->nType, false, PLAYERSIZE * 1.4f);
	}

	{// メニュー
		// メニューの初期化
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = SCREEN_WIDTH * 0.55f;
		menu.fRight = SCREEN_WIDTH * 0.95f;
		menu.fTop = 0.0f;
		menu.fBottom = SCREEN_HEIGHT;
		menu.fWidth = MENU_WIDTH;
		menu.fHeight = MENU_HEIGHT;
		menu.bSort = true;

		menu.texture[MENU_GAME] = TEXTURE_Game_Start;
		menu.texture[MENU_RULE] = TEXTURE_Rule_Select;
		menu.texture[MENU_MAP] = TEXTURE_Map_Select;
		menu.texture[MENU_PLAYER] = TEXTURE_Char_Select;
		menu.texture[MENU_TUTORIAL] = TEXTURE_Tutorial_Start;

		FrameArgument Frame;
		Frame.bUse = true;
		Frame.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		Frame.texture = TEXTURE_NONE;

		// メニューの設定
		s_nIdxUseMenu = SetMenu(menu, Frame);
	}

	{// カーソル
		// カーソル初期化
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MENU_MAX;
		cursor.fPosX = SCREEN_WIDTH * 0.595f;
		cursor.fTop = 0.0f;
		cursor.fBottom = SCREEN_HEIGHT;
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
// 終了
//--------------------------------------------------
void UninitTitle(void)
{
	//サウンド停止
	StopSound();

	// 矩形の終了
	UninitRectangle();

	// メニューの終了
	UninitMenu();

	// カーソルの終了
	UninitCursor();

	// プレイヤーの終了
	UninitPlayer();

	// 使うのを止める
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTitle(void)
{
	// 入力
	Input();

	// メニューの更新
	UpdateMenu();

	// カーソルの更新
	UpdateCursor();

	// ライトの点滅
	FlashLight();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTitle(void)
{
	// 矩形の描画
	DrawRectangle();
}

//--------------------------------------------------
// 赤ライトの設定
//--------------------------------------------------
void SetLightRed(D3DXVECTOR3 pos,TEXTURE texture)
{
	for (int nCnt = 0; nCnt < HALF_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		if (light->bUse == false)
		{//使用していないなら
		 //構造体の設定
			light->pos = pos;
			light->fWidth = 100.0f;
			light->fHeight = 100.0f;
			light->bUse = true;

			// 矩形の設定
			light->nIdx = SetRectangle(texture);

			D3DXVECTOR3 size = D3DXVECTOR3(light->fWidth, light->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(light->nIdx, pos, size);

			break;
		}
	}
}

//--------------------------------------------------
// 青ライトの設定
//--------------------------------------------------
void SetLightBlue(D3DXVECTOR3 pos, TEXTURE texture)
{
	for (int nCnt = HALF_LIGHT; nCnt < MAX_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		if (light->bUse == false)
		{//使用していないなら
		 //構造体の設定
			light->pos = pos;
			light->fWidth = 100.0f;
			light->fHeight = 100.0f;
			light->bUse = true;

			// 矩形の設定
			light->nIdx = SetRectangle(texture);

			D3DXVECTOR3 size = D3DXVECTOR3(light->fWidth, light->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(light->nIdx, pos, size);

			break;
		}
	}
}

//--------------------------------------------------
// ライトの点滅
//--------------------------------------------------
void FlashLight(void)
{
	s_nFlashTimeRed++;						//タイムの加算
	s_nFlashTimeRed %= MAX_FLASH_RED;		//タイムの初期化

	s_nFlashTimeBlue++;		//タイムの加算
	//タイムの初期化
	if (s_nFlashTimeBlue % MAX_FLASH_BLUE == 0)
	{
		s_nFlashTimeBlue = 40;
	}

	//------------------------------
	// 赤ライトの点滅
	//------------------------------
	if (s_nFlashTimeRed >= HALF_FLASH_RED)
	{
		SetColorRectangle(s_Light[0].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		SetColorRectangle(s_Light[1].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		SetColorRectangle(s_Light[0].nIdx, GetColor(COLOR_WHITE));
		SetColorRectangle(s_Light[1].nIdx, GetColor(COLOR_WHITE));
	}

	//------------------------------
	// 青ライトの点滅
	//------------------------------
	if (s_nFlashTimeBlue >= HALF_FLASH_BLUE)
	{
		SetColorRectangle(s_Light[2].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		SetColorRectangle(s_Light[3].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		SetColorRectangle(s_Light[2].nIdx, GetColor(COLOR_WHITE));
		SetColorRectangle(s_Light[3].nIdx, GetColor(COLOR_WHITE));
	}
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

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{// Wキーが押されたかどうか
		PlaySound(SOUND_LABEL_SELECT);

		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

		// カーソルの位置の変更
		ChangePosCursor(s_nIdxCursor, s_nSelectMenu);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{// Sキーが押されたかどうか
		PlaySound(SOUND_LABEL_SELECT);

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
		PlaySound(SOUND_LABEL_ENTER);
		switch (s_nSelectMenu)
		{
		case MENU_GAME:	// ゲーム
			ChangeMode(MODE_GAME);
			break;

		case MENU_RULE:	// ルール
			ChangeMode(MODE_RULE);
			break;

		case MENU_MAP:	// マップ選択
			ChangeMode(MODE_MAP);
			break;

		case MENU_PLAYER:	// プレイヤー選択
			ChangeMode(MODE_PLAYER);
			break;

		case MENU_TUTORIAL:	// チュートリアル
			ChangeMode(MODE_TUTORIAL);
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
