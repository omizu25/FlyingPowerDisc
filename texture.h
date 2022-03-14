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
	TEXTURE_BG = 0,
	TEXTURE_Disc,
	TEXTURE_Frame,
	TEXTURE_Title_blue,
	TEXTURE_TitleLight_red,
	TEXTURE_Pose_Title,
	TEXTURE_Pose_Resume,
	TEXTURE_Pose_Quit,
	TEXTURE_Back_to_title,
	TEXTURE_UI000,
	TEXTURE_UI001,
	TEXTURE_Select_Left,
	TEXTURE_Select_Right,
	TEXTURE_Number_0To9,
	TEXTURE_Select_Time,
	TEXTURE_Select_Point,
	TEXTURE_Select_Set,
	TEXTURE_Select_BG,
	TEXTURE_GAME_BG,
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
