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
const int	TEX_DIVIDE = 10;	// テクスチャの分割数

typedef struct
{
	int		nNumber;	// 数
	int		nIdx;		// 矩形のインデックス
	bool	bUse;		// 使用しているかどうか
}Number;
}// namesapceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Number	s_Number[MAX_NUMBER];	// 数の情報
}// namesapceはここまで

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

		// 使うのを止める
		StopUseRectangle(pNumber->nIdx);
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
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber->nNumber = nNumber;
		pNumber->bUse = true;

		// 矩形の設定
		pNumber->nIdx = SetRectangle(TEXTURE_Number_0To9);

		// 矩形の位置の設定


		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使うのを止める
//--------------------------------------------------
void StopUseNumber(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	pNumber->bUse = false;

	// 使うのを止める
	StopUseRectangle(pNumber->nIdx);
}
