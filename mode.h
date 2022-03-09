//==================================================
// 
// FPG���� ( mode.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _MODE_H_		// ���̃}�N������`����ĂȂ�������
#define _MODE_H_		// 2�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	MODE_TITLE = 0,		// �^�C�g��
	MODE_GAME,			// �Q�[��
	MODE_MAX,
	MODE_NONE
}MODE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void UninitMode(void);
void UpdateMode(void);
void DrawMode(void);
void SetMode(void);
MODE GetMode(void);
void ChangeMode(MODE mode);

#endif // !_MODE_H_
