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
#include "texture.h"

#include <assert.h>

//==================================================
// マクロ定義
//==================================================
#define MAX_LIGHT			(2)				// 後光の最大数
#define TITLE_POS_Y			(200.0f)		// タイトルのYの位置
#define TITLE_WIDTH			(900.0f)		// タイトルの幅
#define TITLE_HEIGHT		(200.0f)		// タイトルの高さ
#define LIGHT_WIDTH			(280.0f)		// ライトの幅
#define LIGHT_HEIGHT		(280.0f)		// ライトの高さ
#define CHANGE_SPEED		(0.005f)		// 収縮の速度
#define CHANGE_AMOUNT		(0.1f)			// 変化量
#define MEDIAN_LENGTH		(0.95f)			// 長さの中央値
#define MAX_ROTATION		(0.003f)		// 回転の最大値
#define DECREASE_SIZE		(0.6f)			// サイズの減少割合
#define DECREASE_SPEED		(0.5f)			// 速度の減少割合
#define MENU_WIDTH			(540.0f)		// メニューの幅
#define MENU_HEIGHT			(64.0f)			// メニューの高さ

//==================================================
// 列挙型
//==================================================
typedef enum
{
	MENU_GAME = 0,		// ゲーム
	MENU_RULE,			// ルール
	MENU_MAX
}MENU;

//==================================================
// 構造体
//==================================================
typedef struct
{
	D3DXVECTOR3		rot;				// 向き
	float			fWidth;				// 幅
	float			fHeight;			// 高さ
	float			fSpeed;				// 速度
	int				nIdx;				// 矩形のインデックス
}Light;

//==================================================
// スタティック変数
//==================================================
static int			s_nIdxBG;				// 背景の矩形のインデックス
static int			s_nIdx;					// 矩形のインデックス
static Light		s_light[MAX_LIGHT];		// 後光の情報
static int			s_nTime;				// 時間
static int			s_nSelectMenu;			// 選ばれているメニュー
static int			s_nIdxUseMenu;			// 使っているメニューの番号

//==================================================
// プロトタイプ宣言
//==================================================
static void UpdateLight(void);
static void Input(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTitle(void)
{
	// 矩形の初期化
	InitRectangle();

	s_nTime = 0;
	s_nSelectMenu = 0;

	// 矩形の設定
	s_nIdxBG = SetRectangle(GetTexture(TEXTURE_BG));

	// 矩形の設定
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		pLight->nIdx = SetRectangle(GetTexture(TEXTURE_TitleLight_red));
	}

	// 矩形の設定
	s_nIdx = SetRectangle(GetTexture(TEXTURE_Title_blue));

	{// 背景
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// ロゴ
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdx, pos, size);
	}

	{// 後光
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			Light *pLight = &s_light[i];

			pLight->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (i == 0)
			{//	奥
				pLight->fWidth = LIGHT_WIDTH;
				pLight->fHeight = LIGHT_HEIGHT;
				pLight->fSpeed = MAX_ROTATION * DECREASE_SPEED;
			}
			else if (i == 1)
			{// 手前
				pLight->fWidth = LIGHT_WIDTH * DECREASE_SIZE;
				pLight->fHeight = LIGHT_HEIGHT * DECREASE_SIZE;
				pLight->fSpeed = MAX_ROTATION;
			}

			// 矩形の回転する位置の設定
			SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, pLight->fWidth, pLight->fHeight);

			D3DXVECTOR2 texU = D3DXVECTOR2(0.0f + (i * 1.0f), 1.0f + (i * -1.0f));
			D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

			// 矩形のテクスチャ座標の設定
			SetTexRectangle(pLight->nIdx, texU, texV);
		}
	}

	// メニューの初期化
	InitMenu();

	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = SCREEN_WIDTH * 0.25f;
	menu.fRight = SCREEN_WIDTH * 0.75f;
	menu.fTop = SCREEN_HEIGHT * 0.5f;
	menu.fBottom = SCREEN_HEIGHT;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	menu.pTexture[MENU_GAME] = GetTexture(TEXTURE_Game_Start);
	menu.pTexture[MENU_RULE] = menu.pTexture[MENU_GAME];

	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = GetColor(COLOR_WHITE);
	Frame.pTexture = GetTexture(TEXTURE_Frame);
	
	// メニューの設定
	s_nIdxUseMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTitle(void)
{
	// 矩形の終了
	UninitRectangle();

	// メニューの終了
	UninitMenu();

	// 使うのを止める
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		StopUseRectangle(pLight->nIdx);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTitle(void)
{
	// 後光
	UpdateLight();

	// 入力
	Input();

	// メニューの更新
	UpdateMenu();
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
// 後光
//--------------------------------------------------
static void UpdateLight(void)
{
	s_nTime++;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
	float fCurve = sinf((s_nTime * CHANGE_SPEED) * (D3DX_PI * 2.0f));

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		if (i == 0)
		{// 奥
			pLight->rot.z += pLight->fSpeed;
		}
		else if (i == 1)
		{// 手前
			pLight->rot.z += -pLight->fSpeed;
		}

		float fRatio = (fCurve * CHANGE_AMOUNT) + MEDIAN_LENGTH;
		float fWidth = pLight->fWidth * fRatio;
		float fHeight = pLight->fHeight * fRatio;

		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, fWidth, fHeight);
	}
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
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
		case MENU_GAME:				// ゲーム
			ChangeMode(MODE_GAME);
			break;

		case MENU_RULE:				// ゲーム
			ChangeMode(MODE_RULE);
			break;

		default:
			assert(false);
			break;
		}

		// 選択肢の決定
		DecisionOption();
	}
}
