//**************************************************
//
// FPD制作 ( game.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _GAME_H_	//このマクロ定義がされてなかったら
#define _GAME_H_	//２重インクルード防止のマクロ定義

//==================================================
// 列挙型
//==================================================
typedef enum
{
	GAMESTATE_NONE = 0,	// 何もしていない状態
	GAMESTATE_START,	// 開始状態
	GAMESTATE_NORMAL,	// 通常状態
	GAMESTATE_RESET,	// リセット状態
	GAMESTATE_END,		// 終了状態
	GAMESTATE_RESULT,	// リザルト状態
	GAMESTATE_MAX
}GAMESTATE;

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void);

//--------------------------------------------------
// 設定
// 引数  : GAMESTATE state / 列挙型 状態
//--------------------------------------------------
void SetGameState(GAMESTATE state);

//--------------------------------------------------
// 取得
// 返値  : GAMESTATE / 列挙型 状態
//--------------------------------------------------
GAMESTATE GetGameState(void);

//--------------------------------------------------
// ポーズの有効無効設定
// 引数  : bool bPause / ポーズするかどうか [ true : する false : しない ]
//--------------------------------------------------
void SetEnablePause(bool bPause);

#endif // !_GAME_H_
