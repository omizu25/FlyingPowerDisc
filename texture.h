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
	//タイトル
	TEXTURE_BG = 0,				//タイトル画面の背景
	TEXTURE_Frame,				//タイトル画面の枠
	TEXTURE_Title_blue,			//タイトルロゴ
	TEXTURE_TitleLight_red,		//タイトルロゴの後光
	TEXTURE_Game_Start,			//ゲームスタートの文字

	//セレクト
	TEXTURE_Select_Left,		//左ボタン
	TEXTURE_Select_Right,		//右ボタン
	TEXTURE_Select_Time,		//制限時間
	TEXTURE_Select_Point,		//ポイント数
	TEXTURE_Select_Set,			//セット数
	TEXTURE_Select_BG,			//セレクト画面の背景
	TEXTURE_Map_Title,			//マップ選択
	TEXTURE_Rule_Title,			//ルール選択

	//ゲーム
	TEXTURE_Disc,				//ディスク
	TEXTURE_Game_BG,			//ゲーム画面の背景
	TEXTURE_UI000,				//UI-0
	TEXTURE_UI001,				//UI-1

	//ポーズ
	TEXTURE_Pose_Title,			//ポーズ画面のタイトル
	TEXTURE_Pose_Resume,		//再開
	TEXTURE_Pose_Quit,			//終了

	//その他
	TEXTURE_Number_0To9,		//数字
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
