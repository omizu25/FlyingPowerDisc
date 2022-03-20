//**************************************************
//
// FPD制作 ( effect.cpp )
// Author  : hamada ryuuga
//
//**************************************************
#include"effect.h"
#include"player.h"
#include"rectangle.h"
#include"texture.h"

#include <assert.h>

#define NUR_EFFECT (4)

//スタティック
static Effect s_aEffect[MAX_EFFECT];
static TEXTURE s_aTexture[EFFECTSTATE_MAX];

//==================================
//初期化
//==================================
void InitEffect(void)
{
	s_aTexture[EFFECTSTATE_SPIN] = TEXTURE_spin;
	s_aTexture[EFFECTSTATE_SHOOT] = TEXTURE_fire;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//初期化忘れない
		s_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_aEffect[nCntEffect].nLife = 0;
		s_aEffect[nCntEffect].bUse = false;

		// 矩形の設定
		s_aEffect[nCntEffect].nIdx = SetRectangle(TEXTURE_NONE);

		// 矩形を描画するかどうか
		SetDrawRectangle(s_aEffect[nCntEffect].nIdx, false);

		// 矩形の加算合成をするかどうか
		SetAddRectangle(s_aEffect[nCntEffect].nIdx, true);
	}
}

//==================================
//破棄
//==================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 使うのを止める
		StopUseRectangle(s_aEffect[nCntEffect].nIdx);
	}
}

//==================================
//更新
//==================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D*pVtx; //頂点へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse)
		{
			// 頂点バッファの取得
			pVtxBuff = GetVtxBuffRectangle(s_aEffect[nCntEffect].nIdx);

			//頂点バッファをアンロック
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			s_aEffect[nCntEffect].pos += s_aEffect[nCntEffect].move;

			SetNormalpos2d(pVtx, s_aEffect[nCntEffect].pos.x - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.x + s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y + s_aEffect[nCntEffect].fRadeius);

			s_aEffect[nCntEffect].col.a -= 1.0f / s_aEffect[nCntEffect].nMaxLife;

			if (s_aEffect[nCntEffect].col.a <=0.0f)
			{
				s_aEffect[nCntEffect].bUse = false;
			}

			// テクスチャの設定
			SetVtxAnimTex(pVtx, &s_aEffect[nCntEffect].AnimTex);
			SetTex2d(pVtx, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)*s_aEffect[nCntEffect].AnimTex.nPatternX
				, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)* (s_aEffect[nCntEffect].AnimTex.nPatternX + 1)
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*s_aEffect[nCntEffect].AnimTex.nPatternY
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*(s_aEffect[nCntEffect].AnimTex.nPatternY + 1));

			//頂点カラーの設定
			pVtx[0].col = s_aEffect[nCntEffect].col;
			pVtx[1].col = s_aEffect[nCntEffect].col;
			pVtx[2].col = s_aEffect[nCntEffect].col;
			pVtx[3].col = s_aEffect[nCntEffect].col;

			//頂点バッファをアンロック
			pVtxBuff->Unlock();
		}
	}

	
}
void DrawEffect(void)
{
}

//==================================
//セット　引数　座標、色、使いたいタイプ、寿命、サイズ
//==================================
void SetEffect(D3DXVECTOR3 pos,  D3DXCOLOR col, EFFECTSTATE nType, int life, float size)
{
	assert(nType >= 0 && nType < EFFECTSTATE_MAX);

	int nCntEffect;
	VERTEX_2D*pVtx; //頂点へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!s_aEffect[nCntEffect].bUse)
		{
			//敵が使用されてない場合
			s_aEffect[nCntEffect].fRadeius = size;
			s_aEffect[nCntEffect].pos.x = pos.x;
			s_aEffect[nCntEffect].pos.y = pos.y;
			s_aEffect[nCntEffect].pos.z = pos.z;

			// 頂点バッファの取得
			pVtxBuff = GetVtxBuffRectangle(s_aEffect[nCntEffect].nIdx);

			//頂点バッファをアンロック
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

													//頂点座標
			SetNormalpos2d(pVtx, s_aEffect[nCntEffect].pos.x - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.x + s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y + s_aEffect[nCntEffect].fRadeius);

			s_aEffect[nCntEffect].nType = nType;
			s_aEffect[nCntEffect].nLife = life;
			s_aEffect[nCntEffect].bUse = true;
			s_aEffect[nCntEffect].col = col;
			s_aEffect[nCntEffect].nMaxLife = life;

			switch (nType)
			{
			case EFFECTSTATE_SHOOT:
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 10;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 1;
				break;
			case EFFECTSTATE_SPIN:
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 5;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 3;
				break;
			default:
				assert(false);
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 1;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 1;
				break;
			}

			SetTex2d(pVtx, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)*s_aEffect[nCntEffect].AnimTex.nPatternX
				, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)* (s_aEffect[nCntEffect].AnimTex.nPatternX + 1)
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*s_aEffect[nCntEffect].AnimTex.nPatternY
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*(s_aEffect[nCntEffect].AnimTex.nPatternY + 1));

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			//頂点バッファをアンロック
			pVtxBuff->Unlock();

			// 矩形を描画するかどうか
			SetDrawRectangle(s_aEffect[nCntEffect].nIdx, true);

			// 矩形のテクスチャの変更
			ChangeTextureRectangle(s_aEffect[nCntEffect].nIdx, s_aTexture[nType]);

			break;
		}
	}
}

//=========================================
// 座標設定
//Auther：hamada ryuuga
//=========================================
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}
//---------------------------------------
//セットテクスチャ(2d)
//Auther：hamada ryuuga
//---------------------------------------
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}

//--------------------------------------------------
// テクスチャアニメーション
// Author：Hamada Ryuga
//--------------------------------------------------
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex)
{
	pAnimTex->nCnt++;
	pAnimTex->nSpeed = 5;//ここで速度調整
	if ((pAnimTex->nCnt % pAnimTex->nSpeed) == 0)
	{

		pAnimTex->nPatternX++;

		if (pAnimTex->nPatternX >= pAnimTex->nDivisionX)
		{
			pAnimTex->nPatternX = 0;
			pAnimTex->nPatternY++;
		}
		if (pAnimTex->nPatternY >= pAnimTex->nDivisionY)
		{
			pAnimTex->nPatternY = 0;
		}

		SetTex2d(pVtx,
			1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX
			, 1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX + (1.0f / pAnimTex->nDivisionX)
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY + 1.0f / pAnimTex->nDivisionY);
	}
}
