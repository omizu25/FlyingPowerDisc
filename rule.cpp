//================================
//
// ルール選択画面の処理
// Author:Sato Teruto
//
//================================

//インクルード
#include "rule.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"

//マクロ定義
#define MAX_RULE	(3)					//ルールの最大数
#define MAX_SWITCH	(6)					//スイッチの最大数
#define HALF_SWITCH	(MAX_SWITCH / 2)	//スイッチの半分
#define MAX_TEXTURE	(3)					//テクスチャの最大数
#define MAX_FLASH	(80)				//点滅の往復時間
#define HALF_FLASH	(MAX_FLASH / 2)		//点滅の切り替え時間

//スタティック変数
static LPDIRECT3DTEXTURE9		s_pTexture[MAX_TEXTURE] = {};	//テクスチャへのポインタ
static Rule s_Rule[MAX_RULE];		//ルール構造体の取得
static Switch s_Switch[MAX_SWITCH];	//スイッチ構造体の取得
static int s_nFlashTime;			//点滅の時間
static int s_nSelect;				//選択中の番号

//ルール選択用
static int s_nMaxTime = 60;	//時間
static int s_MaxPoint = 21;	//ポイント数
static int s_MaxSet = 2;	//セット数

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
	s_pTexture[0] = GetTexture(TEXTURE_Title_blue);
	s_pTexture[1] = GetTexture(TEXTURE_Select_Left);
	s_pTexture[2] = GetTexture(TEXTURE_Select_Right);

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
}

//============================
// ルール選択画面の終了
//============================
void UninitRule(void)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		// 矩形を使うのを止める
		StopUseRectangle(rule->nIdx);
	}
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

	if (GetKeyboardTrigger(DIK_A))
	{//Aキーが押されたとき
	//数値の減算
		SubRule(nNumber);
	}

	if (GetKeyboardTrigger(DIK_D))
	{//Dキーが押されたとき
	 //数値の加算
		AddRule(nNumber);
	}

	//テクスチャの点滅
	FlashTexture(nNumber);
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
			rule->fWidth = 200.0f;
			rule->fHeight = 100.0f;
			rule->bUse = true;

			// 矩形の設定
			rule->nIdx = SetRectangle(s_pTexture[0]);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(rule->nIdx, pos, size);

			//切り替えボタンの設定
			SetSwitchLeft(rule->pos);	//左
			SetSwitchRight(rule->pos);	//右

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
			Switch->nIdx = SetRectangle(s_pTexture[1]);

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
			Switch->nIdx = SetRectangle(s_pTexture[2]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
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
	if (GetKeyboardTrigger(DIK_A))
	{//Aキーが押されたとき
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
	if (GetKeyboardTrigger(DIK_D))
	{//Dキーが押されたとき
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
		if (s_nMaxTime <= 60)
		{
			s_nMaxTime += 30;	//時間の減少
		}
	}
	else if (nNumber == 1)
	{
		if (s_MaxPoint <= 21)
		{
			s_MaxPoint += 3;	//ポイントの減少
		}
	}
	else if (nNumber == 2)
	{
		if (s_MaxPoint <= 2)
		{
			s_MaxSet += 1;	//セット数の減少
		}
	}
}

//============================
// 数値の減算
//============================
void SubRule(int nNumber)
{
	if (nNumber == 0)
	{
		if (s_nMaxTime >= 60)
		{
			s_nMaxTime -= 30;	//時間の減少
		}
	}
	else if (nNumber == 1)
	{
		if (s_MaxPoint >= 21)
		{
			s_MaxPoint -= 3;	//ポイントの減少
		}
	}
	else if (nNumber == 2)
	{
		if (s_MaxPoint >= 2)
		{
			s_MaxSet -= 1;	//セット数の減少
		}
	}
}

//============================
// 選択番号の切り替え
//============================
int ChangeSelect(void)
{
	// 矩形の色の設定
	SetColorRectangle(s_Rule[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W))
	{//Wキーが押されたとき
		if (s_nSelect >= 1 && s_nSelect <= MAX_RULE)
		{//0未満にならないなら
			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_S))
	{//Sキーが押されたとき
		if (s_nSelect >= 0 && s_nSelect < (MAX_RULE - 1))
		{//最大数を超えならないなら
			s_nSelect++;
		}
	}

	return s_nSelect;
}