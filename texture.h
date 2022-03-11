//**************************************************
// 
// FPG���� ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_		//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

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
	TEXTURE_Game_Start,
	TEXTURE_Back_to_game,
	TEXTURE_Retry,
	TEXTURE_Back_to_title,
	TEXTURE_UI000,
	TEXTURE_UI001,
	TEXTURE_Select_Left,
	TEXTURE_Select_Right,
	TEXTURE_MAX,
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
