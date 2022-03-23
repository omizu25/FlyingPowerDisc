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
#include "menu.h"
#include "color.h"
#include "gauge.h"

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // マクロ定義
 //------------------------------
#define MAX_CHARACTER	(5)					//キャラの最大数
#define START_POS_X		(50.0f)				//スタート位置、調整用
#define GAUGE_WIDTH		(50.0f)				//ゲージの幅
#define GAUGE_HEIGHT	(45.0f)				//ゲージの高さ

 //------------------------------
 // 列挙型
 //------------------------------
typedef enum
{
	MENU_FOX = 0,	// キツネ
	MENU_CAPYBARA,	// カピバラ
	MENU_SLIME,		// スライム
	MENU_GHOST,		// ゴースト
	MENU_GROUND,	// 地面
	MENU_MAX
}MENU;

//------------------------------
// 構造体
//------------------------------
typedef struct
{
	float fPower;	// パワー
	float fSpeed;	// スピード
}Status;

 //------------------------------
 // スタティック変数
 //------------------------------
int s_nSelect[MAXPLAYER];
int s_nIdxPower[MAXPLAYER];
int s_nIdxSpeed[MAXPLAYER];
Status s_status[MAX_CHARACTER];
LPDIRECT3DTEXTURE9 s_pTexture[MAX_CHARACTER];
int s_nIdxBG;
int s_nIdxMenu;

//------------------------------
// プロトタイプ宣言
//------------------------------
static void ChangeGauge(void);

//============================
// キャラ選択画面の初期化
//============================
void InitCharacter(void)
{
	// 矩形の初期化
	InitRectangle();

	{// 背景
		// 矩形の設定
		s_nIdxBG = SetRectangle(TEXTURE_BG);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	LoadFileSet("data\\txt\\Status.txt");

	//Player
	InitPlayer();

	Player *pPlayer = GetPlayer();
	s_nSelect[0] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(START_POS_X + PLAYERSIZE , SCREEN_HEIGHT * 0.2f, 0.0f), pPlayer->nType, true, PLAYERSIZE*2);

	pPlayer++;
	s_nSelect[1] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - START_POS_X - PLAYERSIZE, SCREEN_HEIGHT * 0.2f, 0.0f), pPlayer->nType, false, PLAYERSIZE*2);
	
	{// ゲージ
		// ゲージの初期化
		InitGauge();

		float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// ゲージの設定
		s_nIdxPower[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// ゲージの設定
		s_nIdxSpeed[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// ゲージの設定
		s_nIdxPower[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// ゲージの設定
		s_nIdxSpeed[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);
	}

	{// メニュー
		// メニューの初期化
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = 0.0f;
		menu.fRight = SCREEN_WIDTH;
		menu.fTop = 0.0f;
		menu.fBottom = SCREEN_HEIGHT;
		menu.fWidth = PLAYERSIZE;
		menu.fHeight = PLAYERSIZE;
		menu.bSort = true;

		menu.texture[MENU_FOX] = TEXTURE_kitune;
		menu.texture[MENU_CAPYBARA] = TEXTURE_enemy000;
		menu.texture[MENU_SLIME] = TEXTURE_player000;
		menu.texture[MENU_GHOST] = TEXTURE_ghost;
		menu.texture[MENU_GROUND] = TEXTURE_zolbak;

		FrameArgument Frame;
		Frame.bUse = false;
		Frame.col = GetColor(COLOR_WHITE);
		Frame.texture = TEXTURE_NONE;

		// メニューの設定
		s_nIdxMenu = SetMenu(menu, Frame);
	}
}

//============================
// キャラ選択画面の終了
//============================
void UninitCharacter(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxBG);

	UninitPlayer();

	// メニューの終了
	UninitMenu();

	// ゲージの終了
	UninitGauge();

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

		if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
		{//Sキーが押されたとき
		 //数値の減算
			player->nType++;

			if (player->nType >= MAX_CHARACTER)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
		{//Wキーが押されたとき
		 //数値の加算
			player->nType--;
			if (player->nType < 0)
			{
				player->nType = MAX_CHARACTER - 1;
				
			}
		}

		s_nSelect[0] = player->nType;
		//テクスチャ更新
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

		ChangeGauge();
	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD2) || GetJoypadTrigger(JOYKEY_DOWN))
		{//Sキーが押されたとき
		 //数値の減算
			player->nType++;
			if (player->nType >= MAX_CHARACTER)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_NUMPAD5) || GetJoypadTrigger(JOYKEY_UP))
		{//Wキーが押されたとき
		 //数値の加算
			player->nType--;
			
			if (player->nType < 0)
			{
				player->nType = MAX_CHARACTER - 1;
			}
		}

		s_nSelect[1] = player->nType;
		//テクスチャ更新
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

		ChangeGauge();
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{	//ゲーム選択画面行く
		ChangeMode(MODE_TITLE);
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
	int		number = 0;

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");

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

			if (strcmp(&s_aString[0], "STATUSSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &s_aString[0]);

					if (strcmp(&s_aString[0], "ATTACKPOW") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_status[number].fPower);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_status[number].fSpeed);
					}
					if (strcmp(&s_aString[0], "ENDSET") == 0)
					{
						number++;
						break;
					}
				}
			}
			
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

//----------------------------
//ゲージの変更
//----------------------------
static void ChangeGauge(void)
{
	float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// ゲージの減少
	SubGauge(s_nIdxPower[0], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// ゲージの設定
	SubGauge(s_nIdxSpeed[0], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// ゲージの設定
	SubGauge(s_nIdxPower[1], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// ゲージの設定
	SubGauge(s_nIdxSpeed[1], pos, GAUGE_WIDTH, fHeight);
}