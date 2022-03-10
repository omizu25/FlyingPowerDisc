//====================
//プレイヤー設定
//autho hamada ryuuga
//====================

#include"main.h"
#include"Player.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include <stdio.h>

#define MAXPLAYER (2)
#define MAXPLAYERTYPE (4)
#define MOVESPEED (5.0f)
//スタティック変数///スタティックをヘッタに使うなよ？

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPlayer = NULL; //頂点バッファの設定
static D3DXVECTOR3	s_rotPlayer;	//向き
static float s_fLengthPlayer;      //対角線ノ長さ
static float s_fAnglePlayer;		//対角線角度
static float s_fLengthAttack;      //対角線ノ長さ
static float s_fAngleAttack;		//対角線角度
static Player s_Player[MAXPLAYER];//プレイヤー構造体取得
static Player s_PlayerType[MAXPLAYERTYPE];//プレイヤー構造体取得Type別


//=======================
//プレイヤーの初期化設定
//=======================
void InitPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ

							  //デバイスの取得
	pDevice = GetDevice();

	LoadFile("data\\txt\\Status.txt");

	//対角線の長さを算出する
	s_fLengthAttack = sqrtf(PLAYERSIZ_X * PLAYERSIZ_X + PLAYERSIZ_Y * PLAYERSIZ_Y) / 2.0f;

	//対角線の角度を算出する
	s_fAngleAttack = atan2f(PLAYERSIZ_X, PLAYERSIZ_Y);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAXPLAYER *4,//ここは確保するバッファのサイズsizeof(VERTEX_2D)*でここはきまる
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffPlayer,
		NULL);

	for (int count = 0; count < MAXPLAYER; count++)
	{
		
		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bUse = false;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;

	}
	SetPlayer(D3DXVECTOR3((float)0, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 0,true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 1,false);
}

//===================
//終了処理
//===================
void UninitPlayer(void)
{
	//サウンド停止
	StopSound();

	//テクスチャの破棄
	for (int Tex = 0; Tex < MAXPLAYERTYPE; Tex++)
	{
		//テクスチャの破棄
		if (s_pTexturePlayer[Tex] != NULL)
		{
			s_pTexturePlayer[Tex]->Release();
			s_pTexturePlayer[Tex] = NULL;
		}
	}


	//頂点バッファの破棄
	if (s_pVtxBuffPlayer != NULL)
	{
		s_pVtxBuffPlayer->Release();
		s_pVtxBuffPlayer = NULL;
	}

}

//===================
//更新処理
//===================
void UpdatePlayer(void)
{

	VERTEX_2D*pVtx; //頂点へのポインタ
					//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);


	for (int count = 0; count < MAXPLAYER; count++,pVtx += 4)
	{
	

		if (count == 0)
		{
			//行動
			if (GetKeyboardPress(DIK_W))
			{
				s_Player[0].move.y = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_A))
			{
				s_Player[0].move.x = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_S))
			{
				s_Player[0].move.y = s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_D))
			{
				s_Player[0].move.x = s_Player[count].Speed;
			}

		}
		else
		{
			if (GetKeyboardPress(DIK_UP))
			{
				s_Player[1].move.y = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				s_Player[1].move.x = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				s_Player[1].move.y = s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				s_Player[1].move.x = s_Player[count].Speed;
			}
		}

		//移動量を更新(減衰させる)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//（目的の値-現在の値）＊減衰係数											  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//（目的の値-現在の値）＊減衰係数

			//前回の位置の保存
			s_Player[count].posOld = s_Player[count].pos;
			//更新
			s_Player[count].pos.x += s_Player[count].move.x;
			s_Player[count].pos.y += s_Player[count].move.y;

			//壁
			if (s_Player[count].pos.x <= 0.0f + PLAYERMOVE)
			{//横壁（左）
				 s_Player[count].pos.x = 0.0f + PLAYERMOVE;
			}
			else if (s_Player[count].pos.x >= SCREEN_WIDTH - PLAYERMOVE)
			{//横壁（右）
				s_Player[count].pos.x = SCREEN_WIDTH - PLAYERMOVE;
			}
			if (s_Player[count].pos.y <= 0.0f + PLAYERMOVE)
			{//上壁　
				s_Player[count].pos.y = 0.0f + PLAYERMOVE;
			}
			if (s_Player[count].pos.y >= SCREEN_HEIGHT - PLAYERMOVE)
			{//下壁
				s_Player[count].pos.y = SCREEN_HEIGHT - PLAYERMOVE;
			}
			//真ん中ライン
			if (s_Player[0].pos.x >= SCREEN_WIDTH* 0.5f - PLAYERMOVE)
			{
				s_Player[0].pos.x = SCREEN_WIDTH* 0.5f - PLAYERMOVE;
			}
			if (s_Player[1].pos.x <= SCREEN_WIDTH * 0.5f + PLAYERMOVE)
			{
				s_Player[1].pos.x = SCREEN_WIDTH * 0.5f + PLAYERMOVE;
			}
			
			SetUp(pVtx,
				s_Player[count].pos.x,		//プレイヤー中心点ｘ
				s_Player[count].pos.y,		//プレイヤー中心点ｙ
				s_rotPlayer.z,		//プレイヤー回転
				s_fAngleAttack, 		//対角線角度
				s_fLengthAttack);	//対角線ノ長さ長さを算出する
	}
	//頂点バッファをアンロック
	s_pVtxBuffPlayer->Unlock();
}
//===================
//描画処理
//===================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int count = 0; count < MAXPLAYER; count++)
	{
		if (s_Player[count].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexturePlayer[s_Player[count].nType]); //);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, count*4, 2);

		}
	}
}
void SetPlayer(D3DXVECTOR3 pos, int nType,bool light)
{
	VERTEX_2D*pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count< MAXPLAYER; count++, pVtx += 4)
	{
		if (s_Player[count].bUse)
		{
			continue;
		}
		
		s_Player[count].nType = nType;
		s_Player[count].bUse = true;
		s_Player[count].pos = pos;

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

		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = s_PlayerType[nType].Speed;
		s_Player[count].Pow = s_PlayerType[nType].Pow;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;
	
		if (light)
		{
			//テクスチャの座標設定
			SetTex2d(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

		}
		else
		{//テクスチャの座標設定
			SetTex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);
			

		}
		SetUp(pVtx,
			s_Player[count].pos.x,		//プレイヤー中心点ｘ
			s_Player[count].pos.y,		//プレイヤー中心点ｙ
			s_rotPlayer.z,		//プレイヤー回転
			s_fAngleAttack, 		//対角線角度
			s_fLengthAttack);	//対角線ノ長さ長さを算出する
		break;
	}
	//頂点バッファをアンロック
	s_pVtxBuffPlayer->Unlock();

}
//------------------------------------
//中心点真ん中のPOSセット
//------------------------------------
void SetUp(VERTEX_2D *pVtx,
	float posx,		//中心点ｘ
	float posy,		//中心点ｙ
	float rotPlayer,		//プレイヤー回転
	float fAngleAttack 		//対角線角度
	, float fLengthAttack)	//対角線ノ長さ
{
	//回転
	pVtx[0].pos.x = posx + sinf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.y = posy + cosf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = posx + sinf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.y = posy + cosf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = posx + sinf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.y = posy + cosf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = posx + sinf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.y = posy + cosf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.z = 0.0f;

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
void LoadFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;
	int     MoveSet = 0;

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");
	int nCnt = 0;

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//これのあとコメント
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//＝読み込むやつ
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//デバイスの取得
				pDevice = GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTexturePlayer[Num_Tex]);
				Num_Tex++;
				
			}
			if (strcmp(&s_aString[0], "STATUSSET") == 0)
			{
				while(1)
				{
					fscanf(pFile, "%s", &s_aString[0]);

					if (strcmp(&s_aString[0], "ATTACKPOW") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_PlayerType[nCnt].Pow);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_PlayerType[nCnt].Speed);
					}
					if (strcmp(&s_aString[0], "ENDSET") == 0)
					{
						nCnt++;
						break;
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

void MovePlayer(void)
{
	//---------------------------------------
	//１体目の行動
	//----------------------------------------
	if (GetKeyboardPress(DIK_W))
	{
		s_Player[0].move.y = -s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_A))
	{
		s_Player[0].move.x = -s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_S))
	{
		s_Player[0].move.y = s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_D))
	{
		s_Player[0].move.x = s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_SPACE))
	{//ここに玉投げる動作（パワーを玉の速度にするといいんじゃないかな）
		
	}
	//---------------------------------------
	//２体目の行動
	//----------------------------------------
	if (GetKeyboardPress(DIK_UP))
	{
		s_Player[1].move.y = -s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		s_Player[1].move.x = -s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		s_Player[1].move.y = s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		s_Player[1].move.x = s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_RETURN))
	{//ここに玉投げる動作（パワーを玉の速度にするといいんじゃないかな）

	}

}
