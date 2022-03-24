//================================
//
// ルール選択画面の処理
// Author:Sato Teruto
//
//================================

//------------------------------
// インクルード
//------------------------------
#include "rule.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"
#include "number.h"
#include "mode.h"
#include "sound.h"

#include <stdio.h>
#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	START_TIME = 60;					// タイムの始まりの値
const int	START_POINT = 21;					// ポイントの始まりの値
const int	START_SET = 2;						// セットの始まりの値
const int	CHANGE_TIME = 30;					// タイムの変更値
const int	CHANGE_POINT = 3;					// ポイントの変更値
const int	CHANGE_SET = 1;						// セットの変更値
const int	MAX_TIME = 90;						// タイムの最大値
const int	MAX_POINT = 24;						// ポイントの最大値
const int	MAX_SET = 3;						// セットの最大値
const int	MIN_TIME = 30;						// タイムの最小値
const int	MIN_POINT = 18;						// ポイントの最小値
const int	MIN_SET = 1;						// セットの最小値
const float	NUMBER_WIDTH = 30.0f;				// 数の幅
const float	NUMBER_HEIGHT = 100.0f;				// 数の高さ
const char	*RULE_FILE = "data/txt/Rule.txt";	// ルールのファイル

typedef enum
{
	OPTION_TIME = 0,	// タイム
	OPTION_POINT,		// ポイント
	OPTION_SET,			// セット
	OPTION_MAX
}OPTION;
}// namespaceはここまで

//------------------------------
// マクロ定義
//------------------------------
#define MAX_RULE	(3)					//ルールの最大数
#define MAX_SWITCH	(6)					//スイッチの最大数
#define HALF_SWITCH	(MAX_SWITCH / 2)	//スイッチの半分
#define MAX_TEXTURE	(6)					//テクスチャの最大数
#define MAX_FLASH	(80)				//点滅の往復時間
#define HALF_FLASH	(MAX_FLASH / 2)		//点滅の切り替え時間

//------------------------------
// スタティック変数
//------------------------------
static TEXTURE		s_Texture[MAX_TEXTURE] = {};	//テクスチャへのポインタ

//構造体
static Rule s_Rule[MAX_RULE];		//ルール構造体の取得
static Switch s_Switch[MAX_SWITCH];	//スイッチ構造体の取得
static BG s_BG;						//背景構造体の取得

//値
static int s_nFlashTime;				//点滅の時間
static int s_nSelect;					//選択中の番号
static int s_nOption[OPTION_MAX];		// 選択肢の値
static int s_nNumberIdx[OPTION_MAX];	// 選択肢の値のインデックス

//============================
// ルール選択画面の初期化
//============================
void InitRule(void)
{
	// 矩形の初期化
	InitRectangle();

	//------------------------------
	// テクスチャの取得
	//------------------------------
	s_Texture[0] = TEXTURE_Select_Time;			//時間
	s_Texture[1] = TEXTURE_Select_Point;		//ポイント数
	s_Texture[2] = TEXTURE_Select_Set;			//セット数
	s_Texture[3] = TEXTURE_Select_Left;			//左ボタン
	s_Texture[4] = TEXTURE_Select_Right;		//右ボタン
	s_Texture[5] = TEXTURE_Select_BG;			//背景

	//------------------------------
	//	構造体の初期化
	//------------------------------
	//ルール構造体の初期化
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		rule->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		rule->fWidth = 0.0f;	//幅
		rule->fHeight = 0.0f;	//高さ
		rule->bUse = false;		//使用していない状態
	}

	//スイッチ構造体の初期化
	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		Switch->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		Switch->fWidth = 0.0f;		//幅
		Switch->fHeight = 0.0f;		//高さ
		Switch->bUse = false;		//使用していない状態
	}

	s_BG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_BG.fWidth = 0.0f;
	s_BG.fHeight = 0.0f;
	s_BG.bUse = false;

	// 数の初期化
	InitNumber();
}

//============================
// ルール選択画面の終了
//============================
void UninitRule(void)
{
	//音の停止
	StopSound();

	// 数の終了
	UninitNumber();

	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		// 矩形を使うのを止める
		StopUseRectangle(rule->nIdx);
	}

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		// 矩形を使うのを止める
		StopUseRectangle(Switch->nIdx);
	}

	// 矩形を使うのを止める
	StopUseRectangle(s_BG.nIdx);
}

//============================
// ルール選択画面の更新
//============================
void UpdateRule(void)
{
	s_nFlashTime++;				//タイムの加算
	s_nFlashTime %= MAX_FLASH;	//タイムの初期化

	//選択番号の切り替え
	int nNumber = ChangeSelect();

	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
	{//Aキーが押されたとき
		//数値の減算
		SubRule(nNumber);

		// セーブ
		SaveRule();
	}

	if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
	{//Dキーが押されたとき
		//数値の加算
		AddRule(nNumber);

		// セーブ
		SaveRule();
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		 //音の再生
		PlaySound(SOUND_LABEL_ENTER);

		 //タイトルに戻る
		ChangeMode(MODE_TITLE);
	}
	//テクスチャの点滅
	FlashTexture(nNumber);

	// 数の更新
	UpdateNumber();
}

//============================
// ルール選択画面の描画
//============================
void DrawRule(void)
{
	// 矩形の描画
	DrawRectangle();
}

//============================
// ルール選択画面の設定
//============================
void SetRule(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == false)
		{//使用していないなら
			//構造体の設定
			rule->pos = pos;
			rule->fWidth = 400.0f;
			rule->fHeight = 100.0f;
			rule->bUse = true;

			// 矩形の設定
			rule->nIdx = SetRectangle(s_Texture[nCnt]);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(rule->nIdx, pos, size);

			//切り替えボタンの設定
			SetSwitchLeft(rule->pos);	//左
			SetSwitchRight(rule->pos);	//右

			// 桁数
			int nDigit = DigitNumber(s_nOption[nCnt]);

			D3DXVECTOR3 posNumber = pos + D3DXVECTOR3(450.0f + (NUMBER_WIDTH * (nDigit * 0.5f)), 0.0f, 0.0f);
			D3DXVECTOR3 sizeNumber = D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f);

			// 数の設定
			s_nNumberIdx[nCnt] = SetNumber(posNumber, sizeNumber, GetColor(COLOR_RED), s_nOption[nCnt]);

			break;
		}
	}
}

//============================
// 切り替えボタン(左)の設定
//============================
void SetSwitchLeft(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < HALF_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//使用していないなら
			//構造体の設定
			Switch->pos = D3DXVECTOR3(pos.x + 300.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// 矩形の設定
			Switch->nIdx = SetRectangle(s_Texture[3]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// 切り替えボタン(右)の設定
//============================
void SetSwitchRight(D3DXVECTOR3 pos)
{
	for (int nCnt = HALF_SWITCH; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//使用していないなら
		 //構造体の設定
			Switch->pos = D3DXVECTOR3(pos.x + 600.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// 矩形の設定
			Switch->nIdx = SetRectangle(s_Texture[4]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// 背景の設定
//============================
void SetBG(D3DXVECTOR3 pos)
{
	if (s_BG.bUse == false)
	{//使用していないなら
		 //構造体の設定
		s_BG.pos = pos;
		s_BG.fWidth = SCREEN_WIDTH;
		s_BG.fHeight = SCREEN_HEIGHT;
		s_BG.bUse = true;

		// 矩形の設定
		s_BG.nIdx = SetRectangle(s_Texture[5]);

		D3DXVECTOR3 size = D3DXVECTOR3(s_BG.fWidth, s_BG.fHeight, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_BG.nIdx, pos, size);
	}
}

//============================
// テクスチャの点滅
//============================
void FlashTexture(int nNumber)
{
	//------------------------------
	// 項目のテクスチャ
	//------------------------------
	if (s_nFlashTime >= HALF_FLASH)
	{
		// 矩形の色の設定
		SetColorRectangle(s_Rule[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// 矩形の色の設定
		SetColorRectangle(s_Rule[nNumber].nIdx, GetColor(COLOR_WHITE));
	}

	//選択ボタンの色の初期化
	SetColorRectangle(s_Switch[nNumber].nIdx, GetColor(COLOR_WHITE));
	SetColorRectangle(s_Switch[nNumber + 3].nIdx, GetColor(COLOR_WHITE));

	//------------------------------
	// 左選択
	//------------------------------
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
	{//Aキーが押されたとき
	 //音の再生
		PlaySound(SOUND_LABEL_SELECT);

		if (s_nFlashTime >= HALF_FLASH)
		{
			// 矩形の色の設定
			SetColorRectangle(s_Switch[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// 矩形の色の設定
			SetColorRectangle(s_Switch[nNumber].nIdx, GetColor(COLOR_WHITE));
		}
	}

	//------------------------------
	// 右選択
	//------------------------------
	if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
	{//Dキーが押されたとき
	 //音の再生
		PlaySound(SOUND_LABEL_SELECT);

		if (s_nFlashTime >= HALF_FLASH)
		{
			// 矩形の色の設定
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// 矩形の色の設定
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, GetColor(COLOR_WHITE));
		}
	}
}

//============================
// 数値の加算
//============================
void AddRule(int nNumber)
{
	if (nNumber == 0)
	{
		if (s_nOption[nNumber] < MAX_TIME)
		{
			s_nOption[nNumber] += CHANGE_TIME;	//時間の増加
		}
	}
	else if (nNumber == 1)
	{
		if (s_nOption[nNumber] < MAX_POINT)
		{
			s_nOption[nNumber] += CHANGE_POINT;	//ポイントの増加
		}
	}
	else if (nNumber == 2)
	{
		if (s_nOption[nNumber] < MAX_SET)
		{
			s_nOption[nNumber] += CHANGE_SET;	//セット数の増加
		}
	}

	// 数の変更
	ChangeNumber(s_nNumberIdx[nNumber], s_nOption[nNumber]);
}

//============================
// 数値の減算
//============================
void SubRule(int nNumber)
{
	if (nNumber == 0)
	{
		if (s_nOption[nNumber] > MIN_TIME)
		{
			s_nOption[nNumber] -= CHANGE_TIME;	//時間の減少
		}
	}
	else if (nNumber == 1)
	{
		if (s_nOption[nNumber] > MIN_POINT)
		{
			s_nOption[nNumber] -= CHANGE_POINT;	//ポイントの減少
		}
	}
	else if (nNumber == 2)
	{
		if (s_nOption[nNumber] > MIN_SET)
		{
			s_nOption[nNumber] -= CHANGE_SET;	//セット数の減少
		}
	}

	// 数の変更
	ChangeNumber(s_nNumberIdx[nNumber], s_nOption[nNumber]);
}

//============================
// 選択番号の切り替え
//============================
int ChangeSelect(void)
{
	// 矩形の色の設定
	SetColorRectangle(s_Rule[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//Wキーが押されたとき
		if (s_nSelect >= 1 && s_nSelect <= MAX_RULE)
		{//0未満にならないなら
		 //音の再生
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//Sキーが押されたとき
		if (s_nSelect >= 0 && s_nSelect < (MAX_RULE - 1))
		{//最大数を超えならないなら
		 //音の再生
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect++;
		}
	}

	return s_nSelect;
}

//============================
// 読み込み
//============================
void LoadRule(void)
{
	FILE *pFile;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(RULE_FILE, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fscanf(pFile, "%d", &s_nOption[OPTION_TIME]);
		fscanf(pFile, "%d", &s_nOption[OPTION_POINT]);
		fscanf(pFile, "%d", &s_nOption[OPTION_SET]);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//============================
// セーブ
//============================
void SaveRule(void)
{
	FILE *pFile;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(RULE_FILE, "w");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_TIME]);
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_POINT]);
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_SET]);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//============================
// リセットのセーブ
//============================
void ResetSaveRule(void)
{
	FILE *pFile;	// ファイルポインタを宣言

					// ファイルを開く
	pFile = fopen(RULE_FILE, "w");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fprintf(pFile, "%d\n\n", START_TIME);
		fprintf(pFile, "%d\n\n", START_POINT);
		fprintf(pFile, "%d\n\n", START_SET);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//============================
// 時間の取得
//============================
int GetTimeRule(void)
{
	return s_nOption[OPTION_TIME];
}

//============================
// ポイント数の取得
//============================
int GetPointRule(void)
{
	return s_nOption[OPTION_POINT];
}

//============================
// セット数の取得
//============================
int GetSetRule(void)
{
	return s_nOption[OPTION_SET];
}