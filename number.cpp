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
	int		nNumber[MAX_DIGIT];	// 数
	int		nDigit;				// 桁数
	int		nIdx[MAX_DIGIT];	// 矩形のインデックス
	bool	bUse;				// 使用しているかどうか
}Number;
}// namesapceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Number	s_Number[MAX_NUMBER];	// 数の情報
}// namesapceはここまで

 //==================================================
 // スタティック関数プロトタイプ宣言
 //==================================================
namespace
{
void SetOneDigitNumber(Number *pNumber, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col);
}// namespaceはここまで


//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitNumber(void)
{
	// メモリのクリア
	memset(&s_Number, NULL, sizeof(s_Number));
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

		int nSaveNumber = nNumber;

		while (1)
		{// 無限ループ
			if (nSaveNumber >= 10)
			{// 2桁以上
				nSaveNumber /= 10;
				pNumber->nDigit++;
			}
			else
			{// 1桁
				pNumber->nDigit++;

				if (pNumber->nDigit > MAX_DIGIT)
				{// 指定の値より大きい
					pNumber->nDigit = MAX_DIGIT;
				}
				break;
			}
		}

		for (int j = 0; j < pNumber->nDigit; j++)
		{// 一桁ずつに分ける
			pNumber->nNumber[j] = nNumber % 10;
			nNumber /= 10;
		}

		// 一桁ずつの設定
		SetOneDigitNumber(pNumber, pos, size, col);

		pNumber->bUse = true;

		return i;
	}

	assert(false);
	return -1;
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

	float fDivide = 1.0f / TEX_DIVIDE;
	float fTex = fDivide * nNumber;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
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

namespace
{
//--------------------------------------------------
// 一桁ずつの設定
//--------------------------------------------------
void SetOneDigitNumber(Number *pNumber, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col)
{
	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// 矩形の設定
		pNumber->nIdx[i] = SetRectangle(TEXTURE_Number_0To9);

		float fInterval = (size.x * i) + (size.x * 0.5f);

		// 矩形の位置の設定
		SetPosRectangle(pNumber->nIdx[i], pos + D3DXVECTOR3(-fInterval, 0.0f, 0.0f), size);

		// 矩形の色の設定
		SetColorRectangle(pNumber->nIdx[i], col);

		float fDivide = 1.0f / TEX_DIVIDE;
		float fTex = fDivide * pNumber->nNumber[i];

		// 矩形のテクスチャ座標の設定
		SetTexRectangle(pNumber->nIdx[i], D3DXVECTOR2(fTex, fTex + fDivide), D3DXVECTOR2(0.0f, 1.0f));
	}
}
}// namespaceはここまで
