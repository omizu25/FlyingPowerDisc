//================================
//
// キャラ選択画面の処理
// Author:HAMADA
//
//================================

//------------------------------
// インクルード
//------------------------------
#include "playerselect.h"
#include "player.h"
#include "input.h"
#include "rectangle.h"
#include "main.h"
#include <stdio.h>
#include <assert.h>

//==================================================
// 定義
//==================================================

 //------------------------------
 // マクロ定義
 //------------------------------
#define MAX_CHARACTER	(4)					//キャラの最大数


 //------------------------------
 // スタティック変数
 //------------------------------
int s_nSelect;

//============================
// キャラ選択画面の初期化
//============================
void InitCharacter(void)
{


	// 矩形の初期化
	InitRectangle();

	InitPlayer();

	SetPlayer(D3DXVECTOR3((float)PLAYERSIZE_X , (float)SCREEN_HEIGHT * 0.6f, 0.0f), 0, true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.6f, 0.0f), 1, false);
}

//============================
// キャラ選択画面の終了
//============================
void UninitCharacter(void)
{

	UninitPlayer();
	// 矩形の終了
	UninitRectangle();
}

//============================
// キャラ選択画面の更新
//============================
void UpdateCharacter(void)
{
	{
		Player*player = GetPlayer();

		if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
		{//Aキーが押されたとき
		 //数値の減算
			player->nType++;
			player->nIdx++;
			if (player->nType > 4)
			{
				player->nType = 0;
			}

		}

		if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//Dキーが押されたとき
		 //数値の加算

			player->nType--;
			player->nIdx--;
			if (player->nType < -1)
			{
				player->nType = 4;
				
			}
		}

	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_LEFT))
		{//Aキーが押されたとき
		 //数値の減算
			player->nType++;
			if (player->nType > 4)
			{
				player->nType = 0;
			}

		}

		if (GetKeyboardTrigger(DIK_NUMPAD3) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//Dキーが押されたとき
		 //数値の加算
			player->nType--;
			if (player->nType < -1)
			{
				player->nType = 4;
			}

		}
		D3DXVECTOR2 texU(1.0f, 0.0f);
		D3DXVECTOR2 texV(0.0f, 1.0f);

		// 矩形のテクスチャ座標の設定
		SetTexRectangle(player->nIdx, texU, texV);
	}
}

//============================
// キャラ選択画面の描画
//============================
void DrawCharacter(void)
{
	// 矩形の描画
	DrawRectangle();
}



