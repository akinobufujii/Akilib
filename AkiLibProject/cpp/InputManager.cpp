//==============================================================================
/// @file	InputManager.cpp
/// @brief	入力関係ライブラリ実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CInputManager::CInputManager():
	m_lpInput		( nullptr ),
	m_lpKeyboard	( nullptr ),
	m_lpMouse		( nullptr )
{
}

// 初期化
bool CInputManager::Init(HINSTANCE hinst, HWND hwnd)
{
	m_hWnd = hwnd;

	try
	{
		// インターフェイス作成
		CreateInterFace( hinst );

		// キーボード初期化
		InitKeyboard( hwnd );

		// マウス初期化
		InitMouse( hwnd );
	}
	catch( LPCSTR error )
	{
		ErrorMessageBox( error );
		return false;
	}
	catch( ... )
	{
		ErrorMessageBox( "キーボード初期化できませんでした\n原因が不明です" );
		return false;
	}

	return true;
}

// インターフェイス作成
void CInputManager::CreateInterFace( HINSTANCE hInst )
{
	// すでに作成されてあれば何もしない
	if( m_lpInput )
	{
		return;
	}

	if( FAILED( DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>( &m_lpInput ), nullptr ) ) )
	{
		throw "インプットインターフェイス作成失敗";
	}
}

// 解放処理
void CInputManager::Release()
{
	SafeRelease( m_lpMouse );
	SafeRelease( m_lpKeyboard );
	SafeRelease( m_lpInput );
}

// キーボード初期化
bool CInputManager::InitKeyboard( HWND hwnd )
{
	// キーボードデバイス作成
	if( FAILED( m_lpInput->CreateDevice( GUID_SysKeyboard, &m_lpKeyboard, NULL ) ) )
	{
		throw "キーボードデバイス作成失敗";
	}

	// データ形式設定
	if( FAILED( m_lpKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		throw "キーボードデータ形式設定失敗";
	}

	// 強調レベル設定
	m_lpKeyboard->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	// 詳細設定
	DIPROPDWORD diprop = { 0 };
	diprop.diph.dwSize		= sizeof( DIPROPDWORD );
	diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
	diprop.diph.dwObj		= 0;
	diprop.diph.dwHow		= DIPH_DEVICE;
	diprop.dwData			= 8;	// バッファサイズ

	if( FAILED( m_lpKeyboard->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) )
	{
		throw "キーボードモード詳細設定失敗";
	}

	// 入力開始
	m_lpKeyboard->Acquire();

	// 状態初期化
	for( int i = 0; i < _countof(m_KeyboardState); i++ )
	{
		m_KeyboardState[i] = FREE;
	}

	return true;
}

// マウス初期化
bool CInputManager::InitMouse( HWND hwnd )
{
	// マウスデバイス作成
	if( FAILED( m_lpInput->CreateDevice( GUID_SysMouse, &m_lpMouse, nullptr ) ) )
	{
		throw "マウスデバイス作成失敗";
	}

	// データ形式設定
	if( FAILED( m_lpMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		throw "マウス形式設定失敗";
	}

	// 強調レベル設定
	m_lpMouse->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	// 詳細設定
	DIPROPDWORD diprop = { 0 };
	diprop.diph.dwSize		= sizeof( DIPROPDWORD );
	diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
	diprop.diph.dwObj		= 0;
	diprop.diph.dwHow		= DIPH_DEVICE;
	diprop.dwData			= DIPROPAXISMODE_ABS;	// バッファサイズ

	if( FAILED( m_lpMouse->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) )
	{
		throw "マウスモード詳細設定失敗";
	}

	// 入力開始
	m_lpMouse->Acquire();

	// 状態初期化
	for( int i = 0; i < _countof(m_MouseButtonState); i++ )
	{
		m_MouseButtonState[i] = FREE;
	}

	return true;
}

// キーボード入力状態更新
void CInputManager::UpdateState()
{
	// キーボード更新
	if( m_lpKeyboard )
	{
		UpdateKeyboardState();
	}

	// マウス状態更新
	if( m_lpMouse )
	{
		UpdateMouseState();
	}
}

// キーボード状態更新
void CInputManager::UpdateKeyboardState()
{
	BYTE tempKeyState[256];	// キーボードの今の入力状態

	// キーボード状態更新
	if( SUCCEEDED( m_lpKeyboard->GetDeviceState( _countof(tempKeyState), tempKeyState ) ) )
	{
		for( int i = 0; i < 256; i++ )
		{
			// ボタン状態判定
			if( tempKeyState[i] & 0x80 )// 押されている時
			{
				if( m_KeyboardState[i] == FREE || m_KeyboardState[i] == PULL )// 前のフレームで押されていなければ
				{
					m_KeyboardState[i] = PUSH;	// 押した
				}
				else
				{
					m_KeyboardState[i] = HOLD;	// 押されている
				}
			}
			// 押されていないとき
			else
			{
				if( m_KeyboardState[i] == PUSH || m_KeyboardState[ i ] == HOLD )
				{
					m_KeyboardState[i] = PULL;	// 離された
				}
				else
				{
					m_KeyboardState[i] = FREE;	// 離されている
				}
			}
		}
	}
}

// マウス状態更新
void CInputManager::UpdateMouseState()
{
	// マウス状態更新
	if( SUCCEEDED( m_lpMouse->GetDeviceState( sizeof( m_MouseState ), &m_MouseState ) ) )
	{
		for( int i = 0; i < _countof( m_MouseButtonState ); i++ )
		{
			// ボタン状態判定
			if( m_MouseState.rgbButtons[i] & 0x80 )// 押されている時
			{
				if( m_MouseButtonState[i] == FREE || m_MouseButtonState[i] == PULL )// 前のフレームで押されていなければ
				{
					m_MouseButtonState[i] = PUSH;	// 押した
				}
				else
				{
					m_MouseButtonState[i] = HOLD;	// 押されている
				}
			}
			// 押されていないとき
			else
			{
				if( m_MouseButtonState[i] == PUSH || m_MouseButtonState[ i ] == HOLD )
				{
					m_MouseButtonState[i] = PULL;	// 離された
				}
				else
				{
					m_MouseButtonState[i] = FREE;	// 離されている
				}
			}
		}
	}

	// マウス位置更新
	RECT rect;

	GetCursorPos( &m_MousePos );
	GetWindowRect( m_hWnd, &rect );
	m_MousePos.x -= rect.left;
	m_MousePos.y -= rect.top;
}

// キーボード入力制御
void CInputManager::SetKeyboardAcquire( bool flg )
{
	// 真なら入力開始
	if( flg )
	{
		m_lpKeyboard->Acquire();
	}
	// 偽なら入力制御しない
	else
	{	
		m_lpKeyboard->Unacquire();
	}
}

// マウス入力制御
void CInputManager::SetMouseAcquire( bool flg )
{
	// 真なら入力開始
	if( flg )
	{
		m_lpMouse->Acquire();
	}
	// 偽なら入力制御しない
	else
	{	
		m_lpMouse->Unacquire();
	}
}

// キーボード入力状態取得
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

// マウス入力状態取得
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