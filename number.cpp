//**************************************************
// 
// FPD制作 ( number.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "number.h"
#include "rectangle.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_NUMBER = 256;	// 数の最大数
const int	MAX_DIGIT = 8;		// 桁数の最大数
const int	TEX_DIVIDE = 10;	// テクスチャの分割数

typedef struct
{
	D3DXVECTOR3	pos;				// 位置
	D3DXVECTOR3	size;				// サイズ
	D3DXCOLOR	col;				// 色
	int			nNumber[MAX_DIGIT];	// 数
	int			nDigit;				// 桁数
	int			nIdx[MAX_DIGIT];	// 矩形のインデックス
	bool		bUse;				// 使用しているかどうか
}Number;
}// namesapceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Number	s_Number[MAX_NUMBER];	// 数の情報
int		s_nDigit;				// 桁数計算用
}// namesapceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void OneDivideNumber(Number *pNumber, int nNumber);
void SetOneDigitNumber(Number *pNumber, int nDigit);
}// namespaceはここまで


//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitNumber(void)
{
	// メモリのクリア
	memset(&s_Number, NULL, sizeof(s_Number));

	s_nDigit = 0;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitNumber(void)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		for (int j = 0; j < pNumber->nDigit; j++)
		{
			// 使うのを止める
			StopUseRectangle(pNumber->nIdx[j]);
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawNumber(void)
{
	
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetNumber(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col, int nNumber)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pNumber->pos = pos;
		pNumber->size = size;
		pNumber->col = col;

		// 桁数
		pNumber->nDigit = DigitNumber(nNumber);

		// 一桁ずつに分ける
		OneDivideNumber(pNumber, nNumber);

		for (int j = 0; j < pNumber->nDigit; j++)
		{
			// 一桁ずつの設定
			SetOneDigitNumber(pNumber, j);
		}

		pNumber->bUse = true;

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 位置の設定
// 引数1  : int nIdx / 配列のインデックス
// 引数2  : D3DXVECTOR3 &pos / 中心の位置
// 引数3  : D3DXVECTOR3 &size / サイズ
//--------------------------------------------------
void SetPosNumber(int nIdx, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	if (!pNumber->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pNumber->pos = pos;
	pNumber->size = size;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// 一桁ずつの設定
		SetOneDigitNumber(pNumber, i);
	}
}

//--------------------------------------------------
// 変更
//--------------------------------------------------
void ChangeNumber(int nIdx, int nNumber)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	if (!pNumber->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	if (pNumber->nDigit != DigitNumber(nNumber))
	{// 桁数が違う
		for (int i = 0; i < pNumber->nDigit; i++)
		{
			// 使うのを止める
			StopUseRectangle(pNumber->nIdx[i]);
		}

		// 桁数
		pNumber->nDigit = DigitNumber(nNumber);

		// 一桁ずつに分ける
		OneDivideNumber(pNumber, nNumber);

		for (int i = 0; i < pNumber->nDigit; i++)
		{
			// 一桁ずつの設定
			SetOneDigitNumber(pNumber, i);
		}
	}
	else
	{// 桁数が同じ
		// 一桁ずつに分ける
		OneDivideNumber(pNumber, nNumber);
	}

	float fDivide = 1.0f / TEX_DIVIDE;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		float fTex = fDivide * pNumber->nNumber[i];

		// 矩形のテクスチャ座標の設定
		SetTexRectangle(pNumber->nIdx[i], D3DXVECTOR2(fTex, fTex + fDivide), D3DXVECTOR2(0.0f, 1.0f));
	}
}

//--------------------------------------------------
// 使うのを止める
//--------------------------------------------------
void StopUseNumber(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	pNumber->bUse = false;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// 使うのを止める
		StopUseRectangle(pNumber->nIdx[i]);
	}
}

//--------------------------------------------------
// 桁数
//--------------------------------------------------
int DigitNumber(int nNumber)
{
	s_nDigit = 0;
	int nSaveNumber = nNumber;

	while (1)
	{// 無限ループ
		if (nSaveNumber >= 10)
		{// 2桁以上
			nSaveNumber /= 10;
			s_nDigit++;
		}
		else
		{// 1桁
			s_nDigit++;

			if (s_nDigit > MAX_DIGIT)
			{// 指定の値より大きい
				s_nDigit = MAX_DIGIT;
			}
			break;
		}
	}

	return s_nDigit;
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawNumber(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	if (!pNumber->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// 矩形の描画するかどうか
		SetDrawRectangle(pNumber->nIdx[i], bDraw);
	}
}

namespace
{
//--------------------------------------------------
// 一桁ずつに分ける
//--------------------------------------------------
void OneDivideNumber(Number *pNumber, int nNumber)
{
	int nSaveNumber = nNumber;

	for (int i = 0; i < pNumber->nDigit; i++)
	{// 一桁ずつに分ける
		pNumber->nNumber[i] = nSaveNumber % 10;
		nSaveNumber /= 10;
	}
}

//--------------------------------------------------
// 一桁ずつの設定
//--------------------------------------------------
void SetOneDigitNumber(Number *pNumber, int nDigit)
{
	// 矩形の設定
	pNumber->nIdx[nDigit] = SetRectangle(TEXTURE_Number_0To9);

	float fInterval = (pNumber->size.x * nDigit) + (pNumber->size.x * 0.5f);

	// 矩形の位置の設定
	SetPosRectangle(pNumber->nIdx[nDigit], pNumber->pos + D3DXVECTOR3(-fInterval, 0.0f, 0.0f), pNumber->size);

	// 矩形の色の設定
	SetColorRectangle(pNumber->nIdx[nDigit], pNumber->col);

	float fDivide = 1.0f / TEX_DIVIDE;
	float fTex = fDivide * pNumber->nNumber[nDigit];

	// 矩形のテクスチャ座標の設定
	SetTexRectangle(pNumber->nIdx[nDigit], D3DXVECTOR2(fTex, fTex + fDivide), D3DXVECTOR2(0.0f, 1.0f));
}
}// namespaceはここまで
