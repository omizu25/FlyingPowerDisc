//================================
//
// マップ選択画面の処理
// Author:Sato Teruto
// Author:Seino Riku
//
//================================

//------------------------------
// インクルード
//------------------------------
#include "map.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"
#include "number.h"
#include "mode.h"
#include "sound.h"
#include "cursor.h"

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // マクロ定義
 //------------------------------
#define MAX_MAP			(3)				//ルールの最大数
#define MAX_TEXTURE		(3)				//テクスチャの最大数
#define MAX_FLASH		(80)			//点滅の往復時間
#define HALF_FLASH		(MAX_FLASH / 2)	//点滅の切り替え時間
#define MAP_WIDTH		(350.0f)		//マップの幅
#define MAP_HEIGHT		(150.0f)		//マップの高さ
#define NAME_WIDTH		(300.0f)		//名前の幅
#define NAME_HEIGHT		(125.0f)		//名前の高さ
#define SELECT_WIDTH	(250.0f)		//選択の幅
#define SELECT_HEIGHT	(50.0f)			//選択の高さ
#define CURSOR_SIZE		(75.0f)			//カーソルのサイズ

 //------------------------------
 // スタティック変数
 //------------------------------
static TEXTURE		s_Texture[MAX_TEXTURE] = {};	//テクスチャへのポインタ
static Map s_Map[MAX_MAP];							//ルール構造体の取得
static MAPBG s_MAPBG;								//背景構造体の取得
static int s_nIdx;									//矩形のインデックス
static int s_nIdxName[MAX_MAP];						//名前の矩形のインデックス
static int s_nFlashTime;							//点滅の時間
static int s_nSelect;								//選択中の番号
static int s_nIdxCursor;							//カーソルの配列のインデックス

//============================
// ルール選択画面の初期化
//============================
void InitMap(void)
{
	// 矩形の初期化
	InitRectangle();

	//------------------------------
	// テクスチャの取得
	//------------------------------
	s_Texture[0] = TEXTURE_Stage01;
	s_Texture[1] = TEXTURE_Stage02;
	s_Texture[2] = TEXTURE_Stage03;

	//------------------------------
	//	構造体の初期化
	//------------------------------
	//ルール構造体の初期化
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		Map->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		Map->fWidth = 0.0f;	//幅
		Map->fHeight = 0.0f;	//高さ
		Map->bUse = false;		//使用していない状態
	}

	s_MAPBG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_MAPBG.fWidth = 0.0f;
	s_MAPBG.fHeight = 0.0f;
	s_MAPBG.bUse = false;

	SetBGMap(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	{// 選択
		// 矩形の設定
		s_nIdx = SetRectangle(TEXTURE_Map_Title);

		D3DXVECTOR3 size = D3DXVECTOR3(SELECT_WIDTH, SELECT_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SELECT_WIDTH * 0.5f, SELECT_HEIGHT * 0.5f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdx, pos, size);
	}

	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.25f, 0.0f));
	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.5f, 0.0f));
	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.75f, 0.0f));

	{// 名前
		TEXTURE aTexture[MAX_MAP];

		aTexture[0] = TEXTURE_Stage1;
		aTexture[1] = TEXTURE_Stage2;
		aTexture[2] = TEXTURE_Stage3;

		for (int i = 0; i < MAX_MAP; i++)
		{
			// 矩形の設定
			s_nIdxName[i] = SetRectangle(aTexture[i]);

			D3DXVECTOR3 size = D3DXVECTOR3(NAME_WIDTH, NAME_HEIGHT, 0.0f);
			D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, (SCREEN_HEIGHT * 0.25f) + ((SCREEN_HEIGHT * 0.25f) * i), 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(s_nIdxName[i], pos, size);
		}
	}

	{// カーソル
		// カーソル初期化
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MAX_MAP;
		cursor.fPosX = SCREEN_WIDTH * 0.15f;
		cursor.fTop = 0.0f;
		cursor.fBottom = SCREEN_HEIGHT;
		cursor.fWidth = CURSOR_SIZE;
		cursor.fHeight = CURSOR_SIZE;
		cursor.texture = TEXTURE_Cursor_Right;
		cursor.nSelect = s_nSelect;
		cursor.bRotation = false;

		// カーソルの設定
		s_nIdxCursor = SetCursor(cursor);
	}
}

//============================
// マップ選択画面の終了
//============================
void UninitMap(void)
{
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		// 矩形を使うのを止める
		StopUseRectangle(Map->nIdx);
	}

	// 矩形を使うのを止める
	StopUseRectangle(s_MAPBG.nIdx);
	StopUseRectangle(s_nIdx);

	// カーソルの終了
	UninitCursor();
}

//============================
// マップ選択画面の更新
//============================
void UpdateMap(void)
{
	s_nFlashTime++;				//タイムの加算
	s_nFlashTime %= MAX_FLASH;	//タイムの初期化

	//テクスチャの点滅
	FlashTextureMap(ChangeSelectMap());

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		//音の再生
		PlaySound(SOUND_LABEL_ENTER);

		//タイトルに戻る
		ChangeMode(MODE_TITLE);
	}
}

//============================
// マップ選択画面の描画
//============================
void DrawMap(void)
{
	// 矩形の描画
	DrawRectangle();
}

//============================
// マップ選択画面の設定
//============================
void SetMap(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		if (Map->bUse == false)
		{//使用していないなら
		 //構造体の設定
			Map->pos = pos;
			Map->fWidth = MAP_WIDTH;
			Map->fHeight = MAP_HEIGHT;
			Map->bUse = true;

			// 矩形の設定
			Map->nIdx = SetRectangle(s_Texture[nCnt]);

			D3DXVECTOR3 size = D3DXVECTOR3(Map->fWidth, Map->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(Map->nIdx, pos, size);
			break;
		}
	}
}

//============================
// 背景の設定
//============================
void SetBGMap(D3DXVECTOR3 pos)
{
	if (s_MAPBG.bUse == false)
	{//使用していないなら
	 //構造体の設定
		s_MAPBG.pos = pos;
		s_MAPBG.fWidth = SCREEN_WIDTH;
		s_MAPBG.fHeight = SCREEN_HEIGHT;
		s_MAPBG.bUse = true;

		// 矩形の設定
		s_MAPBG.nIdx = SetRectangle(TEXTURE_Select_BG);

		D3DXVECTOR3 size = D3DXVECTOR3(s_MAPBG.fWidth, s_MAPBG.fHeight, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_MAPBG.nIdx, pos, size);
	}
}

//============================
// テクスチャの点滅
//============================
void FlashTextureMap(int nNumber)
{
	//------------------------------
	// 項目のテクスチャ
	//------------------------------
	if (s_nFlashTime >= HALF_FLASH)
	{
		// 矩形の色の設定
		SetColorRectangle(s_Map[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// 矩形の色の設定
		SetColorRectangle(s_Map[nNumber].nIdx, GetColor(COLOR_WHITE));
	}
}

//============================
// 選択番号の切り替え
//============================
int ChangeSelectMap(void)
{
	// 矩形の色の設定
	SetColorRectangle(s_Map[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//Wキーが押されたとき
		if (s_nSelect >= 1 && s_nSelect <= MAX_MAP)
		{//0未満にならないなら
			//音の再生
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect--;

			// カーソルの位置の変更
			ChangePosCursor(s_nIdxCursor, s_nSelect);
		}
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//Sキーが押されたとき
		if (s_nSelect >= 0 && s_nSelect < (MAX_MAP - 1))
		{//最大数を超えならないなら
		 //音の再生
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect++;

			// カーソルの位置の変更
			ChangePosCursor(s_nIdxCursor, s_nSelect);
		}
	}

	return s_nSelect;
}

//============================
// 選択番号の取得
//============================
int GetSelectMap(void)
{
	return s_nSelect;
}
