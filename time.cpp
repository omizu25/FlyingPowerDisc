//================================
//
// タイムの処理
// Author:Seino Riku
//
//================================
#include "time.h"
#include "game.h"
#include "mode.h"
#include "score.h"
#include "rule.h"
#include "disc.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;
D3DXVECTOR3 g_posTime;
int g_nTime;				//最大時間を入れる
int g_TimenCnt;				//カウンター

//スコアの初期化処理
void InitTime(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;				//for文専用のカウント

								//頂点座標へのポインタ
	VERTEX_2D*pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GAME/Number_Yellow.png",
		&g_pTextureTime);

	// スコアの情報の初期化
	g_posTime = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (MAX_SIZE * 0.5f), SCREEN_HEIGHT * 0.1f, 0.0f);

	g_TimenCnt = 60;
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - MAX_SIZE / 2 + (nCntTime * MAX_SIZE), g_posTime.y - MAX_SIZE / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + MAX_SIZE / 2 + (nCntTime * MAX_SIZE), g_posTime.y - MAX_SIZE / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - MAX_SIZE / 2 + (nCntTime * MAX_SIZE), g_posTime.y + MAX_SIZE / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + MAX_SIZE / 2 + (nCntTime * MAX_SIZE), g_posTime.y + MAX_SIZE / 2, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//スコアの
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破壊
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//スコアの
void UpdateTime(void)
{
	if (GetGameState() != GAMESTATE_NORMAL)
	{
		g_TimenCnt = 60;
		return;
	}

	//デクリメントでカウント
	g_TimenCnt--;
	if (g_TimenCnt == 0)
	{
		g_TimenCnt = 60;
		AddTime(-1);
	}
	if (g_nTime <= 0)
	{
		if (GetPointScore(0) > GetPointScore(1))
		{
			// セット数の加算
			AddSetScore(0, 1);

			// 次に始めるプレイヤーの設定
			SetPossDisc(1);
		}
		else if (GetPointScore(0) < GetPointScore(1))
		{
			// セット数の加算
			AddSetScore(1, 1);

			// 次に始めるプレイヤーの設定
			SetPossDisc(0);
		}
		else
		{
			// 全てのセット数の加算
			AllAddSetScore(1);

			if (GetSetScore(0) > GetSetScore(1))
			{
				// 次に始めるプレイヤーの設定
				SetPossDisc(1);
			}
			else if (GetSetScore(0) < GetSetScore(1))
			{
				// 次に始めるプレイヤーの設定
				SetPossDisc(0);
			}
			else
			{
				// 次に始めるプレイヤーの設定
				SetPossDisc(0);
			}
		}

		// ポイント数を0にする
		ZeroPointScore(0);
		ZeroPointScore(1);

		// 時間のリセット
		SetTime(GetTimeRule());

		// ゲームの状態の設定
		SetGameState(GAMESTATE_RESET);

		// ディスクのリセット
		ResetDisc();
	}
}

//スコアの
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntTime * 4,
			2);
	}
}

//スコアの
void SetTime(int nTime)
{
	//頂点座標へのポインタ
	VERTEX_2D *pVtx;
	int nNumber[MAX_TIME];
	int nCntTime;

	g_nTime = nTime;

	//nNumber[0] = g_nTime % 1000 / 100;
	nNumber[0] = g_nTime % 100 / 10;
	nNumber[1] = g_nTime % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();

}

//スコアの
void AddTime(int nValue)
{
	//頂点座標へのポインタ
	VERTEX_2D*pVtx;
	int nNumber[MAX_TIME];	//各桁の数字を格納

	g_nTime += nValue;

	//nNumber[0] = g_nTime % 1000 / 100;
	nNumber[0] = g_nTime % 100 / 10;
	nNumber[1] = g_nTime % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//スコアの
int GetTime(void)
{
	return g_nTime;
}
