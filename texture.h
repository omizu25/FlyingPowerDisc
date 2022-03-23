//**************************************************
// 
// FPG���� ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �񋓌^
//==================================================
typedef enum
{
	/*�� �^�C�g�� ��*/
	TEXTURE_BG = 0,				// �^�C�g����ʂ̔w�i
	TEXTURE_Frame,				// �^�C�g����ʂ̘g
	TEXTURE_Title_blue,			// �^�C�g�����S
	TEXTURE_TitleLight_red,		// �^�C�g�����S�̌��
	TEXTURE_Game_Start,			// �Q�[���X�^�[�g�̕���
	TEXTURE_Rule_Select,		// ���[���I���̕���
	TEXTURE_Map_Select,			// �}�b�v�I���̕���
	TEXTURE_Char_Select,		// �L�����I���̕���
	TEXTURE_Tutorial_Start,		// �`���[�g���A���̕���
	TEXTURE_Three_Point,		// 3�|�C���g
	TEXTURE_Five_Point,			// 5�|�C���g

	/*�� �Z���N�g ��*/
	TEXTURE_Select_Left,		// ���{�^��
	TEXTURE_Select_Right,		// �E�{�^��
	TEXTURE_Select_Time,		// ��������
	TEXTURE_Select_Point,		// �|�C���g��
	TEXTURE_Select_Set,			// �Z�b�g��
	TEXTURE_Select_BG,			// �Z���N�g��ʂ̔w�i
	TEXTURE_Map_Title,			// �}�b�v�I��
	TEXTURE_Rule_Title,			// ���[���I��

	/*�� �Q�[�� ��*/
	TEXTURE_Disc,				// �f�B�X�N
	TEXTURE_Game_BG,			// �Q�[����ʂ̔w�i
	TEXTURE_Number_Yellow,		// ����(���F)
	TEXTURE_Stage01,			// 1�ڂ̃X�e�[�W

	/*�� �Q�[������UI ��*/
	TEXTURE_Set1_1p,			// �Z�b�g1(1p�J���[)
	TEXTURE_Set2_1p,			// �Z�b�g2(1p�J���[)
	TEXTURE_Set3_1p,			// �Z�b�g3(1p�J���[)
	TEXTURE_Set1_2p,			// �Z�b�g1(2p�J���[)
	TEXTURE_Set2_2p,			// �Z�b�g2(2p�J���[)
	TEXTURE_Set3_2p,			// �Z�b�g3(2p�J���[)
	TEXTURE_GAME_SetFrame01,	// �Z�b�g�̘g(1p�J���[)
	TEXTURE_GAME_SetFrame02,	// �Z�b�g�̘g(2p�J���[)
	TEXTURE_GAME_PtsFrame01,	// �|�C���g�̘g(1p�J���[)
	TEXTURE_GAME_PtsFrame02,	// �|�C���g�̘g(2p�J���[)
	TEXTURE_UI000,				// UI-0
	TEXTURE_UI001,				// UI-1
	TEXTURE_Set1,				//�Z�b�g1�̕\��
	TEXTURE_Set2,				//�Z�b�g2�̕\��
	TEXTURE_Set3,				//�Z�b�g3�̕\��
	TEXTURE_Set4,				//�Z�b�g2�̕\��
	TEXTURE_Set5,				//�Z�b�g3�̕\��

	/*�� �|�[�Y ��*/
	TEXTURE_Pose_Title,			// �|�[�Y��ʂ̃^�C�g��
	TEXTURE_Pose_Resume,		// �ĊJ
	TEXTURE_Pose_Quit,			// �I��

	/*�� ���U���g ��*/
	TEXTURE_Result_Win,			// ����
	TEXTURE_Result_Lose,		// �s�k
	TEXTURE_Result_Draw,		// ��������

	/*�� �G�t�F�N�g ��*/
	TEXTURE_Effect_fire,		// ���̃G�t�F�N�g
	TEXTURE_Effect_spin,		// �f�B�X�N�̃G�t�F�N�g
	TEXTURE_Effect_tackle,		// �^�b�N���̃G�t�F�N�g

	/*�� ���̑� ��*/
	TEXTURE_Number_0To9,		// ����
	TEXTURE_MAX,
	TEXTURE_NONE,
}TEXTURE;

//==================================================
// �v���g�^�C�v�錾
//==================================================
//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadTexture(void);

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTexture(void);

//--------------------------------------------------
// �擾
// ����  : TEXTURE texture / �񋓌^ ���
// �Ԓl  : LPDIRECT3DTEXTURE9 / �e�N�X�`��
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture);

#endif // !_TEXTURE_H_
