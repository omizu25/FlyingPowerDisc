//**************************************************
// 
// FPG制作 ( rectangle.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _RECTANGLE_H_		//このマクロ定義がされてなかったら
#define _RECTANGLE_H_		//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "main.h"

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void);

//--------------------------------------------------
// 設定
// 引数  : LPDIRECT3DTEXTURE9 pTexture / テクスチャ
//--------------------------------------------------
int SetRectangle(LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// 使うのを止める
// 引数  : int nIdx / インデックス
//--------------------------------------------------
void StopUseRectangle(int nIdx);

//--------------------------------------------------
// 位置の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 pos / 中心の位置
// 引数3  : D3DXVECTOR3 size / サイズ
//--------------------------------------------------
void SetPosRectangle(int nIdx, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size);

//--------------------------------------------------
// 回転する位置の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 pos / 中心の位置
// 引数3  : D3DXVECTOR3 rot / 向き
// 引数4  : float fAngle / 角度
// 引数5  : float fLength / 長さ
//--------------------------------------------------
void SetRotationPosRectangle(int nIdx, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float fAngle, float fLength);

//--------------------------------------------------
// 色の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXCOLOR color / 色
//--------------------------------------------------
void SetColorRectangle(int nIdx, const D3DXCOLOR &color);

//--------------------------------------------------
// テクスチャ座標の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR2 U / x = 左端, y = 右端
// 引数3  : D3DXVECTOR2 V / x = 上端, y = 下端
//--------------------------------------------------
void SetTexRectangle(int nIdx, const D3DXVECTOR2 &U, const D3DXVECTOR2 &V);

//--------------------------------------------------
// 頂点バッファの取得
// 引数  : int nIdx / インデックス
// 返値  : LPDIRECT3DVERTEXBUFFER9 / 頂点バッファ
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx);

#endif // !_RECTANGLE_H_
