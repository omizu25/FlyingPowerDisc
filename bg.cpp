//**************************************************
//
// FPD制作 ( bg.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "bg.h"
#include "rectangle.h"
#include "color.h"
#include "texture.h"
#include "player.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float	MAX_WIDTH = 200.0f;		// 幅の最大値
const float	MAX_HEIGHT = 125.0f;	// 高さの最大値
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	nIdxBG;					// 背景の矩形のインデックス
int	nIdxStage;				// ステージの矩形のインデックス
int	nIdxSet[MAXPLAYER];		// セット数
int	nIdxPoint[MAXPLAYER];	// ポイント数
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBG(void)
{
	{// 背景
		// 矩形の設定
		nIdxBG = SetRectangle(TEXTURE_NONE);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(nIdxBG, pos, size);
	}

	{// ステージ
		// 矩形の設定
		nIdxStage = SetRectangle(TEXTURE_Stage01);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.8f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(nIdxStage, pos, size);
	}

	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	{// セット数
		// 矩形の設定
		nIdxSet[0] = SetRectangle(TEXTURE_GAME_SetFrame01);
		nIdxSet[1] = SetRectangle(TEXTURE_GAME_SetFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(nIdxSet[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.9f;

		SetPosRectangle(nIdxSet[1], pos, size);
	}

	{// ポイント数
		// 矩形の設定
		nIdxPoint[0] = SetRectangle(TEXTURE_GAME_PtsFrame01);
		nIdxPoint[1] = SetRectangle(TEXTURE_GAME_PtsFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(nIdxPoint[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.7f;

		SetPosRectangle(nIdxPoint[1], pos, size);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBG(void)
{
	// 使うのを止める
	StopUseRectangle(nIdxBG);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBG(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBG(void)
{
	/* 矩形で描画してます */
}
