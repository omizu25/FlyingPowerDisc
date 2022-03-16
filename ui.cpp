//========================================================
//
//UI処理
//Author:髙野馨將
//
//========================================================
#include "ui.h"
#include "game.h"
//====================================
//マクロ定義
//====================================
#define MAX_UI		128				//UIの最大数
#define NUM_UI		10				//UIの種類
#define MAX_TIME	60				//出ている時間
//====================================
//グローバル変数
//====================================
LPDIRECT3DTEXTURE9 g_pTextureUi[NUM_UI] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	//頂点バッファへのポインタ
Ui g_aUi[MAX_UI];		//UIの情報
//====================================
//メイン関数
//====================================

//====================================
//UIの初期化処理
//====================================
void InitUi(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI000.png",
		&g_pTextureUi[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI001.png",
		&g_pTextureUi[1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAME\\FivePoints.png",
		&g_pTextureUi[2]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAME\\ThreePoints.png",
		&g_pTextureUi[3]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI001.png",
		&g_pTextureUi[4]);
	//UIの情報の初期化
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		g_aUi[nCntUi].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].nCntTime = 0.0f;
		g_aUi[nCntUi].fHeight = 0.0f;
		g_aUi[nCntUi].fWidth = 0.0f;
		g_aUi[nCntUi].nType = 0;
		g_aUi[nCntUi].bUse = false;		//使用していない状態にする
		g_aUi[nCntUi].bSwitch = false;	//消えていく状態にする
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffUi->Unlock();
}
//====================================
//UIの終了処理
//====================================
void UninitUi(void)
{
	//テクスチャの破棄
	if (g_pTextureUi[NUM_UI] != NULL)
	{
		g_pTextureUi[NUM_UI]->Release();
		g_pTextureUi[NUM_UI] = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}
}
//====================================
//UIの更新処理
//====================================
void UpdateUi(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		//種類ごとの動き(左から出る)
		if (g_aUi[nCntUi].nType == 2 && g_aUi[nCntUi].bSwitch == false)
		{//出てくる処理
			g_aUi[nCntUi].scale.x += 0.01f;
			//拡大率の調整
			if (g_aUi[nCntUi].scale.x > 1.0f)
			{
				g_aUi[nCntUi].scale.x = 1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 2 && g_aUi[nCntUi].bSwitch)
		{//消える処理
			g_aUi[nCntUi].scale.x -= 0.01f;
		}

		//種類ごとの動き(右から出る)
		if (g_aUi[nCntUi].nType == 3 && g_aUi[nCntUi].bSwitch == false)
		{//出てくる処理
			g_aUi[nCntUi].scale.x -= 0.01f;
			//拡大率の調整
			if (g_aUi[nCntUi].scale.x < -1.0f)
			{
				g_aUi[nCntUi].scale.x = -1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 3 && g_aUi[nCntUi].bSwitch)
		{//消える処理
			g_aUi[nCntUi].scale.x += 0.01f;
		}

		//種類ごとの動き(上下に伸びる)
		if (g_aUi[nCntUi].nType == 4 && g_aUi[nCntUi].bSwitch == false)
		{//出てくる処理
			g_aUi[nCntUi].scale.y += 0.01f;
			//拡大率の調整
			if (g_aUi[nCntUi].scale.y > 1.0f)
			{
				g_aUi[nCntUi].scale.y = 1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 4 && g_aUi[nCntUi].bSwitch)
		{//消える処理
			g_aUi[nCntUi].scale.y -= 0.01f;

			if (g_aUi[nCntUi].scale.y <= 0.0f)
			{
				// ゲームの状態の設定
				SetGameState(GAMESTATE_START);
			}
		}

		//拡大率の調整
		if (g_aUi[nCntUi].scale.x > 1.0f)
		{
			g_aUi[nCntUi].scale.x = 1.0f;
			g_aUi[nCntUi].bSwitch = true;
		}
		else if(g_aUi[nCntUi].scale.y > 1.0f)
		{
			g_aUi[nCntUi].scale.y = 1.0f;
			g_aUi[nCntUi].bSwitch = true;
		}
		if (g_aUi[nCntUi].scale.x < -1.0f)
		{
			g_aUi[nCntUi].scale.x = -1.0f;
		}
		else if (g_aUi[nCntUi].scale.y < -1.0f)
		{
			g_aUi[nCntUi].scale.y = -1.0f;
		}

		if (g_aUi[nCntUi].nType == 2)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
		}
		else if (g_aUi[nCntUi].nType == 3)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - (g_aUi[nCntUi].fWidth / 2 * -g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth  / 2, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - (g_aUi[nCntUi].fWidth / 2 * -g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth  / 2 , (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
		}
		else if (g_aUi[nCntUi].nType == 4)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2),							g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2),							g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
		}
		if (g_aUi[nCntUi].nType == 3)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffUi->Unlock();
}
//====================================
//UIの描画処理
//====================================
void DrawUi(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse)
		{//UIが使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureUi[g_aUi[nCntUi].nTexture]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);	//プリミティブ(ポリゴン)数
		}
	}
}
//====================================
//UIの設定処理
//====================================
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, D3DXVECTOR3 scale,int nTexture)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse == false)
		{//UIが使用されていない
			g_aUi[nCntUi].pos = pos;
			g_aUi[nCntUi].fWidth = fWidth;
			g_aUi[nCntUi].fHeight = fHeight;
			g_aUi[nCntUi].nType = nType;
			g_aUi[nCntUi].scale = scale;
			g_aUi[nCntUi].nTexture = nTexture;
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);

			g_aUi[nCntUi].bUse = true;		//使用している状態
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffUi->Unlock();
}
//====================================
//UI情報の取得
//====================================
Ui * GetUi(void)
{
	return &g_aUi[0];
}