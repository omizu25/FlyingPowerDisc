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
#include "mode.h"
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
LPDIRECT3DTEXTURE9 s_pTexture[MAX_CHARACTER];
//============================
// キャラ選択画面の初期化
//============================
void InitCharacter(void)
{
	LoadFileSet("data\\txt\\Status.txt");

	// 矩形の初期化
	InitRectangle();

	//Player
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

			if (player->nType > 3)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//Dキーが押されたとき
		 //数値の加算
			player->nType--;
			if (player->nType < 0)
			{
				player->nType = 3;
				
			}
		}

		//テクスチャ更新
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);
	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_LEFT))
		{//Aキーが押されたとき
		 //数値の減算
			player->nType++;
			if (player->nType > 3)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_NUMPAD3) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//Dキーが押されたとき
		 //数値の加算
			player->nType--;
			
			if (player->nType < 0)
			{
				player->nType = 3;
			}
		}

		//テクスチャ更新
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{	//ゲーム選択画面行く
		ChangeMode(MODE_GAME);		
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

//----------------------------
//Playerのステータス読み込み
//----------------------------
void LoadFileSet(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");
	int number = 0;

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//これのあとコメント
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//＝読み込むやつ
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//デバイスの取得
				pDevice = GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTexture[Num_Tex]);
				Num_Tex++;

			}
			
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

