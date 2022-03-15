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
	//�^�C�g��
	TEXTURE_BG = 0,				//�^�C�g����ʂ̔w�i
	TEXTURE_Frame,				//�^�C�g����ʂ̘g
	TEXTURE_Title_blue,			//�^�C�g�����S
	TEXTURE_TitleLight_red,		//�^�C�g�����S�̌��
	TEXTURE_Game_Start,			//�Q�[���X�^�[�g�̕���

	//�Z���N�g
	TEXTURE_Select_Left,		//���{�^��
	TEXTURE_Select_Right,		//�E�{�^��
	TEXTURE_Select_Time,		//��������
	TEXTURE_Select_Point,		//�|�C���g��
	TEXTURE_Select_Set,			//�Z�b�g��
	TEXTURE_Select_BG,			//�Z���N�g��ʂ̔w�i
	TEXTURE_Map_Title,			//�}�b�v�I��
	TEXTURE_Rule_Title,			//���[���I��

	//�Q�[��
	TEXTURE_Disc,				//�f�B�X�N
	TEXTURE_Game_BG,			//�Q�[����ʂ̔w�i
	TEXTURE_UI000,				//UI-0
	TEXTURE_UI001,				//UI-1

	//�|�[�Y
	TEXTURE_Pose_Title,			//�|�[�Y��ʂ̃^�C�g��
	TEXTURE_Pose_Resume,		//�ĊJ
	TEXTURE_Pose_Quit,			//�I��

	//���̑�
	TEXTURE_Number_0To9,		//����
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
