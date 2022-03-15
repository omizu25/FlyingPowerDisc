//========================================================
//タイム処理
//Author:Seino Riku
//========================================================
#ifndef _TIMEH
#define _TIMEH

#include "main.h"

#define MAX_TIME (3)
#define MAX_SIZE  (50.0f)        //大きさの最大数


//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nScore);
void AddTime(int nValue);
int GetTime(void);

#endif
