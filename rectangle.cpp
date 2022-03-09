//==================================================
// 
// FPG制作 ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "rectangle.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_RECTANGLE		(1024)		// 矩形の最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9			*pTexture;			// テクスチャ
	LPDIRECT3DVERTEXBUFFER9		*pVtxBuff;			// 頂点バッファ
	int							nBuffLength;		// バッファの長さ
	bool						bUse;				// 使用しているかどうか
}RectAngle;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
RectAngle		s_aRectAngle[MAX_RECTANGLE];		// 矩形の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectAngle(void)
{
	// メモリのクリア
	memset(s_aRectAngle, 0, sizeof(s_aRectAngle));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectAngle(void)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->pTexture != NULL)
		{// テクスチャの解放
			pRectAngle->pTexture = NULL;
		}

		if (pRectAngle->pVtxBuff != NULL)
		{// 頂点バッファの解放
			pRectAngle->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectAngle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (!pRectAngle->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, *pRectAngle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		if (*pRectAngle->pTexture != NULL)
		{// テクスチャがある
			pDevice->SetTexture(0, *pRectAngle->pTexture);
		}
		else
		{// テクスチャがない
			pDevice->SetTexture(0, NULL);
		}

		for (int j = 0; j < pRectAngle->nBuffLength; j++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				j * 4,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetRectAngle(LPDIRECT3DTEXTURE9 *pTexture, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff, const int nBuffLength)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		if (pTexture != NULL)
		{// テクスチャがある
			pRectAngle->pTexture = pTexture;
		}
		else
		{// テクスチャがない
			pRectAngle->pTexture = NULL;
		}

		pRectAngle->pVtxBuff = pVtxBuff;
		pRectAngle->nBuffLength = nBuffLength;
		pRectAngle->bUse = true;

		break;
	}
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseRectAngle(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->pVtxBuff != pVtxBuff)
		{// 頂点バッファが違う
			continue;
		}

		/*↓ 頂点バッファが同じ ↓*/

		pRectAngle->bUse = false;

		break;
	}
}