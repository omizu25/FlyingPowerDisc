//**************************************************
//
// FPD制作 ( disc.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "disc.h"
#include "rectangle.h"
#include "color.h"
#include "game.h"
#include "player.h"

#include <assert.h>

//==================================================
// マクロ定義
//==================================================
#define DISC_SIZE			(75.0f)										// ディスクのサイズ
#define DISC_ROT_SPEED		(-0.25f)									// ディスクの回転速度
#define MAX_MOVE			(5.0f)										// 移動量の最大値
#define START_POS_X			(SCREEN_WIDTH * 0.5f)						// ディスクの始まりのXの位置
#define START_POS_Y			(SCREEN_HEIGHT - (DISC_SIZE * 0.5f))		// ディスクの始まりのYの位置

//==================================================
// 構造体
//==================================================
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXVECTOR3		move;			// 移動量
	int				nIdx;			// 矩形のインデックス
	float			fSize;			// サイズ
	bool			bUse;			// 使用してるかどうか
}Disc;

//==================================================
// スタティック変数
//==================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// テクスチャへのポインタ
static Disc						s_disc;					// ディスクの情報
static int						s_nPossPlayer;			// 次のディスクの始まりのプレイヤー

//==================================================
// プロトタイプ宣言
//==================================================
static void OffScreen(void);
static void NormalizeAngle(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitDisc(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/disc.png",
		&s_pTexture);

	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(MAX_MOVE, MAX_MOVE, 0.0f);
	s_disc.fSize = DISC_SIZE * 0.5f;

	// 矩形の設定
	s_disc.nIdx = SetRectangle(s_pTexture);

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitDisc(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateDisc(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_START:		// 開始状態
	{
		Player *pPlayer = GetPlayer();

		pPlayer += (s_nPossPlayer * 1);

		//float fRotMove, fRotDest, fRotDiff;

		////現在の移動方向(角度)
		//fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);

		////目的の移動方向(角度)
		//fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);

		//fRotDiff = fRotDest - fRotMove;		//目的の移動方向までの差分

		//if (fRotDiff >= D3DX_PI)
		//{
		//	fRotDiff = fRotDiff - (D3DX_PI * 2.0f);
		//}
		//else if (fRotDiff <= -D3DX_PI)
		//{
		//	fRotDiff = fRotDiff + (D3DX_PI * 2.0f);
		//}

		//fRotMove += fRotDiff * 0.075f;		//移動方向(角度)の補正

		//if (fRotMove >= D3DX_PI)
		//{
		//	fRotMove = fRotMove - (D3DX_PI * 2.0f);
		//}
		//else if (fRotMove <= -D3DX_PI)
		//{
		//	fRotMove = fRotMove + (D3DX_PI * 2.0f);
		//}

		//g_aBullet[nCntBullet].move.x = sinf(fRotMove) * 9.0f;
		//g_aBullet[nCntBullet].move.y = cosf(fRotMove) * 9.0f;
	}
	break;

	case GAMESTATE_RESTART:		// 再開始状態
		break;

	case GAMESTATE_NORMAL:		// 通常状態
		// 回転
		s_disc.rot.z += DISC_ROT_SPEED;

		// 角度の正規化
		NormalizeAngle();

		// 位置の更新
		s_disc.pos += s_disc.move;

		// 画面外
		OffScreen();

		// 矩形の回転する位置の設定
		SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
		break;

	case GAMESTATE_END:			// 終了状態

		break;

	case GAMESTATE_RESULT:		// リザルト状態

		break;

	case GAMESTATE_NONE:		// 何もしていない状態
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 画面外
//--------------------------------------------------
static void OffScreen(void)
{
	if (s_disc.pos.y >= SCREEN_HEIGHT - s_disc.fSize)
	{// 下
		s_disc.pos.y = SCREEN_HEIGHT - s_disc.fSize;
		s_disc.move.y *= -1.0f;
	}
	else if (s_disc.pos.y <= s_disc.fSize)
	{// 上
		s_disc.pos.y = s_disc.fSize;
		s_disc.move.y *= -1.0f;
	}

	if (s_disc.pos.x >= SCREEN_WIDTH - s_disc.fSize)
	{// 右
		s_disc.pos.x = SCREEN_WIDTH - s_disc.fSize;
		s_disc.move.x *= -1.0f;
	}
	else if (s_disc.pos.x <= s_disc.fSize)
	{// 左
		s_disc.pos.x = s_disc.fSize;
		s_disc.move.x *= -1.0f;
	}
}

//--------------------------------------------------
// 角度の正規化
//--------------------------------------------------
static void NormalizeAngle(void)
{
	if (s_disc.rot.z >= D3DX_PI)
	{// 3.14より大きい
		s_disc.rot.z -= D3DX_PI * 2.0f;
	}
	else if (s_disc.rot.z <= -D3DX_PI)
	{// -3.14より小さい
		s_disc.rot.z += D3DX_PI * 2.0f;
	}
}