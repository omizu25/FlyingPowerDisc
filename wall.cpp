//====================
//かべ設定
//autho hamada ryuuga
//====================

#include"main.h"
#include"wall.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include <stdio.h>

#define MAXWALL			(2)//登場最大数
#define MAXWALLTYPE		(4)//Type最大数
#define MOVESPEED		(5.0f)
#define WALLWIDE		(30.0f)
//スタティック変数///スタティックをヘッタに使うなよ？

static LPDIRECT3DTEXTURE9 s_pTextureWall[MAXWALLTYPE] = {}; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffWall = NULL; //頂点バッファの設定
static D3DXVECTOR3	s_rotWall;	//向き

//計算用関数
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);

 //=======================
 //かべの初期化設定
 //=======================
void InitWall(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	 //デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAXWALL * 4,//ここは確保するバッファのサイズsizeof(VERTEX_2D)*でここはきまる
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffWall,
		NULL);
	
	VERTEX_2D*pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count< MAXWALL; count++,pVtx+=4)
	{

		if (count == 0)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT - WALLWIDE, SCREEN_HEIGHT);
		}
		if (count == 1)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, 0.0f , 0.0f+ WALLWIDE);
		}

		//RHWの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



		//頂点バッファをアンロック
		s_pVtxBuffWall->Unlock();
	}
}

//===================
//終了処理
//===================
void UninitWall(void)
{
	//サウンド停止
	StopSound();

	//テクスチャの破棄
	for (int Tex = 0; Tex < MAXWALLTYPE; Tex++)
	{
		//テクスチャの破棄
		if (s_pTextureWall[Tex] != NULL)
		{
			s_pTextureWall[Tex]->Release();
			s_pTextureWall[Tex] = NULL;
		}
	}


	//頂点バッファの破棄
	if (s_pVtxBuffWall != NULL)
	{
		s_pVtxBuffWall->Release();
		s_pVtxBuffWall = NULL;
	}

}

//===================
//更新処理
//===================
void UpdateWall(void)
{

	VERTEX_2D*pVtx; //頂点へのポインタ
					//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);


	MoveWall();

	for (int count = 0; count < MAXWALL; count++, pVtx += 4)
	{

	}
	//頂点バッファをアンロック
	s_pVtxBuffWall->Unlock();
}
//===================
//描画処理
//===================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ


											//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffWall, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int count = 0; count < MAXWALL; count++)
	{
		
			//テクスチャの設定
			pDevice->SetTexture(0, NULL); //);

																			 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, count * 4, 2);
		
	}
}

//----------------------------
//Wall動き
//----------------------------
bool CollisionWall(VERTEX_2D *pVtx, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	VERTEX_2D*pVtx; //頂点へのポインタ
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool bIsLanding = false;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
			//V1 move
			D3DXVECTOR3 vecMove = *pPos - *pPosOld;

			D3DXVECTOR3 vecMoveN;

			D3DXVec3Normalize(&vecMoveN, &vecMove);

			//ベクトルS2 V2
			D3DXVECTOR3 vecWall = D3DXVECTOR3(WORLD[1]._41, WORLD[1]._42, WORLD[1]._43) - D3DXVECTOR3(WORLD[0]._41, WORLD[0]._42, WORLD[0]._43) - (vecMoveN*Siz.z);

			//ベクトル現在のPOSと始点までの距離
			D3DXVECTOR3 vecPos = *pPos - D3DXVECTOR3(WORLD[0]._41, WORLD[0]._42, WORLD[0]._43);

			//外積計算
			float vecLine = Vec2Cross(&vecPos, &vecWall);

			if (vecLine <= 0.0f)
			{
				bIsLanding = true;

				//ベクトルV
				D3DXVECTOR3 vecOld = D3DXVECTOR3(WORLD[0]._41, WORLD[0]._42, WORLD[0]._43) - *pPosOld;

				//V3 
				D3DXVECTOR3 vecV3 = D3DXVECTOR3(WORLD[0]._41, WORLD[0]._42, WORLD[0]._43) - D3DXVECTOR3(WORLD[2]._41, WORLD[2]._42, WORLD[2]._43);

				//法線
				D3DXVECTOR3 vecB;

				//AとBの法線を求めるやつ
				D3DXVec3Cross(&vecB, &vecWall, &vecV3);

				//vecB をノーマライズして、長さ 1にする。
				D3DXVec3Normalize(&vecB, &vecB);

				D3DXVECTOR3 reversevecMove = -vecMove;
				//AとBの内積を求める
				float Dot = Vec2Dot(&reversevecMove, &vecB);

				//方向
				D3DXVECTOR3 vecC = vecB*Dot;

				//計算用の箱
				float vecCalculation[2];

				//T1求めるやつ
				vecCalculation[0] = Vec2Cross(&vecOld, &vecWall);
				vecCalculation[1] = Vec2Cross(&vecMove, &vecWall);

				//posOldから交点までの距離
				float t1 = vecCalculation[0] / vecCalculation[1];

				//T2求めるやつ
				vecCalculation[0] = Vec2Cross(&vecOld, &vecMove);
				vecCalculation[1] = Vec2Cross(&vecMove, &vecWall);

				//壁の視点から交点までの距離
				float t2 = vecCalculation[0] / vecCalculation[1];

				//差
				float eps = 0.00001f;

				if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
				{//交差してないときの判
				}
				else
				{//交差してるとき
					pPos->x = (pPosOld->x + vecMove.x*t1) + (vecC.x*0.5f);
					pPos->z = (pPosOld->z + vecMove.z*t1) + (vecC.z*0.5f);

					if (GetKeyboardPress(DIK_B))
					{//壁走り
						pPos->y += 1.0f;
					}
				}
			}
		}
		pVtx += 4;
	}
	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

	return bIsLanding;

}

void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}