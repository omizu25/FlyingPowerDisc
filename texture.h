//**************************************************
// 
// FPG制作 ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

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
	TEXTURE_Pose_Title,
	TEXTURE_Pose_Resume,
	TEXTURE_Pose_Quit,
	TEXTURE_Back_to_title,
	TEXTURE_UI000,
	TEXTURE_UI001,
	TEXTURE_Select_Left,
	TEXTURE_Select_Right,
	TEXTURE_Number_0To9,
	TEXTURE_Select_Time,
	TEXTURE_Select_Point,
	TEXTURE_Select_Set,
	TEXTURE_Select_BG,
	TEXTURE_GAME_BG,
	TEXTURE_MAX,
	TEXTURE_NONE,
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
