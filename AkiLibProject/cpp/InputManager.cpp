//==============================================================================
/// @file	InputManager.cpp
/// @brief	���͊֌W���C�u��������
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CInputManager::CInputManager():
	m_lpInput		( nullptr ),
	m_lpKeyboard	( nullptr ),
	m_lpMouse		( nullptr )
{
}

// ������
bool CInputManager::Init(HINSTANCE hinst, HWND hwnd)
{
	m_hWnd = hwnd;

	try
	{
		// �C���^�[�t�F�C�X�쐬
		CreateInterFace( hinst );

		// �L�[�{�[�h������
		InitKeyboard( hwnd );

		// �}�E�X������
		InitMouse( hwnd );
	}
	catch( LPCSTR error )
	{
		ErrorMessageBox( error );
		return false;
	}
	catch( ... )
	{
		ErrorMessageBox( "�L�[�{�[�h�������ł��܂���ł���\n�������s���ł�" );
		return false;
	}

	return true;
}

// �C���^�[�t�F�C�X�쐬
void CInputManager::CreateInterFace( HINSTANCE hInst )
{
	// ���łɍ쐬����Ă���Ή������Ȃ�
	if( m_lpInput )
	{
		return;
	}

	if( FAILED( DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>( &m_lpInput ), nullptr ) ) )
	{
		throw "�C���v�b�g�C���^�[�t�F�C�X�쐬���s";
	}
}

// �������
void CInputManager::Release()
{
	SafeRelease( m_lpMouse );
	SafeRelease( m_lpKeyboard );
	SafeRelease( m_lpInput );
}

// �L�[�{�[�h������
bool CInputManager::InitKeyboard( HWND hwnd )
{
	// �L�[�{�[�h�f�o�C�X�쐬
	if( FAILED( m_lpInput->CreateDevice( GUID_SysKeyboard, &m_lpKeyboard, NULL ) ) )
	{
		throw "�L�[�{�[�h�f�o�C�X�쐬���s";
	}

	// �f�[�^�`���ݒ�
	if( FAILED( m_lpKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		throw "�L�[�{�[�h�f�[�^�`���ݒ莸�s";
	}

	// �������x���ݒ�
	m_lpKeyboard->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	// �ڍאݒ�
	DIPROPDWORD diprop = { 0 };
	diprop.diph.dwSize		= sizeof( DIPROPDWORD );
	diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
	diprop.diph.dwObj		= 0;
	diprop.diph.dwHow		= DIPH_DEVICE;
	diprop.dwData			= 8;	// �o�b�t�@�T�C�Y

	if( FAILED( m_lpKeyboard->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) )
	{
		throw "�L�[�{�[�h���[�h�ڍאݒ莸�s";
	}

	// ���͊J�n
	m_lpKeyboard->Acquire();

	// ��ԏ�����
	for( int i = 0; i < _countof(m_KeyboardState); i++ )
	{
		m_KeyboardState[i] = FREE;
	}

	return true;
}

// �}�E�X������
bool CInputManager::InitMouse( HWND hwnd )
{
	// �}�E�X�f�o�C�X�쐬
	if( FAILED( m_lpInput->CreateDevice( GUID_SysMouse, &m_lpMouse, nullptr ) ) )
	{
		throw "�}�E�X�f�o�C�X�쐬���s";
	}

	// �f�[�^�`���ݒ�
	if( FAILED( m_lpMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		throw "�}�E�X�`���ݒ莸�s";
	}

	// �������x���ݒ�
	m_lpMouse->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	// �ڍאݒ�
	DIPROPDWORD diprop = { 0 };
	diprop.diph.dwSize		= sizeof( DIPROPDWORD );
	diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
	diprop.diph.dwObj		= 0;
	diprop.diph.dwHow		= DIPH_DEVICE;
	diprop.dwData			= DIPROPAXISMODE_ABS;	// �o�b�t�@�T�C�Y

	if( FAILED( m_lpMouse->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) )
	{
		throw "�}�E�X���[�h�ڍאݒ莸�s";
	}

	// ���͊J�n
	m_lpMouse->Acquire();

	// ��ԏ�����
	for( int i = 0; i < _countof(m_MouseButtonState); i++ )
	{
		m_MouseButtonState[i] = FREE;
	}

	return true;
}

// �L�[�{�[�h���͏�ԍX�V
void CInputManager::UpdateState()
{
	// �L�[�{�[�h�X�V
	if( m_lpKeyboard )
	{
		UpdateKeyboardState();
	}

	// �}�E�X��ԍX�V
	if( m_lpMouse )
	{
		UpdateMouseState();
	}
}

// �L�[�{�[�h��ԍX�V
void CInputManager::UpdateKeyboardState()
{
	BYTE tempKeyState[256];	// �L�[�{�[�h�̍��̓��͏��

	// �L�[�{�[�h��ԍX�V
	if( SUCCEEDED( m_lpKeyboard->GetDeviceState( _countof(tempKeyState), tempKeyState ) ) )
	{
		for( int i = 0; i < 256; i++ )
		{
			// �{�^����Ԕ���
			if( tempKeyState[i] & 0x80 )// ������Ă��鎞
			{
				if( m_KeyboardState[i] == FREE || m_KeyboardState[i] == PULL )// �O�̃t���[���ŉ�����Ă��Ȃ����
				{
					m_KeyboardState[i] = PUSH;	// ������
				}
				else
				{
					m_KeyboardState[i] = HOLD;	// ������Ă���
				}
			}
			// ������Ă��Ȃ��Ƃ�
			else
			{
				if( m_KeyboardState[i] == PUSH || m_KeyboardState[ i ] == HOLD )
				{
					m_KeyboardState[i] = PULL;	// �����ꂽ
				}
				else
				{
					m_KeyboardState[i] = FREE;	// ������Ă���
				}
			}
		}
	}
}

// �}�E�X��ԍX�V
void CInputManager::UpdateMouseState()
{
	// �}�E�X��ԍX�V
	if( SUCCEEDED( m_lpMouse->GetDeviceState( sizeof( m_MouseState ), &m_MouseState ) ) )
	{
		for( int i = 0; i < _countof( m_MouseButtonState ); i++ )
		{
			// �{�^����Ԕ���
			if( m_MouseState.rgbButtons[i] & 0x80 )// ������Ă��鎞
			{
				if( m_MouseButtonState[i] == FREE || m_MouseButtonState[i] == PULL )// �O�̃t���[���ŉ�����Ă��Ȃ����
				{
					m_MouseButtonState[i] = PUSH;	// ������
				}
				else
				{
					m_MouseButtonState[i] = HOLD;	// ������Ă���
				}
			}
			// ������Ă��Ȃ��Ƃ�
			else
			{
				if( m_MouseButtonState[i] == PUSH || m_MouseButtonState[ i ] == HOLD )
				{
					m_MouseButtonState[i] = PULL;	// �����ꂽ
				}
				else
				{
					m_MouseButtonState[i] = FREE;	// ������Ă���
				}
			}
		}
	}

	// �}�E�X�ʒu�X�V
	RECT rect;

	GetCursorPos( &m_MousePos );
	GetWindowRect( m_hWnd, &rect );
	m_MousePos.x -= rect.left;
	m_MousePos.y -= rect.top;
}

// �L�[�{�[�h���͐���
void CInputManager::SetKeyboardAcquire( bool flg )
{
	// �^�Ȃ���͊J�n
	if( flg )
	{
		m_lpKeyboard->Acquire();
	}
	// �U�Ȃ���͐��䂵�Ȃ�
	else
	{	
		m_lpKeyboard->Unacquire();
	}
}

// �}�E�X���͐���
void CInputManager::SetMouseAcquire( bool flg )
{
	// �^�Ȃ���͊J�n
	if( flg )
	{
		m_lpMouse->Acquire();
	}
	// �U�Ȃ���͐��䂵�Ȃ�
	else
	{	
		m_lpMouse->Unacquire();
	}
}

// �L�[�{�[�h���͏�Ԏ擾
bool CInputManager::IsKeyboardState( BYTE Key, BYTE state )
{
	if( (m_KeyboardState[Key] & state) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// �}�E�X���͏�Ԏ擾
bool CInputManager::IsMouseButtonState( EMouseButton button, BYTE state )
{
	if( (m_MouseButtonState[button] & state) )
	{
		return true;
	}
	else
	{
		return false;
	}
}