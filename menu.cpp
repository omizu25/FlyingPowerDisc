//**************************************************
// 
// FPG制作 ( menu.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "input.h"
#include "menu.h"
#include "fade.h"
#include "color.h"
#include "rectangle.h"

//==================================================
// マクロ定義
//==================================================
#define MAX_MENU					(16)		// メニューの最大数
#define NORMAL_BLINK_SPEED			(0.01f)		// 通常時の点滅速度
#define DECISION_BLINK_SPEED		(0.1f)		// 決定時の点滅速度
#define MIN_ALPHA					(0.6f)		// α値の最小値

//==================================================
// 構造体
//==================================================

/*↓ 選択肢 ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXCOLOR				col;			// 色
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
	int						nIdx;			// 矩形のインデックス
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
}Option;

/*↓ メニュー ↓*/

typedef struct
{
	D3DXVECTOR3				pos;					// 位置
	LPDIRECT3DTEXTURE9		pTexture;				// テクスチャ
	Option					Option[MAX_OPTION];		// 選択肢の情報
	int						nNumUse;				// 使用数
	int						nIdx;					// 矩形のインデックス
	float					fWidth;					// 幅
	float					fHeight;				// 高さ
	float					fInterval;				// 選択肢の間隔
	float					fBlinkSpeed;			// 点滅速度
	bool					bFrame;					// 枠がいるかどうか [ true : いる false : いらない ]
	bool					bUse;					// 使用しているかどうか
}Menu;

//==================================================
// スタティック変数
//==================================================
static Menu		s_aMenu[MAX_MENU];		// メニューの情報
static int		s_nIdxMenu;				// 選ばれているメニューの番号
static int		s_nIdxOption;			// 選ばれている選択肢の番号
static int		s_nAlphaTime;			// α値変更用の時間

//==================================================
// プロトタイプ宣言
//==================================================
static void ChangeColor(Menu *pMenu);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMenu(void)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;

	// メモリのクリア
	memset(s_aMenu, 0, sizeof(s_aMenu));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMenu(void)
{
	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		if (pMenu->bFrame)
		{// 枠を使っている
			// 使うのを止める
			StopUseRectangle(pMenu->nIdx);
		}

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			// 使うのを止める
			StopUseRectangle(pOption->nIdx);
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}
	
	// 色の変更
	ChangeColor(pMenu);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame)
{
	int nIdx = 0;
	Menu *pMenu = nullptr;

	for (nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pMenu->nNumUse = menu.nNumUse;
		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;
		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;
		pMenu->pTexture = Frame.pTexture;
		pMenu->bFrame = Frame.bUse;

		if (menu.bSort)
		{// 縦
			pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		}
		else
		{// 横
			pMenu->fInterval = pMenu->fWidth / (menu.nNumUse + 1);
		}

		pMenu->bUse = true;

		s_nIdxMenu = nIdx;
		s_nIdxOption = 0;

		if (Frame.bUse)
		{// 枠がいる
			// 矩形の設定
			pMenu->nIdx = SetRectangle(Frame.pTexture);

			fPosX = menu.fLeft + (pMenu->fWidth * 0.5f);
			fPosY = menu.fTop + (pMenu->fHeight * 0.5f);
			D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
			D3DXVECTOR3 size = D3DXVECTOR3(pMenu->fWidth, pMenu->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(pMenu->nIdx, pos, size);

			// 矩形の色の設定
			SetColorRectangle(pMenu->nIdx, Frame.col);
		}

		break;
	}

	for (int j = 0; j < pMenu->nNumUse; j++)
	{
		Option *pOption = &pMenu->Option[j];

		if (menu.bSort)
		{// 縦
			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
		}
		else
		{// 横
			pOption->pos = D3DXVECTOR3(menu.fLeft + (pMenu->fInterval * (j + 1)), pMenu->pos.y, 0.0f);
		}
		
		pOption->col = GetColor(COLOR_WHITE);
		pOption->fWidth = menu.fWidth;
		pOption->fHeight = menu.fHeight;
		pOption->pTexture = menu.pTexture[j];

		// 矩形の設定
		pOption->nIdx = SetRectangle(menu.pTexture[j]);

		D3DXVECTOR3 size = D3DXVECTOR3(menu.fWidth, menu.fHeight, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(pOption->nIdx, pOption->pos, size);

		// 矩形の色の設定
		SetColorRectangle(pOption->nIdx, pOption->col);
	}

	return nIdx;
}

//--------------------------------------------------
// 選択肢の色の初期化
//--------------------------------------------------
void InitColorOption(void)
{
	Option *pOption = &s_aMenu[s_nIdxMenu].Option[s_nIdxOption];

	// 矩形の色の設定
	SetColorRectangle(pOption->nIdx, GetColor(COLOR_WHITE));
}

//--------------------------------------------------
// 選択肢の変更
//--------------------------------------------------
void ChangeOption(int nIdx)
{
	s_nIdxOption = nIdx;
}

//--------------------------------------------------
// 選択肢の決定
//--------------------------------------------------
void DecisionOption(void)
{
	s_aMenu[s_nIdxMenu].fBlinkSpeed = DECISION_BLINK_SPEED;
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;
	
	Menu *pMenu = &s_aMenu[nIdx];

	if (pMenu->bFrame)
	{// 枠を使っている
		// 使うのを止める
		StopUseRectangle(pMenu->nIdx);
	}

	for (int i = 0; i < pMenu->nNumUse; i++)
	{
		Option *pOption = &pMenu->Option[i];

		// 使うのを止める
		StopUseRectangle(pOption->nIdx);
	}

	pMenu->bUse = false;
}

//--------------------------------------------------
// 色の変更
//--------------------------------------------------
static void ChangeColor(Menu *pMenu)
{
	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nIdxOption];

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));
	float fAlpha = (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);

	// 矩形の色の設定
	SetColorRectangle(pOption->nIdx, pOption->col);
}
