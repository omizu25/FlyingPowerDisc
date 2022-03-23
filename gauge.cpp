//**************************************************
//
// FPD制作 ( gauge.cpp )
// Author  : katsuki mizuki
//
//**************************************************
//==================================================
// インクルード
//==================================================
#include "gauge.h"
#include "rectangle.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_GAUGE = 256;		// ゲージの最大数
const float	CHANGE_RATIO = 0.05f;	// 変更の割合

typedef struct
{
	D3DXVECTOR3	pos;			// 位置
	int			nIdx;			// 矩形のインデックス
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	float		fWidthDest;		// 目的の幅
	float		fHeightDest;	// 目的の高さ
	GAUGE		gauge;			// 中心の位置
	bool		bUse;			// 使用しているかどうか
}Gauge;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Gauge	s_gauge[MAX_GAUGE];	// ゲージの情報
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void PosEdge(D3DXVECTOR3 *pPosOut, const D3DXVECTOR3 &posStart, const D3DXVECTOR3 &size, GAUGE gauge);
void ChangeLength(float *fLength, float fLengthDest);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGauge(void)
{
	// メモリのクリア
	memset(&s_gauge[0], 0, sizeof(s_gauge));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGauge(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (!pGauge->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// 使うのを止める
		StopUseRectangle(pGauge->nIdx);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGauge(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (!pGauge->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// 長さの変更
		ChangeLength(&pGauge->fWidth, pGauge->fWidthDest);
		ChangeLength(&pGauge->fHeight, pGauge->fHeightDest);

		D3DXVECTOR3 size = D3DXVECTOR3(pGauge->fWidth, pGauge->fHeight, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 位置を端にずらす
		PosEdge(&pos, pGauge->pos, size, pGauge->gauge);

		// 矩形の位置の設定
		SetPosRectangle(pGauge->nIdx, pos, size);
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGauge(void)
{
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetGauge(const D3DXVECTOR3 &posStart, const D3DXCOLOR &col, float fWidth, float fHeight, GAUGE gauge)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (pGauge->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pGauge->pos = posStart;
		pGauge->fWidth = fWidth;
		pGauge->fHeight = fHeight;
		pGauge->fWidthDest = fWidth;
		pGauge->fHeightDest = fHeight;
		pGauge->gauge = gauge;
		pGauge->bUse = true;

		// 矩形の設定
		pGauge->nIdx = SetRectangle(TEXTURE_NONE);

		D3DXVECTOR3 size = D3DXVECTOR3(fWidth, fHeight, 0.0f);
		D3DXVECTOR3 posOut = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 位置を端にずらす
		PosEdge(&posOut, posStart, size, gauge);

		// 矩形の位置の設定
		SetPosRectangle(pGauge->nIdx, posOut, size);

		// 矩形の色の設定
		SetColorRectangle(pGauge->nIdx, col);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 減少
//--------------------------------------------------
void SubGauge(int nIdx, float fWidth, float fHeight)
{
	assert(nIdx >= 0 && nIdx < MAX_GAUGE);

	Gauge *pGauge = &s_gauge[nIdx];

	pGauge->fWidthDest = fWidth;
	pGauge->fHeightDest = fHeight;
}

namespace
{
//--------------------------------------------------
// 位置を端にずらす
//--------------------------------------------------
void PosEdge(D3DXVECTOR3 *pPosOut, const D3DXVECTOR3 &posStart, const D3DXVECTOR3 &size, GAUGE gauge)
{
	switch (gauge)
	{
	case GAUGE_TOP:		// 上
		*pPosOut = D3DXVECTOR3(posStart.x, posStart.y + (size.y * 0.5f), 0.0f);
		break;

	case GAUGE_BOTTOM:	// 下
		*pPosOut = D3DXVECTOR3(posStart.x, posStart.y - (size.y * 0.5f), 0.0f);
		break;

	case GAUGE_LEFT:	// 左
		*pPosOut = D3DXVECTOR3(posStart.x + (size.x * 0.5f), posStart.y, 0.0f);
		break;

	case GAUGE_RIGHT:	// 右
		*pPosOut = D3DXVECTOR3(posStart.x - (size.x * 0.5f), posStart.y, 0.0f);
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 長さの変更
//--------------------------------------------------
void ChangeLength(float *fLength, float fLengthDest)
{
	if (*fLength == fLengthDest)
	{// 目的の長さと現在の長さが同じ
		return;
	}

	float fDiff = fLengthDest - *fLength;
	float fChange = fDiff * CHANGE_RATIO;

	if ((fChange <= CHANGE_RATIO) && (fChange >= -CHANGE_RATIO))
	{// 変更値が割合値以下
		*fLength = fLengthDest;
	}
	else
	{// 変更値が割合値より大き
		*fLength += fChange;
	}
}
}// namespaceはここまで
