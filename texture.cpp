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
	/*↓ タイトル ↓*/
	"data/TEXTURE/TITLE/BG.png",				// タイトル画面の背景
	"data/TEXTURE/TITLE/Frame.png",				// タイトル画面の枠
	"data/TEXTURE/TITLE/Title_Rogo.png",		// タイトルロゴ
	"data/TEXTURE/TITLE/TitleLight_red.png",	// タイトルロゴの後光
	"data/TEXTURE/TITLE/Game_Start.png",		// ゲームスタートの文字
	"data/TEXTURE/TITLE/Rule_Select.png",		// ルール選択の文字
	"data/TEXTURE/TITLE/Map_Select.png",		// マップ選択の文字
	"data/TEXTURE/TITLE/Char_Select.png",		// キャラ選択の文字
	"data/TEXTURE/TITLE/Tutorial_Start.png",	// チュートリアルの文字
	"data/TEXTURE/TITLE/ThreePoints.png",		// 3ポイント
	"data/TEXTURE/TITLE/FivrPoints.png",		// 5ポイント

	/*↓ セレクト ↓*/
	"data/TEXTURE/SELECT/Select_Left.png",		// 左ボタン
	"data/TEXTURE/SELECT/Select_Right.png",		// 右ボタン
	"data/TEXTURE/SELECT/Select_Time.png",		// 制限時間
	"data/TEXTURE/SELECT/Select_Point.png",		// ポイント数
	"data/TEXTURE/SELECT/Select_Set.png",		// セット数
	"data/TEXTURE/SELECT/Select_BG.png",		// セレクト画面の背景
	"data/TEXTURE/SELECT/Map_Title.png",		// マップ選択
	"data/TEXTURE/SELECT/Rule_Title.png",		// ルール選択
	
	/*↓ ゲーム ↓*/
	"data/TEXTURE/GAME/Disc.png",				// ディスク
	"data/TEXTURE/GAME/Game_BG.png",			// ゲーム画面の背景
	"data/TEXTURE/GAME/Number_Yellow.png",		// 数字(黄色)
	"data/TEXTURE/GAME/Stage01.png",			// 1つ目のステージ

	/*↓ ゲーム中のUI ↓*/
	"data/TEXTURE/UI/Set1_1p.png",				// セット1(1pカラー)
	"data/TEXTURE/UI/Set2_1p.png",				// セット2(1pカラー)
	"data/TEXTURE/UI/Set3_1p.png",				// セット3(1pカラー)
	"data/TEXTURE/UI/Set1_2p.png",				// セット1(2pカラー)
	"data/TEXTURE/UI/Set2_2p.png",				// セット2(2pカラー)
	"data/TEXTURE/UI/Set3_2p.png",				// セット3(2pカラー)
	"data/TEXTURE/UI/SetFrame01.png",			// セットの枠(1pカラー)
	"data/TEXTURE/UI/SetFrame02.png",			// セットの枠(2pカラー)
	"data/TEXTURE/UI/PtsFrame01.png",			// ポイントの枠(1pカラー)
	"data/TEXTURE/UI/PtsFrame02.png",			// ポイントの枠(2pカラー)
	"data/TEXTURE/UI/UI000.png",				// プレイヤー1
	"data/TEXTURE/UI/UI001.png",				// プレイヤー2
	"data/TEXTURE/UI/Set1.png",					//セット1の表示
	"data/TEXTURE/UI/Set2.png",					//セット2の表示
	"data/TEXTURE/UI/Set3.png",					//セット3の表示
	"data/TEXTURE/UI/Set4.png",					//セット3の表示
	"data/TEXTURE/UI/Set5.png",					//セット3の表示

	/*↓ ポーズ ↓*/
	"data/TEXTURE/PAUSE/Pose_Title.png",		// ポーズ画面のタイトル
	"data/TEXTURE/PAUSE/Pose_Resume.png",		// 再開
	"data/TEXTURE/PAUSE/Pose_Quit.png",			// 終了

	/*↓ リザルト ↓*/
	"data/TEXTURE/RESULT/Win.png",				// 勝利
	"data/TEXTURE/RESULT/Lose.png",				// 敗北
	"data/TEXTURE/RESULT/Draw.png",				// 引き分け

	/*↓ プレイヤー ↓*/
	"data/TEXTURE/PLAYER/enemy000.png",			// カピバラ
	"data/TEXTURE/PLAYER/kitune.png",			// ゴン
	"data/TEXTURE/PLAYER/player000.png",		// スライム
	"data/TEXTURE/PLAYER/ghost.png",			// ゴースト
	"data/TEXTURE/PLAYER/zolbak.png",			// 地面

	/*↓ エフェクト ↓*/
	"data/TEXTURE/EFFECT/fire.png",			// 炎のエフェクト
	"data/TEXTURE/EFFECT/spin.png",			// ディスクのエフェクト
	"data/TEXTURE/EFFECT/tackle.png",		// タックルのエフェクト
	"data/TEXTURE/EFFECT/Goal.png",					// ゴールのエフェクト

	/*↓ その他 ↓*/
	"data/TEXTURE/Number_0To9.png",			// 数字
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
