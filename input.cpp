//=============================================================================
//
// 入力処理 [jobiusinput.cpp]
// Author1 : KOZUNA HIROHITO
// Author2  : katsuki mizuki
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "input.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define NUM_KEY_MAX		(256)	//キーの最大数（キーボード）
#define PLAYER_MAX		(2)		//プレイヤーの最大人数

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------

//ジョイパッド(DirectInput)
typedef struct
{
	LPDIRECTINPUT8 pJoyKeyInput = NULL;				//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevJoyKey = NULL;			//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	DIJOYSTATE2 JoyKeyStateDirect;					//ジョイパット（プレス処理）
	DIJOYSTATE2 JoyKeyStateDirectTrigger;			//ジョイパット（トリガー処理）
	DWORD OldJoyKeyDirect = 0xffffffff;				//前回の十字キーの値
	JOYKEY_CROSS OldJoyKeyStickDirect;				//前回のスティックの位置
	bool bJoyKey = false;							//使っていつかどうか
}JoyKeyDirect;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

//キーボード
static LPDIRECTINPUT8 s_pInput = NULL;					//DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;		//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
static BYTE s_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
static BYTE s_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
static BYTE s_aKeyStateRelease[NUM_KEY_MAX];			//キーボードのリリース情報

//ジョイパッド(DirectInput)
static JoyKeyDirect s_aJoyKeyDirectState;				//ジョイパッド(DirectInput)の構造体変数

//ジョイパッド
static XINPUT_STATE s_JoyKeyState[PLAYER_MAX];			//ジョイパットのプレス情報
static XINPUT_STATE s_JoyKeyStateTrigger[PLAYER_MAX];	//ジョイパットのトリガー情報
static D3DXVECTOR3 s_JoyStickPos[PLAYER_MAX];			//ジョイスティックの傾き
static JOYKEY_CROSS s_OldJoyKeyStick;					//前回のスティックの位置

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		//初期化
void UninitKeyboard(void);									//終了処理
void UpdateKeyboard(void);									//更新処理

//ジョイパッド(DirectInput)
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitJoypadDirect(void);								//終了処理
void UpdateJoypadDirect(void);								//更新処理

//ジョイパッド
HRESULT InitJoypad(void);									//初期化
void UninitJoypad(void);									//終了処理
void UpdateJoypad(void);									//更新処理

//*************************************************************************************
//入力処理全体
//*************************************************************************************

//入力処理全部の初期化
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッド(DirectInput)の初期化処理

	if (FAILED(InitJoypadDirect(hInstance, hWnd)))
	{
		s_aJoyKeyDirectState.bJoyKey = false;
	}
	else
	{
		s_aJoyKeyDirectState.bJoyKey = true;
	}

	//ジョイパッド初期化
	InitJoypad();

	return S_OK;
}

//入力処理全部の終了処理
void UninitInput(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//ジョイパッド(DirectInput)終了処理
	UninitJoypadDirect();

	//ジョイパッド終了処理
	UninitJoypad();
}

//入力処理全部の更新処理
void UpdateInput(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッド(DirectInput)更新処理
	UpdateJoypadDirect();

	//ジョイパッド更新処理
	UpdateJoypad();
}


//*****************************************************************************
//キーボード入力処理
//*****************************************************************************

//キーボード初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard, &s_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//キーボードの終了処理
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の放棄
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を放棄
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破壊
	if (s_pInput != NULL)
	{
		s_pInput->Release();
		s_pInput = NULL;
	}
}

//キーボードの更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = (s_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報を保存
			s_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();			//キーボードへのアクセス権を獲得
	}
}

//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//キーボードの全キープレス情報を取得
bool GetKeyboardAllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardPress(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//キーボードの全キートリガー情報を取得
bool GetKeyboardAllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardTrigger(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//*************************************************************************************
//ジョイパッド(DirectInput)入力処理
//*************************************************************************************

//ジョイパッド(DirectInput)初期化処理
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&s_aJoyKeyDirectState.pJoyKeyInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（ジョイパッド(DirectInput)）の生成
	if (FAILED(s_aJoyKeyDirectState.pJoyKeyInput->CreateDevice(GUID_Joystick, &s_aJoyKeyDirectState.pDevJoyKey, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(s_aJoyKeyDirectState.pDevJoyKey->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(s_aJoyKeyDirectState.pDevJoyKey->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ジョイパッド(DirectInput)へのアクセス権を獲得
	s_aJoyKeyDirectState.pDevJoyKey->Acquire();

	return S_OK;
}

//ジョイパッド(DirectInput)の終了処理
void UninitJoypadDirect(void)
{

	//入力デバイス（ジョイパッド(DirectInput)）の放棄
	if (s_aJoyKeyDirectState.pDevJoyKey != NULL)
	{
		s_aJoyKeyDirectState.pDevJoyKey->Unacquire();		//ジョイパッド(DirectInput)へのアクセス権を放棄
		s_aJoyKeyDirectState.pDevJoyKey->Release();
		s_aJoyKeyDirectState.pDevJoyKey = NULL;
	}

	//DirectInputオブジェクトの破壊
	if (s_aJoyKeyDirectState.pJoyKeyInput != NULL)
	{
		s_aJoyKeyDirectState.pJoyKeyInput->Release();
		s_aJoyKeyDirectState.pJoyKeyInput = NULL;
	}

}

//ジョイパッド(DirectInput)の更新処理
void UpdateJoypadDirect(void)
{
	DIJOYSTATE2 aKeyState;		//ジョイパッド(DirectInput)の入力情報


	if (s_aJoyKeyDirectState.bJoyKey)
	{
		//入力デバイスからデータを取得
		if (SUCCEEDED(s_aJoyKeyDirectState.pDevJoyKey->GetDeviceState(sizeof(aKeyState), &aKeyState)))
		{
			for (int nCnt = 0; nCnt < 32; nCnt++)
			{
				s_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[nCnt] = (s_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & aKeyState.rgbButtons[nCnt]; //キーボードのトリガー情報を保存
			}
			s_aJoyKeyDirectState.JoyKeyStateDirect = aKeyState;		//プレス処理
		}
		else
		{
			s_aJoyKeyDirectState.pDevJoyKey->Acquire();			//ジョイパッド(DirectInput)へのアクセス権を獲得
		}
	}
}

//ジョイパッド(DirectInput)全キープレス処理
bool GetDirectJoypadAllPress(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		if (GetDirectJoypadPress(JOYKEY_DIRECT(nCnt)))
		{
			return true;
		}
	}
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[nCnt] != 0xFFFFFFFF)
		{
			return true;
		}
	}
	return false;
}

//ジョイパッド(DirectInput)プレス処理
bool GetDirectJoypadPress(JOYKEY_DIRECT Key)
{
	return (s_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[Key] & 0x80) ? true : false;
}

//ジョイパッド(DirectInput)プレス処理(十字キーのみ)
bool GetDirectJoypadPress(JOYKEY_CROSS Key)
{
	return (signed)s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == Key;
}

//ジョイパッド(DirectInput)トリガー処理(十字キーのみ)
bool GetDirectJoypadTrigger(JOYKEY_CROSS Key)
{
	if (s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == s_aJoyKeyDirectState.OldJoyKeyDirect)
	{
		return false;
	}
	else if ((signed)s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == Key
		&& (signed)s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] != s_aJoyKeyDirectState.OldJoyKeyDirect)
	{
		s_aJoyKeyDirectState.OldJoyKeyDirect = s_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0];
		return true;
	}
	s_aJoyKeyDirectState.OldJoyKeyDirect = 0xffffffff;
	return false;
}

//ジョイパッド(DirectInput)トリガー処理
bool GetDirectJoypadTrigger(JOYKEY_DIRECT Key)
{
	return (s_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[Key] & 0x80) ? true : false;
}

//ジョイパッド(DirectInput)スティック処理
D3DXVECTOR3 GetDirectJoypadStick(JOYKEY_RIGHT_LEFT Key)
{
	switch (Key)
	{
	case JOYKEY_RIGHT_STICK:
		return D3DXVECTOR3(float(s_aJoyKeyDirectState.JoyKeyStateDirect.lZ) / 32767.0f - 1.0f, -float(s_aJoyKeyDirectState.JoyKeyStateDirect.lRz) / 32767.0f + 1.0f, 0.0f);
		break;
	case JOYKEY_LEFT_STICK:
		return D3DXVECTOR3(float(s_aJoyKeyDirectState.JoyKeyStateDirect.lX) / 32767.0f - 1.0f, -float(s_aJoyKeyDirectState.JoyKeyStateDirect.lY) / 32767.0f + 1.0f, 0.0f);
		break;
	}

	//メモ、Xは右が１、左が-１、Yは上が１、下が-１

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//ジョイパッドスティック8方向プレス処理
bool GetDirectJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	D3DXVECTOR3 pos = GetDirectJoypadStick(RightLeft);

	if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

//ジョイパッドスティック8方向トリガー処理
bool GetDirectJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	if (GetDirectJoypadStickPress(RightLeft, Key)
		&& Key != s_aJoyKeyDirectState.OldJoyKeyStickDirect)
	{
		s_aJoyKeyDirectState.OldJoyKeyStickDirect = Key;
		return true;
	}
	else if (GetDirectJoypadStickPress(RightLeft, Key)
		&& Key == s_aJoyKeyDirectState.OldJoyKeyStickDirect)
	{
		return false;
	}

	s_aJoyKeyDirectState.OldJoyKeyStickDirect = JOYKEY_CROSS_MAX;
	return false;
}

//*************************************************************************************
//ジョイパッド入力処理
//*************************************************************************************

//ジョイパッドの初期化
HRESULT InitJoypad(void)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーのクリア
		memset(&s_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&s_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//ジョイパッドの終了
void UninitJoypad(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//ジョイパッドの更新
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			s_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~s_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //トリガー情報を保存
			s_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理
		}
	}
}

//ジョイパッドのプレス処理
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパッドのトリガー処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパット（スティックプレス）処理
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer)
{
	switch (RightLeft)
	{
	case JOYKEY_LEFT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -s_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -s_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return s_JoyStickPos[nPlayer];
}

//ジョイパット（トリガーペダル）処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//ジョイパッドスティックプレス８方向
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer)
{
	D3DXVECTOR3 pos = GetJoypadStick(RightLeft, nPlayer);

	if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

//ジョイパッドスティックトリガー８方向
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer)
{
	if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key != s_OldJoyKeyStick)
	{
		s_OldJoyKeyStick = Key;
		return true;
	}
	else if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key == s_OldJoyKeyStick)
	{
		return false;
	}

	s_OldJoyKeyStick = JOYKEY_CROSS_MAX;
	return false;
}