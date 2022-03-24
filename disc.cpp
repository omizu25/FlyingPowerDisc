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
#include "effect.h"
#include "utility.h"
#include "score.h"
#include "wall.h"
#include "sound.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	EFFECT_TIME = 1;									// エフェクトの出すタイム
const float	DISC_SIZE = 75.0f;									// ディスクのサイズ
const float	DISC_ROT_SPEED = -0.25f;							// ディスクの回転速度
const float	MAX_MOVE = 10.0f;									// 移動量の最大値
const float	START_POS_X = SCREEN_WIDTH * 0.5f;					// ディスクの始まりのXの位置
const float	START_POS_Y = SCREEN_HEIGHT - (DISC_SIZE * 0.5f);	// ディスクの始まりのYの位置
const float	GOAL_TOP = 350.0f;									// ゴールの上端
const float	GOAL_BOTTOM = 500.0f;								// ゴールの下端
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Disc	s_disc;			// ディスクの情報
int		s_nPossPlayer;	// 次のディスクの始まりのプレイヤー
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void UpdateStart(void);
void UpdateNormal(void);
void Goal(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitDisc(void)
{
	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.nThrow = s_nPossPlayer ^ 1;
	s_disc.bHave = false;

	// 矩形の設定
	s_disc.nIdx = SetRectangle(TEXTURE_Disc);

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitDisc(void)
{
	//サウンド停止
	StopSound();

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
	case GAMESTATE_NONE:	// 何もしていない状態
		break;

	case GAMESTATE_START:	// 開始状態
		UpdateStart();
		break;

	case GAMESTATE_NORMAL:	// 通常状態
		UpdateNormal();
		break;

	case GAMESTATE_RESET:	// リセット状態
		break;

	case GAMESTATE_END:		// 終了状態
	case GAMESTATE_RESULT:	// リザルト状態

		break;

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
// 取得
//--------------------------------------------------
Disc *GetDisc(void)
{
	return &s_disc;
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetDisc(void)
{
	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// 次に始めるプレイヤーの設定
//--------------------------------------------------
void SetPossDisc(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAXPLAYER);

	s_nPossPlayer = nIdx;
	s_disc.nThrow = s_nPossPlayer ^ 1;
}

//==================================================
// スタティック関数
//==================================================
namespace
{
//--------------------------------------------------
// 開始状態
//--------------------------------------------------
void UpdateStart(void)
{
	Player *pPlayer = GetPlayer();

	pPlayer += (s_nPossPlayer * 1);

	D3DXVECTOR3 posDest = pPlayer->pos;

	switch (s_nPossPlayer)
	{
	case 0:
		posDest.x += PLAYERSIZE * 0.5f;
		break;
		
	case 1:
		posDest.x += -PLAYERSIZE * 0.5f;
		break;

	default:
		posDest = pPlayer->pos;
		assert(false);
		break;
	}

	// ホーミング
	Homing(&s_disc.pos, s_disc.pos, posDest, MAX_MOVE);

	// 回転
	s_disc.rot.z += DISC_ROT_SPEED;

	// 角度の正規化
	NormalizeAngle(&s_disc.rot.z);

	// 位置の更新
	s_disc.pos += s_disc.move;
	
	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);

	if (CollisionPlayer(&s_disc, DISC_SIZE, s_disc.nThrow ^ 1))
	{// プレイヤーとディスクの当たり判定
		// ゲームの状態の設定
		SetGameState(GAMESTATE_NORMAL);

		s_nPossPlayer = s_nPossPlayer ^ 1;
	}
}

//--------------------------------------------------
// 通常状態
//--------------------------------------------------
void UpdateNormal(void)
{
	if (s_disc.bHave)
	{// プレイヤーが持っている
		return;
	}

	/*↓ プレイヤーが持ってない ↓*/

	// 回転
	s_disc.rot.z += DISC_ROT_SPEED;

	// 角度の正規化
	NormalizeAngle(&s_disc.rot.z);

	// 位置の更新
	s_disc.posOld = s_disc.pos;
	s_disc.pos += s_disc.move;
	
	//------------------------
	//このまま使うと変だから一定間隔で使うといいかも
	//------------------------
	s_disc.nEffect++;

	if (s_disc.nEffect % EFFECT_TIME == 0)
	{// 一定間隔
		// エフェクトの設定
		SetEffect(s_disc.pos, GetColor(COLOR_WHITE), EFFECTSTATE_SPIN, 30, 100.0f, false);
	}
	
	// プレイヤーとディスクの当たり判定
	CollisionPlayer(&s_disc, DISC_SIZE, s_disc.nThrow ^ 1);

	if (CollisionWall(&s_disc.pos, &s_disc.posOld))
	{// 壁とディスクの当たり判定
		s_disc.move.y *= -1.0f;
	}

	// ゴール
	Goal();

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// ゴール
//--------------------------------------------------
void Goal(void)
{
	float fRadius = DISC_SIZE * 0.5f;

	if (s_disc.pos.x >= SCREEN_WIDTH - fRadius)
	{// 右
		if ((s_disc.pos.y >= GOAL_TOP) && s_disc.pos.y <= GOAL_BOTTOM)
		{
			// ポイント数の加算
			AddPointScore(0, 5);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT5, 100, 50.0f, true);
		}
		else
		{
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT3, 100, 50.0f, true);
			// ポイント数の加算
			AddPointScore(0, 3);
		}
	
		//歓声
		PlaySound(SOUND_LABEL_KANSEI);
		// ゲームの状態の設定
		SetGameState(GAMESTATE_RESET);

		// リセット
		ResetDisc();

		// 次に始めるプレイヤーの設定
		SetPossDisc(1);
	}
	else if (s_disc.pos.x <= fRadius)
	{// 左
		if ((s_disc.pos.y >= GOAL_TOP) && s_disc.pos.y <= GOAL_BOTTOM)
		{
			// ポイント数の加算
			AddPointScore(1, 5);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT5, 100, 50.0f, false);
		}
		else
		{
			// ポイント数の加算
			AddPointScore(1, 3);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT3, 100, 50.0f, false);
		}
		
		//歓声
		PlaySound(SOUND_LABEL_KANSEI);
		// ゲームの状態の設定
		SetGameState(GAMESTATE_RESET);

		// リセット
		ResetDisc();

		// 次に始めるプレイヤーの設定
		SetPossDisc(0);
	}
}
} // namespaceはここまで
