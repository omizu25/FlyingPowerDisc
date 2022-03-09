//==================================================
//
// FPD制作 ( disc.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "disc.h"
#include "rectangle.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define DISC_SIZE		(150.0f)		// ディスクのサイズ

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXVECTOR3		move;			// 移動量
	float			fLength;		// 対角線の長さ
	float			fAngle;			// 対角線の角度
	bool			bUse;			// 使用してるかどうか
}Disc;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファのポインタ
static Disc							s_disc;					// ディスクの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void OffScreen(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitDisc(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/disc.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_disc.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(5.0f, 5.0f, 0.0f);

	float fSize = DISC_SIZE * 0.5f;

	// 対角線の長さを算出する
	s_disc.fLength = sqrtf((fSize * fSize) + (fSize * fSize));

	// 対角線の角度を算出する
	s_disc.fAngle = atan2f(fSize, fSize);

	// 頂点座標の設定
	pVtx[0].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = WHITE_COLOR;
	pVtx[1].col = WHITE_COLOR;
	pVtx[2].col = WHITE_COLOR;
	pVtx[3].col = WHITE_COLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// 矩形の設定
	//SetRectangle(&s_pTexture, &s_pVtxBuff, 1);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitDisc(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateDisc(void)
{
	s_disc.rot.z += 0.25f;

	if (s_disc.rot.z >= D3DX_PI)
	{// 3.14より大きい
		s_disc.rot.z -= D3DX_PI * 2.0f;
	}
	else if (s_disc.rot.z <= -D3DX_PI)
	{// -3.14より小さい
		s_disc.rot.z += D3DX_PI * 2.0f;
	}

	s_disc.pos += s_disc.move;

	// 画面外
	OffScreen();

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 画面外
//--------------------------------------------------
static void OffScreen(void)
{
	float fSize = DISC_SIZE * 0.5f;

	if (s_disc.pos.y >= SCREEN_HEIGHT - fSize)
	{// 下
		s_disc.pos.y = SCREEN_HEIGHT - fSize;
		s_disc.move.y *= -1.0f;
	}
	else if (s_disc.pos.y <= 0.0f + fSize)
	{// 上
		s_disc.pos.y = 0.0f + fSize;
		s_disc.move.y *= -1.0f;
	}

	if (s_disc.pos.x >= SCREEN_WIDTH - fSize)
	{// 右
		s_disc.pos.x = SCREEN_WIDTH - fSize;
		s_disc.move.x *= -1.0f;
	}
	else if (s_disc.pos.x <= 0.0f + fSize)
	{// 左
		s_disc.pos.x = 0.0f + fSize;
		s_disc.move.x *= -1.0f;
	}
}
