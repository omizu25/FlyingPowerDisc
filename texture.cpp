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
namespace
{
const char *s_FileName[] =
{// テクスチャのパス
	//タイトル
	"data/TEXTURE/TITLE/BG.png",			//タイトル画面の背景
	"data/TEXTURE/TITLE/Frame.png",			//タイトル画面の枠
	"data/TEXTURE/TITLE/Title_blue.png",	//タイトルロゴ
	"data/TEXTURE/TITLE/TitleLight_red.png",//タイトルロゴの後光
	"data/TEXTURE/TITLE/Game_Start.png",	//ゲームスタートの文字
	"data/TEXTURE/TITLE/ThreePoints.png",	//3ポイント
	"data/TEXTURE/TITLE/FivrPoints.png",	//5ポイント

	//セレクト
	"data/TEXTURE/SELECT/Select_Left.png",	//左ボタン
	"data/TEXTURE/SELECT/Select_Right.png",	//右ボタン
	"data/TEXTURE/SELECT/Select_Time.png",	//制限時間
	"data/TEXTURE/SELECT/Select_Point.png",	//ポイント数
	"data/TEXTURE/SELECT/Select_Set.png",	//セット数
	"data/TEXTURE/SELECT/Select_BG.png",	//セレクト画面の背景
	"data/TEXTURE/SELECT/Map_Title.png",	//マップ選択
	"data/TEXTURE/SELECT/Rule_Title.png",	//ルール選択
	
	//ゲーム
	"data/TEXTURE/GAME/Disc.png",			//ディスク
	"data/TEXTURE/GAME/Game_BG.png",		//ゲーム画面の背景
	"data/TEXTURE/GAME/Number_Yellow.png",	//数字(黄色)
	"data/TEXTURE/GAME/Stage01.png",		//1つ目のステージ

	//ゲーム中のUI
	"data/TEXTURE/GAME/Set1_1p.png",		//セット1(1pカラー)
	"data/TEXTURE/GAME/Set2_1p.png",		//セット2(1pカラー)
	"data/TEXTURE/GAME/Set3_1p.png",		//セット3(1pカラー)
	"data/TEXTURE/GAME/Set1_2p.png",		//セット1(2pカラー)
	"data/TEXTURE/GAME/Set2_2p.png",		//セット2(2pカラー)
	"data/TEXTURE/GAME/Set3_2p.png",		//セット3(2pカラー)
	"data/TEXTURE/GAME/SetFrame01.png",		//セットの枠(1pカラー)
	"data/TEXTURE/GAME/SetFrame02.png",		//セットの枠(2pカラー)
	"data/TEXTURE/GAME/PtsFrame01.png",		//ポイントの枠(1pカラー)
	"data/TEXTURE/GAME/PtsFrame02.png",		//ポイントの枠(2pカラー)
	"data/TEXTURE/UI/UI000.png",			//UI-0
	"data/TEXTURE/UI/UI001.png",			//UI-1

	//ポーズ
	"data/TEXTURE/PAUSE/Pose_Title.png",	//ポーズ画面のタイトル
	"data/TEXTURE/PAUSE/Pose_Resume.png",	//再開
	"data/TEXTURE/PAUSE/Pose_Quit.png",		//終了

	//その他
	"data/TEXTURE/Number_0To9.png",			//数字
};
}// namespaceはここまで

static_assert(sizeof(s_FileName) / sizeof(s_FileName[0]) == TEXTURE_MAX, "aho");

//==================================================
// スタティック変数
//==================================================
namespace
{
LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];
}// namespaceはここまで

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
	if (texture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return NULL;
	}

	assert(texture >= 0 && texture < TEXTURE_MAX);

	return s_pTexture[texture];
}
