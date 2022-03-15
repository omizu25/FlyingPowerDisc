//========================================================
//�^�C������
//Author:Seino Riku
//========================================================
#ifndef _TIMEH
#define _TIMEH

#include "main.h"

#define MAX_TIME (3)
#define MAX_SIZE  (50.0f)        //�傫���̍ő吔


//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nScore);
void AddTime(int nValue);
int GetTime(void);

#endif
