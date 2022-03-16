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
namespace
{
const char *s_FileName[] =
{// �e�N�X�`���̃p�X
	//�^�C�g��
	"data/TEXTURE/TITLE/BG.png",			//�^�C�g����ʂ̔w�i
	"data/TEXTURE/TITLE/Frame.png",			//�^�C�g����ʂ̘g
	"data/TEXTURE/TITLE/Title_blue.png",	//�^�C�g�����S
	"data/TEXTURE/TITLE/TitleLight_red.png",//�^�C�g�����S�̌��
	"data/TEXTURE/TITLE/Game_Start.png",	//�Q�[���X�^�[�g�̕���
	"data/TEXTURE/TITLE/ThreePoints.png",	//3�|�C���g
	"data/TEXTURE/TITLE/FivrPoints.png",	//5�|�C���g

	//�Z���N�g
	"data/TEXTURE/SELECT/Select_Left.png",	//���{�^��
	"data/TEXTURE/SELECT/Select_Right.png",	//�E�{�^��
	"data/TEXTURE/SELECT/Select_Time.png",	//��������
	"data/TEXTURE/SELECT/Select_Point.png",	//�|�C���g��
	"data/TEXTURE/SELECT/Select_Set.png",	//�Z�b�g��
	"data/TEXTURE/SELECT/Select_BG.png",	//�Z���N�g��ʂ̔w�i
	"data/TEXTURE/SELECT/Map_Title.png",	//�}�b�v�I��
	"data/TEXTURE/SELECT/Rule_Title.png",	//���[���I��
	
	//�Q�[��
	"data/TEXTURE/GAME/Disc.png",			//�f�B�X�N
	"data/TEXTURE/GAME/Game_BG.png",		//�Q�[����ʂ̔w�i
	"data/TEXTURE/GAME/Number_Yellow.png",	//����(���F)
	"data/TEXTURE/GAME/Stage01.png",		//1�ڂ̃X�e�[�W

	//�Q�[������UI
	"data/TEXTURE/GAME/Set1_1p.png",		//�Z�b�g1(1p�J���[)
	"data/TEXTURE/GAME/Set2_1p.png",		//�Z�b�g2(1p�J���[)
	"data/TEXTURE/GAME/Set3_1p.png",		//�Z�b�g3(1p�J���[)
	"data/TEXTURE/GAME/Set1_2p.png",		//�Z�b�g1(2p�J���[)
	"data/TEXTURE/GAME/Set2_2p.png",		//�Z�b�g2(2p�J���[)
	"data/TEXTURE/GAME/Set3_2p.png",		//�Z�b�g3(2p�J���[)
	"data/TEXTURE/GAME/SetFrame01.png",		//�Z�b�g�̘g(1p�J���[)
	"data/TEXTURE/GAME/SetFrame02.png",		//�Z�b�g�̘g(2p�J���[)
	"data/TEXTURE/GAME/PtsFrame01.png",		//�|�C���g�̘g(1p�J���[)
	"data/TEXTURE/GAME/PtsFrame02.png",		//�|�C���g�̘g(2p�J���[)
	"data/TEXTURE/UI/UI000.png",			//UI-0
	"data/TEXTURE/UI/UI001.png",			//UI-1

	//�|�[�Y
	"data/TEXTURE/PAUSE/Pose_Title.png",	//�|�[�Y��ʂ̃^�C�g��
	"data/TEXTURE/PAUSE/Pose_Resume.png",	//�ĊJ
	"data/TEXTURE/PAUSE/Pose_Quit.png",		//�I��

	//���̑�
	"data/TEXTURE/Number_0To9.png",			//����
};
}// namespace�͂����܂�

static_assert(sizeof(s_FileName) / sizeof(s_FileName[0]) == TEXTURE_MAX, "aho");

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];
}// namespace�͂����܂�

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
	if (texture == TEXTURE_NONE)
	{// �e�N�X�`�����g�p���Ȃ�
		return NULL;
	}

	assert(texture >= 0 && texture < TEXTURE_MAX);

	return s_pTexture[texture];
}
