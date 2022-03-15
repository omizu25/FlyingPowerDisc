//====================
//プレイヤー設定
//autho hamada ryuuga
//====================

#include "main.h"
#include "Player.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "rectangle.h"
#include "wall.h"
#include "game.h"
#include "effect.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h>

#define MAXPLAYERTYPE (4)//Type最大数
#define MOVESPEED (5.0f)
#define DEAD_ZONE	(0.1f)		// スティックの遊び
#define MAX_DIVECOUNT (15)		// ダイブの硬直時間
#define MAX_RESET_SPEED (5.0f)	// リセット状態の速さ
#define MAX_HAVE_COUNT (120)	// 持ってる時間の最大値
#define DISC_SPEED (2.0f)		// ディスクの速さ
//スタティック変数///スタティックをヘッタに使うなよ？

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //テクスチャのポインタ
static Player s_Player[MAXPLAYER];//プレイヤー構造体取得
static Player s_PlayerType[MAXPLAYERTYPE];//プレイヤーのTypeを保存する
static bool	s_bKeyBoardWASD;			// WASDのキーボード入力があるかどうか
static bool	s_bKeyBoardArrow;			// やじるしのキーボード入力があるかどうか
static bool	s_bJoyPad[MAXPLAYER];		// ジョイパッド入力があるかどうか
static bool	s_bStickLeft[MAXPLAYER];	// 左スティック入力があるかどうか

// スタティック関数プロトタイプ宣言
static void UpdateNormal(void);
static void UpdateReset(void);
static void InputMove(void);

//=======================
//プレイヤーの初期化設定
//=======================
void InitPlayer(void)
{
	LoadFile("data\\txt\\Status.txt");

	for (int count = 0; count < MAXPLAYER; count++)
	{
		
		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bHave = false;
		s_Player[count].bUse = false;
		s_Player[count].bDive = false;
		s_Player[count].fheight = PLAYERSIZE_Y;
		s_Player[count].fwidth = PLAYERSIZE_X;
		s_Player[count].nSkillCount = 0;
		s_Player[count].nDiveCount = 0;
		s_Player[count].nHaveCount = 0;
		// 矩形の設定
		s_Player[count].nIdx = SetRectangleWithTex(s_pTexturePlayer[count]);
	}
	SetPlayer(D3DXVECTOR3((float)PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 0,true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 1,false);
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

	for (int count = 0; count < MAXPLAYER; count++)
	{
		// 使うのを止める
		StopUseRectangle(s_Player[count].nIdx);
	}
}

//===================
//更新処理
//===================
void UpdatePlayer(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_START:	// 開始状態
		break;

	case GAMESTATE_NORMAL:	// 通常状態
		UpdateNormal();
		break;

	case GAMESTATE_RESET:	// リセット状態
		UpdateReset();
		break;

	case GAMESTATE_END:		// 終了状態

		break;
	case GAMESTATE_RESULT:	// リザルト状態

		break;

	case GAMESTATE_NONE:	// 何もしていない状態
	default:
		assert(false);
		break;
	}
}
//===================
//描画処理
//===================
void DrawPlayer(void)
{
}

//===================
//セット
//===================
void SetPlayer(D3DXVECTOR3 pos, int nType,bool light)
{
	for (int count = 0; count< MAXPLAYER; count++)
	{
		Player *pPlayer = &s_Player[count];

		if (s_Player[count].bUse)
		{
			continue;
		}
		s_Player[count].nType = nType;
		s_Player[count].bUse = true;
		s_Player[count].pos = pos;

		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = s_PlayerType[nType].Speed;
		s_Player[count].Pow = s_PlayerType[nType].Pow;
		s_Player[count].fheight = PLAYERSIZE_Y;
		s_Player[count].fwidth = PLAYERSIZE_X;
		s_Player[count].bHave = false;
		if (light)
		{
			D3DXVECTOR2 texU(1.0f, 0.0f);
			D3DXVECTOR2 texV(0.0f, 1.0f);

			// 矩形のテクスチャ座標の設定
			SetTexRectangle(pPlayer->nIdx, texU, texV);
		}
		else
		{
			D3DXVECTOR2 texU(0.0f, 1.0f);
			D3DXVECTOR2 texV(0.0f, 1.0f);

			// 矩形のテクスチャ座標の設定
			SetTexRectangle(pPlayer->nIdx, texU, texV);
		}

		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
		
		break;
	}
}

//----------------------------
//Playerのステータス読み込み
//----------------------------
void LoadFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");
	int number = 0;

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
						fscanf(pFile, "%f", &s_PlayerType[number].Pow);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_PlayerType[number].Speed);
					}
					if (strcmp(&s_aString[0], "ENDSET") == 0)
					{
						number++;
						break;
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

//----------------------------
//Player動き
//----------------------------
void MovePlayer(void)
{
	// 移動の入力があるかないか
	InputMove();

	Disc *pDisc = GetDisc();

	
	//---------------------------------------
	//１体目の行動
	//----------------------------------------
	if (!s_Player[0].bHave)
	{// ディスクを持っていない
		
		if (s_Player[0].bDive == true && pDisc->nThrow == 1)
		{//タックル適用時
			Player *pPlayer = &s_Player[0];

			float fHeight = ((PLAYERSIZE_Y * 0.5f));
			float fWidth = ((PLAYERSIZE_X * 0.5f));

			if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
				(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
				(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
				(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
			{// プレイヤーにディスクが当たった時
				
				pDisc->nThrow = 0;
				s_Player[0].bDive = false;
				s_Player[0].nDiveCount = 0;
				pDisc->move = D3DXVECTOR3(DISC_SPEED, 0.0f, 0.0f)*s_Player[0].Pow * 3;
			}
		}
		
		if (s_Player[0].bDive == true)
		{
			s_Player[0].nDiveCount++;

			if (s_Player[0].nDiveCount >= MAX_DIVECOUNT)
			{
				s_Player[0].nDiveCount = 0;
				s_Player[0].bDive = false;
			}
		}
		else if (GetKeyboardTrigger(DIK_C) || GetJoypadIdxPress(JOYKEY_A, 0))
		{//タックル
			s_Player[0].pos.x += s_Player[0].Speed * 5;
			s_Player[0].bDive = true;
			s_Player[0].nDiveCount = 0;
		}
		else if (s_bKeyBoardWASD)
		{// キーボード
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetKeyboardPress(DIK_A))
			{// キーが押された
				vec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_D))
			{// キーが押された
				vec.x += 1.0f;
			}
			if (GetKeyboardPress(DIK_W))
			{// キーが押された
				vec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_S))
			{// キーが押された
				vec.y += 1.0f;
			}

			// ベクトルの正規化
			D3DXVec3Normalize(&vec, &vec);

			s_Player[0].move = vec * s_Player[0].Speed;
		}
		else if (s_bJoyPad[0])
		{// ジョイパッド
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetJoypadIdxPress(JOYKEY_LEFT, 0))
			{// ボタンが押された
				vec.x -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_RIGHT, 0))
			{// ボタンが押された
				vec.x += 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_UP, 0))
			{// ボタンが押された
				vec.y -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_DOWN, 0))
			{// ボタンが押された
				vec.y += 1.0f;
			}

			// ベクトルの正規化
			D3DXVec3Normalize(&vec, &vec);

			s_Player[0].move = vec * s_Player[0].Speed;
		}
		else if (s_bStickLeft[0])
		{// 左スティック
			D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
			stick.y = GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

			// ベクトルの正規化
			D3DXVec3Normalize(&stick, &stick);

			s_Player[0].move = stick * s_Player[0].Speed;
		}
	}
	else
	{// ディスクを持っている
		s_Player[0].nHaveCount++;
		if (GetKeyboardPress(DIK_SPACE) || GetJoypadIdxPress(JOYKEY_A, 0)|| s_Player[0].nHaveCount >= MAX_HAVE_COUNT)
		{//ここに玉投げる動作（パワーを玉の速度にするといいんじゃないかな）
			s_Player[0].bHave = false;
			pDisc->nThrow = 0;
			//タイミングのによって速度変えるやつ
			int Ross = s_Player[0].nHaveCount / 10;
			pDisc->move = D3DXVECTOR3(DISC_SPEED - Ross*0.1f, 0.0f, 0.0f)*s_Player[0].Pow;
			pDisc->bHave = false;
			s_Player[0].nHaveCount = 0;
		}
	}

	//---------------------------------------
	//２体目の行動
	//----------------------------------------
	if (!s_Player[1].bHave)
	{// ディスクを持っていない
		if (s_Player[1].bDive == true && pDisc->nThrow == 0)
		{//タックル適用時
			Player *pPlayer = &s_Player[1];

			float fHeight = ((PLAYERSIZE_Y * 0.5f));
			float fWidth = ((PLAYERSIZE_X * 0.5f));

			if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
				(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
				(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
				(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
			{// プレイヤーにディスクが当たった時

				pDisc->nThrow = 1;
				s_Player[1].bDive = false;
				s_Player[1].nDiveCount = 0;
				pDisc->move = D3DXVECTOR3(-DISC_SPEED, 0.0f, 0.0f)*s_Player[1].Pow * 3;
			}
		}

		if (s_Player[1].bDive == true)
		{
			s_Player[1].nDiveCount++;

			if (s_Player[1].nDiveCount >= MAX_DIVECOUNT)
			{
				s_Player[1].nDiveCount = 0;
				s_Player[1].bDive = false;
			}
		}
		else if (GetKeyboardTrigger(DIK_L) || GetJoypadIdxPress(JOYKEY_A, 1))
		{//タックル
			s_Player[1].pos.x -= s_Player[1].Speed * 5;
			s_Player[1].bDive = true;
			s_Player[1].nDiveCount = 0;
		}
		else if (s_bKeyBoardArrow)
		{// キーボード
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetKeyboardPress(DIK_LEFT))
			{// キーが押された
				vec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{// キーが押された
				vec.x += 1.0f;
			}
			if (GetKeyboardPress(DIK_UP))
			{// キーが押された
				vec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{// キーが押された
				vec.y += 1.0f;
			}

			// ベクトルの正規化
			D3DXVec3Normalize(&vec, &vec);

			s_Player[1].move = vec * s_Player[1].Speed;
		}
		else if (s_bJoyPad[1])
		{// ジョイパッド
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetJoypadIdxPress(JOYKEY_LEFT, 1))
			{// ボタンが押された
				vec.x -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_RIGHT, 1))
			{// ボタンが押された
				vec.x += 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_UP, 1))
			{// ボタンが押された
				vec.y -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_DOWN, 1))
			{// ボタンが押された
				vec.y += 1.0f;
			}

			// ベクトルの正規化
			D3DXVec3Normalize(&vec, &vec);

			s_Player[1].move = vec * s_Player[1].Speed;
		}
		else if (s_bStickLeft[1])
		{// 左スティック
			D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 1).x;
			stick.y = GetJoypadStick(JOYKEY_LEFT_STICK, 1).y;

			// ベクトルの正規化
			D3DXVec3Normalize(&stick, &stick);

			s_Player[1].move = stick * s_Player[1].Speed;
		}
	}
	else
	{// ディスクを持っている
		s_Player[1].nHaveCount++;
		if (GetKeyboardPress(DIK_RETURN) || GetJoypadIdxPress(JOYKEY_A, 1) || s_Player[1].nHaveCount >= MAX_HAVE_COUNT)
		{//ここに玉投げる動作（パワーを玉の速度にするといいんじゃないかな）
			s_Player[1].bHave = false;
			pDisc->nThrow = 1;
			//タイミングのによって速度変えるやつ
			int Ross = s_Player[1].nHaveCount / 10;
			pDisc->move = D3DXVECTOR3(-DISC_SPEED + Ross*0.1f, 0.0f, 0.0f) * s_Player[1].Pow;
			pDisc->bHave = false;
			s_Player[1].nHaveCount = 0;
		}
	}
}

//----------------------------
//Player当たり判定
//----------------------------
bool CollisionPlayer(Disc *pDisc, float Size, int number)
{
	bool bIsLanding = false;
	Player *pPlayer = &s_Player[number];

	float fDiscSize = Size * 0.0f;
	float fHeight = (fDiscSize + (PLAYERSIZE_Y * 0.5f));
	float fWidth = (fDiscSize + (PLAYERSIZE_X * 0.5f));

	if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
		(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
		(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
		(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
	{// プレイヤーにディスクが当たった時
		switch (number)
		{
		case 0:
			pDisc->pos.x = s_Player[number].pos.x + fWidth;
			break;

		case 1:

			pDisc->pos.x = s_Player[number].pos.x - fWidth;
			break;

		default:
			assert(false);
			break;
		}
		pDisc->pos.y = s_Player[number].pos.y;
		pDisc->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pDisc->bHave = true;
		s_Player[number].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[number].nSkillCount++;
		if (s_Player[number].nSkillCount >= 10)
		{
			s_Player[number].nSkillCount = 0;
			s_Player[number].bSkill = true;
		}
		s_Player[number].bHave = true;
	
		bIsLanding = true;
	}

	return bIsLanding;
}

//----------------------------
//Player情報を取得
//----------------------------
Player* GetPlayer(void)
{
	return s_Player;
}

//----------------------------
// 通常状態の更新
//----------------------------
static void UpdateNormal(void)
{
	MovePlayer();

	for (int count = 0; count < MAXPLAYER; count++)
	{
		Player *pPlayer = &s_Player[count];

		//移動量を更新(減衰させる)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//（目的の値-現在の値）＊減衰係数											  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//（目的の値-現在の値）＊減衰係数

			//前回の位置の保存
		s_Player[count].posOld = s_Player[count].pos;
		//更新
		s_Player[count].pos.x += s_Player[count].move.x;
		s_Player[count].pos.y += s_Player[count].move.y;

		//壁---------------------------------------------------
		if (s_Player[count].pos.x <= 0.0f + pPlayer->fwidth / 2.0f)
		{//横壁（左）
			s_Player[count].pos.x = 0.0f + pPlayer->fwidth / 2.0f;
		}
		else if (s_Player[count].pos.x >= SCREEN_WIDTH - pPlayer->fwidth / 2.0f)
		{//横壁（右）
			s_Player[count].pos.x = SCREEN_WIDTH - pPlayer->fwidth / 2.0f;
		}
		if (s_Player[count].pos.y <= 0.0f + pPlayer->fheight / 2.0f+ WALLSIZ_Y /2)
		{//上壁　
			s_Player[count].pos.y = 0.0f + pPlayer->fheight / 2.0f + WALLSIZ_Y / 2;
		}
		if (s_Player[count].pos.y >= SCREEN_HEIGHT - pPlayer->fheight / 2.0f - WALLSIZ_Y / 2)
		{//下壁
			s_Player[count].pos.y = SCREEN_HEIGHT - pPlayer->fheight / 2.0f - WALLSIZ_Y / 2;
		}
		//真ん中ライン
		if (s_Player[0].pos.x >= SCREEN_WIDTH* 0.5f - pPlayer->fwidth / 2.0f)
		{
			s_Player[0].pos.x = SCREEN_WIDTH* 0.5f - pPlayer->fwidth / 2.0f;
		}
		if (s_Player[1].pos.x <= SCREEN_WIDTH * 0.5f + pPlayer->fwidth / 2.0f)
		{
			s_Player[1].pos.x = SCREEN_WIDTH * 0.5f + pPlayer->fwidth / 2.0f;
		}
		//skill使用可能な時のエフェクト
		if (s_Player[count].bSkill)
		{
			SetEffect(D3DXVECTOR3(s_Player[count].pos.x, s_Player[count].pos.y - 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_SHOOT, 10, 200.0f);
		}
		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}
}

//----------------------------
// リセット状態の更新
//----------------------------
static void UpdateReset(void)
{
	bool bOverlap[MAXPLAYER];

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		Player *pPlayer = &s_Player[nPlayerNo];

		bOverlap[nPlayerNo] = false;

		D3DXVECTOR3 posDest;
		
		switch (nPlayerNo)
		{
		case 0:
			posDest = D3DXVECTOR3(PLAYERSIZE_X, SCREEN_HEIGHT * 0.5f, 0.0f);
			break;

		case 1:
			posDest = D3DXVECTOR3(SCREEN_WIDTH - PLAYERSIZE_X, SCREEN_HEIGHT * 0.5f, 0.0f);
			break;
		
		default:
			posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			assert(false);
			break;
		}

		// ホーミング
		bOverlap[nPlayerNo] = Homing(&pPlayer->pos, pPlayer->pos, posDest, MAX_RESET_SPEED);
		
		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}

	if (bOverlap[0] && bOverlap[1])
	{// どちらも指定の位置
		// ゲームの状態の設定
		SetGameState(GAMESTATE_START);
	}
}

//----------------------------
// 移動の入力があるかないか
//----------------------------
static void InputMove(void)
{
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;

	for (int i = 0; i < MAXPLAYER; i++)
	{
		s_bJoyPad[i] = false;
		s_bStickLeft[i] = false;
	}

	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) ||
		GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
	{// キーが押された
		s_bKeyBoardWASD = true;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// キーが押された
		s_bKeyBoardArrow = true;
	}

	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (GetJoypadIdxPress(JOYKEY_LEFT, i) || GetJoypadIdxPress(JOYKEY_RIGHT, i) ||
			GetJoypadIdxPress(JOYKEY_UP, i) || GetJoypadIdxPress(JOYKEY_DOWN, i))
		{// ボタンが押された
			s_bJoyPad[i] = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, i).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).y < -DEAD_ZONE)
		{// 左スティックが傾いた
			s_bStickLeft[i] = true;
		}
	}
}
