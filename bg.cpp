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
#include "map.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_STAGE = 3;			// ステージの最大数
const float	MAX_WIDTH = 200.0f;		// 幅の最大値
const float	MAX_HEIGHT = 125.0f;	// 高さの最大値
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdxBG;				// 背景の矩形のインデックス
int	s_nIdxStage;			// ステージの矩形のインデックス
int	s_nIdxSet[MAXPLAYER];	// セット数
int	s_nIdxPoint[MAXPLAYER];	// ポイント数
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBG(void)
{
	{// 背景
		TEXTURE aTexture[MAX_STAGE];

		aTexture[0] = TEXTURE_BG;
		aTexture[1] = TEXTURE_BG2;
		aTexture[2] = TEXTURE_BG3;

		// 矩形の設定
		s_nIdxBG = SetRectangle(aTexture[GetSelectMap()]);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// ステージ
		TEXTURE aTexture[MAX_STAGE];

		aTexture[0] = TEXTURE_Stage01;
		aTexture[1] = TEXTURE_Stage02;
		aTexture[2] = TEXTURE_Stage03;

		// 矩形の設定
		s_nIdxStage = SetRectangle(aTexture[GetSelectMap()]);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.8f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxStage, pos, size);
	}

	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	{// セット数
		// 矩形の設定
		s_nIdxSet[0] = SetRectangle(TEXTURE_GAME_SetFrame01);
		s_nIdxSet[1] = SetRectangle(TEXTURE_GAME_SetFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxSet[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.9f;

		SetPosRectangle(s_nIdxSet[1], pos, size);
	}

	{// ポイント数
		// 矩形の設定
		s_nIdxPoint[0] = SetRectangle(TEXTURE_GAME_PtsFrame01);
		s_nIdxPoint[1] = SetRectangle(TEXTURE_GAME_PtsFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxPoint[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.7f;

		SetPosRectangle(s_nIdxPoint[1], pos, size);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBG(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdxStage);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		StopUseRectangle(s_nIdxSet[i]);
		StopUseRectangle(s_nIdxPoint[i]);
	}
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
