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
	/*�� �^�C�g�� ��*/
	"data/TEXTURE/TITLE/BG.png",				// �^�C�g����ʂ̔w�i(����)
	"data/TEXTURE/TITLE/BG2.png",				// �^�C�g����ʂ̔w�i(�ጴ)
	"data/TEXTURE/TITLE/BG3.png",				// �^�C�g����ʂ̔w�i(�C)
	"data/TEXTURE/TITLE/Title_Rogo.png",		// �^�C�g�����S
	"data/TEXTURE/TITLE/Game_Start.png",		// �Q�[���X�^�[�g�̕���
	"data/TEXTURE/TITLE/Rule_Select.png",		// ���[���I���̕���
	"data/TEXTURE/TITLE/Map_Select.png",		// �}�b�v�I���̕���
	"data/TEXTURE/TITLE/Char_Select.png",		// �L�����I���̕���
	"data/TEXTURE/TITLE/Tutorial_Start.png",	// �`���[�g���A���̕���

	/*�� �Z���N�g ��*/
	"data/TEXTURE/SELECT/Select_Left.png",		// ���{�^��
	"data/TEXTURE/SELECT/Select_Right.png",		// �E�{�^��
	"data/TEXTURE/SELECT/Select_Time.png",		// ��������
	"data/TEXTURE/SELECT/Select_Point.png",		// �|�C���g��
	"data/TEXTURE/SELECT/Select_Set.png",		// �Z�b�g��
	"data/TEXTURE/SELECT/Select_BG.png",		// �Z���N�g��ʂ̔w�i
	"data/TEXTURE/SELECT/Map_Title.png",		// �}�b�v�I��
	"data/TEXTURE/SELECT/Rule_Title.png",		// ���[���I��
	"data/TEXTURE/SELECT/SPEED.png",			// ���x
	"data/TEXTURE/SELECT/POWER.png",			// �p���[
	
	/*�� �Q�[�� ��*/
	"data/TEXTURE/GAME/Disc.png",				// �f�B�X�N
	"data/TEXTURE/GAME/Number_Yellow.png",		// ����(���F)
	"data/TEXTURE/GAME/Stage01.png",			// 1�ڂ̃X�e�[�W
	"data/TEXTURE/GAME/Stage02.png",			// 2�ڂ̃X�e�[�W
	"data/TEXTURE/GAME/Stage03.png",			// 3�ڂ̃X�e�[�W
	"data/TEXTURE/GAME/Stage01_BG.png",			// 1�ڂ̃X�e�[�W�̔w�i
	"data/TEXTURE/GAME/Stage02_BG.png",			// 2�ڂ̃X�e�[�W�̔w�i
	"data/TEXTURE/GAME/Stage03_BG.png",			// 3�ڂ̃X�e�[�W�̔w�i

	/*�� �Q�[������UI ��*/
	"data/TEXTURE/UI/SetFrame01.png",			// �Z�b�g�̘g(1p�J���[)
	"data/TEXTURE/UI/SetFrame02.png",			// �Z�b�g�̘g(2p�J���[)
	"data/TEXTURE/UI/PtsFrame01.png",			// �|�C���g�̘g(1p�J���[)
	"data/TEXTURE/UI/PtsFrame02.png",			// �|�C���g�̘g(2p�J���[)
	"data/TEXTURE/UI/UI000.png",				// �v���C���[1
	"data/TEXTURE/UI/UI001.png",				// �v���C���[2
	"data/TEXTURE/UI/Set1.png",					// �Z�b�g1�̕\��
	"data/TEXTURE/UI/Set2.png",					// �Z�b�g2�̕\��
	"data/TEXTURE/UI/Set3.png",					// �Z�b�g3�̕\��
	"data/TEXTURE/UI/Set4.png",					// �Z�b�g4�̕\��
	"data/TEXTURE/UI/Set5.png",					// �Z�b�g5�̕\��
	"data/TEXTURE/UI/ThreePoints.png",			// 3�|�C���g
	"data/TEXTURE/UI/FivrPoints.png",			// 5�|�C���g

	/*�� �|�[�Y ��*/
	"data/TEXTURE/PAUSE/Pose_Title.png",		// �|�[�Y��ʂ̃^�C�g��
	"data/TEXTURE/PAUSE/Pose_Resume.png",		// �ĊJ
	"data/TEXTURE/PAUSE/Pose_Quit.png",			// �I��

	/*�� ���U���g ��*/
	"data/TEXTURE/RESULT/Win.png",				// ����
	"data/TEXTURE/RESULT/Lose.png",				// �s�k
	"data/TEXTURE/RESULT/Draw.png",				// ��������

	/*�� �v���C���[ ��*/
	"data/TEXTURE/PLAYER/enemy000.png",			// �J�s�o��
	"data/TEXTURE/PLAYER/kitune.png",			// �S��
	"data/TEXTURE/PLAYER/player000.png",		// �X���C��
	"data/TEXTURE/PLAYER/ghost.png",			// �S�[�X�g
	"data/TEXTURE/PLAYER/cat.png",				// �L

	/*�� �G�t�F�N�g ��*/
	"data/TEXTURE/EFFECT/fire.png",			// ���̃G�t�F�N�g
	"data/TEXTURE/EFFECT/spin.png",			// �f�B�X�N�̃G�t�F�N�g
	"data/TEXTURE/EFFECT/tackle.png",		// �^�b�N���̃G�t�F�N�g
	"data/TEXTURE/EFFECT/Goal.png",			// �S�[���̃G�t�F�N�g
	"data/TEXTURE/EFFECT/spark.png",		// �ǂ̉Ή�

	/*�� ���̑� ��*/
	"data/TEXTURE/Number_0To9.png",			// ����
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
