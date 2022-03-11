//**************************************************
// 
// FPG制作 ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_		//このマクロ定義がされてなかったら
#define _TEXTURE_H_		//２重インクルード防止のマクロ定義

//==================================================
// 列挙型
//==================================================
typedef enum
{
	TEXTURE_BG = 0,
	TEXTURE_Disc,
	TEXTURE_Frame,
	TEXTURE_Title_blue,
	TEXTURE_TitleLight_red,
	TEXTURE_Game_Start,
	TEXTURE_Back_to_game,
	TEXTURE_Retry,
	TEXTURE_Back_to_title,
	TEXTURE_UI000,
	TEXTURE_UI001,
	TEXTURE_Select_Left,
	TEXTURE_Select_Right,
	TEXTURE_MAX,
}TEXTURE;

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadTexture(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTexture(void);

//--------------------------------------------------
// 取得
// 引数  : TEXTURE texture / 列挙型 種類
// 返値  : LPDIRECT3DTEXTURE9 / テクスチャ
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture);

#endif // !_TEXTURE_H_
