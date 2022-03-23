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
const int	MAX_GAUGE = 256;	// ゲージの最大数

typedef struct
{
	D3DXVECTOR3	pos;			// 位置
	int			nIdx;			// 矩形のインデックス
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	float		fWidthDest;		// 目的の幅
	float		fHeightDest;	// 目的の高さ
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
int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (pGauge->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pGauge->pos = pos;
		pGauge->fWidth = fWidth;
		pGauge->fHeight = fHeight;
		pGauge->fWidthDest = fWidth;
		pGauge->fHeightDest = fHeight;
		pGauge->bUse = true;

		// 矩形の設定
		pGauge->nIdx = SetRectangle(TEXTURE_NONE);

		// 矩形の位置の設定
		SetPosRectangle(pGauge->nIdx, pos, D3DXVECTOR3(fWidth, fHeight, 0.0f));

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
void SubGauge(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	assert(nIdx >= 0 && nIdx < MAX_GAUGE);

	Gauge *pGauge = &s_gauge[nIdx];

	pGauge->pos = pos;
	pGauge->fWidth = fWidth;
	pGauge->fHeight = fHeight;
	pGauge->fWidthDest = fWidth;
	pGauge->fHeightDest = fHeight;

	D3DXVECTOR3 size = D3DXVECTOR3(pGauge->fWidth, pGauge->fHeight, 0.0f);

	// 矩形の位置の設定
	SetPosRectangle(pGauge->nIdx, pGauge->pos, size);
}
