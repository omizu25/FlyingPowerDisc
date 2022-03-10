//**************************************************
// 
// FPG���� ( texture.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "texture.h"

#include <assert.h>

//==================================================
// �萔
//==================================================
static const char *s_FileName[] =
{// �F�̔z��
	"data/TEXTURE/BG.png",
	"data/TEXTURE/Disc.png",
	"data/TEXTURE/Frame.png",
	"data/TEXTURE/Title_blue.png",
	"data/TEXTURE/TitleLight_red.png",
	"data/TEXTURE/Game_Start.png",
	"data/TEXTURE/Back_to_game.png",
	"data/TEXTURE/Retry.png",
	"data/TEXTURE/Back_to_title.png",
	"data/TEXTURE/UI000.png",
	"data/TEXTURE/UI001.png",
};

static_assert(sizeof(s_FileName) / sizeof(s_FileName[0]) == TEXTURE_MAX, "aho");

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadTexture(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTexture(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture)
{
	assert(texture >= 0 && texture < TEXTURE_MAX);

	return s_pTexture[texture];
}
