//==================================================
//
// タイトル画面 ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "rectangle.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_LIGHT				(2)				// 後光の最大数
#define TITLE_POS_Y				(200.0f)		// タイトルのYの位置
#define TITLE_WIDTH				(900.0f)		// タイトルの幅
#define TITLE_HEIGHT			(200.0f)		// タイトルの高さ
#define LIGHT_WIDTH				(280.0f)		// ライトの幅
#define LIGHT_HEIGHT			(280.0f)		// ライトの高さ
#define CHANGE_SPEED			(0.005f)		// 収縮の速度
#define CHANGE_AMOUNT			(0.1f)			// 変化量
#define MEDIAN_LENGTH			(0.95f)			// 長さの中央値
#define MAX_ROTATION			(0.003f)		// 回転の最大値
#define DECREASE_SIZE			(0.6f)			// サイズの減少割合
#define DECREASE_SPEED			(0.5f)			// 速度の減少割合
#define MENU_WIDTH				(540.0f)		// メニューの幅
#define MENU_HEIGHT				(64.0f)			// メニューの高さ

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		// ゲーム
	MENU_MAX
}MENU;

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		rot;				// 向き
	float			fLength;			// 対角線の長さ
	float			fSaveLength;		// 初期化での対角線の長さを保存
	float			fAngle;				// 対角線の角度
	float			fSpeed;				// 速度
}Light;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTextureBG = NULL;			// 背景のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// 背景の頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;				// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureLight = NULL;			// 後光のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLight = NULL;			// 後光の頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureFrame = NULL;			// 枠のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		// メニューのテクスチャへのポインタ
static Light						s_light[MAX_LIGHT];				// 後光の情報
static int							s_nTime;						// 時間
static int							s_nSelectMenu;					// 選ばれているメニュー
static int							s_nIdxUseMenu;					// 使っているメニューの番号

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void UpdateLight(void);
static void Input(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTitle(void)
{
	// 矩形の初期化
	InitRectAngle();

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;
	s_nSelectMenu = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BG.png",
		&s_pTextureBG);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title_blue.png",
		&s_pTexture);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitleLight_red.png",
		&s_pTextureLight);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Frame.png",
		&s_pTextureFrame);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/New_GAMESTART_NoBG.png",
		&s_pTextureMenu[MENU_GAME]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIGHT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffLight,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

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
	s_pVtxBuffBG->Unlock();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	fWidth = TITLE_WIDTH * 0.5f;
	fHeight = TITLE_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

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

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffLight->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		pLight->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (i == 0)
		{//	奥
			fWidth = LIGHT_WIDTH;
			fHeight = LIGHT_HEIGHT;
			pLight->fSpeed = MAX_ROTATION * DECREASE_SPEED;
		}
		else
		{// 手前
			fWidth = LIGHT_WIDTH * DECREASE_SIZE;
			fHeight = LIGHT_HEIGHT * DECREASE_SIZE;
			pLight->fSpeed = MAX_ROTATION;
		}

		// 対角線の長さを算出する
		pLight->fSaveLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
		pLight->fLength = pLight->fSaveLength;

		// 対角線の角度を算出する
		pLight->fAngle = atan2f(fWidth, fHeight);

		// 頂点座標の設定
		pVtx[0].pos.x = pos.x + (sinf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.y = pos.y + (cosf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + (sinf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.y = pos.y + (cosf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + (sinf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.y = pos.y + (cosf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + (sinf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.y = pos.y + (cosf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f + (i * 1.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f + (i * -1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (i * 1.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f + (i * -1.0f), 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffLight->Unlock();

	// 矩形の設定
	SetRectAngle(&s_pTextureBG, &s_pVtxBuffBG, 1);

	SetRectAngle(&s_pTextureLight, &s_pVtxBuffLight, MAX_LIGHT);

	SetRectAngle(&s_pTexture, &s_pVtxBuff, 1);

	// メニューの初期化
	InitMenu();

	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = SCREEN_WIDTH * 0.25f;
	menu.fRight = SCREEN_WIDTH * 0.75f;
	menu.fTop = SCREEN_HEIGHT * 0.5f;
	menu.fBottom = SCREEN_HEIGHT;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	for (int i = 0; i < MENU_MAX; i++)
	{
		menu.pTexture[i] = &s_pTextureMenu[i];
	}

	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = WHITE_COLOR;
//	Frame.col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	Frame.pTexture = &s_pTextureFrame;
//	Frame.pTexture = NULL;
	
	// メニューの設定
	s_nIdxUseMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTitle(void)
{
	// 矩形の終了
	UninitRectAngle();

	// メニューの終了
	UninitMenu();

	if (s_pTextureBG != NULL)
	{// テクスチャの解放
		s_pTextureBG->Release();
		s_pTextureBG = NULL;
	}

	if (s_pVtxBuffBG != NULL)
	{// 頂点バッファの解放
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureLight != NULL)
	{// テクスチャの解放
		s_pTextureLight->Release();
		s_pTextureLight = NULL;
	}

	if (s_pTextureLight != NULL)
	{// 頂点バッファの解放
		s_pTextureLight->Release();
		s_pTextureLight = NULL;
	}

	if (s_pTextureFrame != NULL)
	{// テクスチャの解放
		s_pTextureFrame->Release();
		s_pTextureFrame = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{// テクスチャの解放
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTitle(void)
{
	// 後光
	UpdateLight();

	// 入力
	Input();

	// メニューの更新
	UpdateMenu();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTitle(void)
{
	// 矩形の描画
	DrawRectAngle();

	// メニューの描画
	DrawMenu();
}

//--------------------------------------------------
// 後光
//--------------------------------------------------
static void UpdateLight(void)
{
	s_nTime++;

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffLight->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		float fCurve = sinf((s_nTime * CHANGE_SPEED) * (D3DX_PI * 2.0f));

		pLight->fLength = pLight->fSaveLength * ((fCurve * CHANGE_AMOUNT) + MEDIAN_LENGTH);

		if (i == 0)
		{// 奥
			pLight->rot.z += pLight->fSpeed;
		}
		else if (i == 1)
		{// 手前
			pLight->rot.z += -pLight->fSpeed;
		}

		// 頂点座標の設定
		pVtx[0].pos.x = pos.x + (sinf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.y = pos.y + (cosf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + (sinf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.y = pos.y + (cosf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + (sinf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.y = pos.y + (cosf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + (sinf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.y = pos.y + (cosf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffLight->Unlock();
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// Wキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// Sキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAME:				// ゲーム
			ChangeMode(MODE_GAME);
			break;

		default:
			assert(false);
			break;
		}

		// 選択肢の決定
		DecisionOption();
	}
}
