//**************************************************
// 
// FPG制作 ( texture.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定数
//==================================================
static const char *s_FileName[] =
{// 色の配列
	"data/TEXTURE/BG.png",
	"data/TEXTURE/Disc.png",
	"data/TEXTURE/Frame.png",
	"data/TEXTURE/Title_blue.png",
	"data/TEXTURE/TitleLight_red.png",
	"data/TEXTURE/Game_Start.png",
	"data/TEXTURE/Back_to_game.png",
	"data/TEXTURE/Retry.png",
	"data/TEXTURE/Back_to_title.png",
	"data/TEXTURE/UI000.png",
	"data/TEXTURE/UI001.png",
};

static_assert(sizeof(s_FileName) / sizeof(s_FileName[0]) == TEXTURE_MAX, "aho");

//==================================================
// スタティック変数
//==================================================
static LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadTexture(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTexture(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture)
{
	assert(texture >= 0 && texture < TEXTURE_MAX);

	return s_pTexture[texture];
}
