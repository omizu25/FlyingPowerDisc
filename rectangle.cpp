//**************************************************
// 
// FPG制作 ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "rectangle.h"

#include <assert.h>

//==================================================
// マクロ定義
//==================================================
#define MAX_RECTANGLE		(1024)		// 矩形の最大数

//==================================================
// 構造体
//==================================================
typedef struct
{
	LPDIRECT3DTEXTURE9			pTexture;		// テクスチャ
	LPDIRECT3DVERTEXBUFFER9		pVtxBuff;		// 頂点バッファ
	bool						bUse;			// 使用しているかどうか
}MyRectangle;

//==================================================
// スタティック変数
//==================================================
MyRectangle		s_aRectangle[MAX_RECTANGLE];		// 矩形の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void)
{
	// メモリのクリア
	memset(s_aRectangle, 0, sizeof(s_aRectangle));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle *pRectangle = &s_aRectangle[i];

		if (!pRectangle->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pRectangle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pRectangle->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetRectangle(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle *pRectangle = &s_aRectangle[i];

		if (pRectangle->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pRectangle->pTexture = pTexture;
		pRectangle->bUse = true;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRectangle->pVtxBuff,
			NULL);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle *pRectangle = &s_aRectangle[nIdx];

	pRectangle->bUse = false;

	if (pRectangle->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pRectangle->pVtxBuff->Release();
		pRectangle->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 頂点バッファを取得
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	return s_aRectangle[nIdx].pVtxBuff;
}
