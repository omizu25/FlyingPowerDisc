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
#include "texture.h"

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
static Disc		s_disc;				// ディスクの情報
static int		s_nPossPlayer;		// 次のディスクの始まりのプレイヤー

//==================================================
// プロトタイプ宣言
//==================================================
static void UpdateStart(void);
static void UpdateReset(void);
static void UpdateNormal(void);
static void Reflect(void);
static void NormalizeAngle(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitDisc(void)
{
	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.fSize = DISC_SIZE * 0.5f;

	// 矩形の設定
	s_disc.nIdx = SetRectangle(GetTexture(TEXTURE_Disc));

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitDisc(void)
{
	// 使うのを止める
	StopUseRectangle(s_disc.nIdx);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateDisc(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_START:		// 開始状態
		UpdateStart();
		break;

	case GAMESTATE_NORMAL:		// 通常状態
		UpdateNormal();
		break;

	case GAMESTART_RESET:		// リセット状態
		UpdateReset();
		break;

	case GAMESTATE_END:			// 終了状態
	case GAMESTATE_RESULT:		// リザルト状態

		break;

	case GAMESTATE_NONE:		// 何もしていない状態
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawDisc(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 開始状態
//--------------------------------------------------
static void UpdateStart(void)
{
	Player *pPlayer = GetPlayer();

	pPlayer += (s_nPossPlayer * 1);

	float fRotMove, fRotDest, fRotDiff;

	//現在の移動方向(角度)
	fRotMove = atan2f(s_disc.move.x, s_disc.move.y);

	//目的の移動方向(角度)
	fRotDest = atan2f(pPlayer->pos.x - s_disc.pos.x, pPlayer->pos.y - s_disc.pos.y);

	fRotDiff = fRotDest - fRotMove;		//目的の移動方向までの差分

	if (fRotDiff >= D3DX_PI)
	{
		fRotDiff = fRotDiff - (D3DX_PI * 2.0f);
	}
	else if (fRotDiff <= -D3DX_PI)
	{
		fRotDiff = fRotDiff + (D3DX_PI * 2.0f);
	}

	fRotMove += fRotDiff;		//移動方向(角度)の補正

	if (fRotMove >= D3DX_PI)
	{
		fRotMove = fRotMove - (D3DX_PI * 2.0f);
	}
	else if (fRotMove <= -D3DX_PI)
	{
		fRotMove = fRotMove + (D3DX_PI * 2.0f);
	}

	s_disc.move.x = sinf(fRotMove) * MAX_MOVE;
	s_disc.move.y = cosf(fRotMove) * MAX_MOVE;
}

//--------------------------------------------------
// 通常状態
//--------------------------------------------------
static void UpdateNormal(void)
{
	// 回転
	s_disc.rot.z += DISC_ROT_SPEED;

	// 角度の正規化
	NormalizeAngle();

	// 位置の更新
	s_disc.pos += s_disc.move;

	// 反射
	Reflect();

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// リセット状態
//--------------------------------------------------
static void UpdateReset(void)
{
	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// 反射
//--------------------------------------------------
static void Reflect(void)
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
		// ゲームの状態の設定
		SetGameState(GAMESTART_RESET);
	}
	else if (s_disc.pos.x <= s_disc.fSize)
	{// 左
		// ゲームの状態の設定
		SetGameState(GAMESTART_RESET);
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
