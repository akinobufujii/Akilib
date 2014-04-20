//==================================================================================
/// @file	ApplicationWindow.cpp
/// @author ����͒�
//==================================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "AkiLib/ApplicationWindow.h"

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �f�t�H���g�̃E�B���h�E�v���V�[�W��(Unicode�p)
LRESULT CALLBACK CApplicationWindow::DefaultWindowProcedureW( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	// ���b�Z�[�W����
	switch( msg )
	{
	case WM_DESTROY:	// �E�B���h�E���j�����ꂽ�Ƃ�
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// �L�[�{�[�h�����ꂽ�Ƃ�
		switch( wparam )
		{
		case VK_ESCAPE:
			DestroyWindow( hwnd );
			break;
		}
	}

	return DefWindowProcW( hwnd, msg, wparam, lparam );
}

// �f�t�H���g�̃E�B���h�E�v���V�[�W��(�}���`�o�C�g�p)
LRESULT CALLBACK CApplicationWindow::DefaultWindowProcedureA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	// ���b�Z�[�W����
	switch( msg )
	{
	case WM_DESTROY:	// �E�B���h�E���j�����ꂽ�Ƃ�
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// �L�[�{�[�h�����ꂽ�Ƃ�
		switch( wparam )
		{
		case VK_ESCAPE:
			DestroyWindow( hwnd );
			break;
		}
	}

	return DefWindowProcA( hwnd, msg, wparam, lparam );
}

// �E�B���h�E�쐬(Unicode�p)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPWSTR windowName,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXW& windowClass )
{
	// �E�B���h�E�쐬(�����ɏo��)
	RECT rect;
	GetWindowRect( GetDesktopWindow(), &rect );

	return InitWindow(
		hinst,
		windowName,
		rect.right  / 2 - width  / 2,
		rect.bottom / 2 - height / 2,
		width,
		height,
		windowStyle,
		proc,
		windowClass );
}

// �E�B���h�E�쐬(�}���`�o�C�g�p)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPSTR windowName,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXA& windowClass )
{
	// �E�B���h�E�쐬(�����ɏo��)
	RECT rect;
	GetWindowRect( GetDesktopWindow(), &rect );

	return InitWindow(
		hinst,
		windowName,
		rect.right  / 2 - width  / 2,
		rect.bottom / 2 - height / 2,
		width,
		height,
		windowStyle,
		proc,
		windowClass );
}

// �E�B���h�E�쐬(Unicode�p)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPWSTR windowName,
				UINT x,
				UINT y,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXW& windowClass )
{
	try
	{
		m_hInst = hinst;

		// �C���X�^���X�n���h���ݒ�
		if( windowClass.hInstance == NULL )
		{
			windowClass.hInstance = m_hInst;
		}

		// �v���V�[�W���ݒ�
		if( windowClass.lpfnWndProc == NULL )
		{
			windowClass.lpfnWndProc = proc;
		}

		// �E�B���h�E���ݒ�
		if( windowClass.lpszClassName == NULL )
		{
			windowClass.lpszClassName = windowName;
		}

		// �n���ꂽ�E�B���h�E�N���X�œo�^
		if( RegisterClassExW( &windowClass ) == NULL )
		{
			throw L"�E�B���h�E�N���X�o�^���s";
		}

		// �E�B���h�E�쐬(�����ɏo��)
		RECT Size;
		GetWindowRect( GetDesktopWindow(), &Size );

		m_hWnd = CreateWindowW(
			windowName,	// �E�B���h�E�N���X��
			windowName,	// �^�C�g���o�[������
			windowStyle,// �E�B���h�E�X�^�C��(�T�C�Y�ύX����)
			x,			// x���W
			y,			// y���W
			width,		// ��
			height,		// ����
			NULL,		// �e�n���h��
			NULL,		// ���j���[�n���h��
			hinst,		// �C���X�^���X�n���h��
			NULL );		// ����f�[�^

		if( m_hWnd == NULL )
		{
			throw L"�E�B���h�E�쐬���s";
		}

		ShowWindow( m_hWnd, SW_SHOW );
		UpdateWindow( m_hWnd );
		SetForegroundWindow( m_hWnd );
	}
	catch( LPWSTR& errorInfo )
	{
		ErrorMessageBox( errorInfo );
		return false;
	}
	catch(...)
	{
		ErrorMessageBox( L"�E�B���h�E�쐬����\n�����s���̃G���[���������܂���" );
		return false;
	}

	return true;
}

// �E�B���h�E�쐬(�}���`�o�C�g�p)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPSTR windowName,
				UINT x,
				UINT y,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXA& windowClass )
{
	try
	{
		m_hInst = hinst;

		// �C���X�^���X�n���h���ݒ�
		if( windowClass.hInstance == NULL )
		{
			windowClass.hInstance = m_hInst;
		}

		// �v���V�[�W���ݒ�
		if( windowClass.lpfnWndProc == NULL )
		{
			windowClass.lpfnWndProc = proc;
		}

		// �E�B���h�E���ݒ�
		if( windowClass.lpszClassName == NULL )
		{
			windowClass.lpszClassName = windowName;
		}

		// �n���ꂽ�E�B���h�E�N���X�œo�^
		if( RegisterClassExA( &windowClass ) == NULL )
		{
			throw "�E�B���h�E�N���X�o�^���s";
		}

		// �E�B���h�E�쐬(�����ɏo��)
		RECT Size;
		GetWindowRect( GetDesktopWindow(), &Size );

		m_hWnd = CreateWindowA(
			windowName,	// �E�B���h�E�N���X��
			windowName,	// �^�C�g���o�[������
			windowStyle,// �E�B���h�E�X�^�C��(�T�C�Y�ύX����)
			x,			// x���W
			y,			// y���W
			width,		// ��
			height,		// ����
			NULL,		// �e�n���h��
			NULL,		// ���j���[�n���h��
			hinst,		// �C���X�^���X�n���h��
			NULL );		// ����f�[�^

		if( m_hWnd == NULL )
		{
			throw "�E�B���h�E�쐬���s";
		}

		ShowWindow( m_hWnd, SW_SHOW );
		UpdateWindow( m_hWnd );
		SetForegroundWindow( m_hWnd );
	}
	catch( LPSTR& errorInfo )
	{
		ErrorMessageBox( errorInfo );
		return false;
	}
	catch(...)
	{
		ErrorMessageBox( "�E�B���h�E�쐬����\n�����s���̃G���[���������܂���" );
		return false;
	}
	return true;
}

// �R���\�[���E�B���h�E�쐬
void CApplicationWindow::CreateConsoleWindow()
{
	// �R���\�[�������蓖�Ă�
	if( AllocConsole() )
	{
		// �R���\�[���𗧂��グ��
		FILE* lptemp;
		freopen_s( &lptemp, "CONOUT$", "w", stdout );

		// �T�C�Y�̓E�B���h�Ɠ�����
		HWND hwnd = GetConsoleWindow();
		RECT tempRect;
		GetWindowRect( m_hWnd, &tempRect );

		// �E�ׂɒu��
		SetWindowPos( 
			hwnd,
			nullptr,
			tempRect.right,
			tempRect.top,
			400,
			tempRect.bottom - tempRect.top,
			0 );

		// �t�H�[�J�X�̓��C���E�B���h�E��
		SetForegroundWindow( m_hWnd );
	}
}

// �f�o�b�O�E�B���h�E�쐬(Unicode�p)
void CApplicationWindow::CreateDebugWindowsW()
{
	RECT tempRect;
	GetWindowRect( GetDesktopWindow(), &tempRect );

	WindowClassW tempClass;

	tempClass.hInstance = m_hInst;
	tempClass.lpfnWndProc = DebugProcW;
	tempClass.lpszClassName = L"DebugWindow";

	// �n���ꂽ�E�B���h�E�N���X�œo�^
	if( RegisterClassExW( &tempClass ) == NULL )
	{
		//throw "�E�B���h�E�N���X�o�^���s";
	}

	m_hDebugWnd = CreateWindowW(
		L"DebugWindow",	// �E�B���h�E�N���X��
		L"�e�X�g������p",	// �^�C�g���o�[������
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// �E�B���h�E�X�^�C��(�T�C�Y�ύX����)
		1200,			// x���W
		0,				// y���W
		400,			// ��
		tempRect.bottom,		// ����
		m_hWnd,			// �e�n���h��
		NULL,			// ���j���[�n���h��
		m_hInst,		// �C���X�^���X�n���h��
		NULL );			// ����f�[�^

	// �t�H�[�J�X�̓��C���E�B���h�E��
	SetForegroundWindow( m_hWnd );
}

// �f�o�b�O�E�B���h�E�쐬(�}���`�o�C�g�p)
void CApplicationWindow::CreateDebugWindowsA()
{
	// �N���e�B�J���Z�N�V����
	InitializeCriticalSection( &m_CriticalSection );

	RECT tempRect;
	GetWindowRect( GetDesktopWindow(), &tempRect );

	WindowClassA tempClass;

	tempClass.hInstance = m_hInst;
	tempClass.lpfnWndProc = DebugProcA;
	tempClass.lpszClassName = "DebugWindow";

	// �n���ꂽ�E�B���h�E�N���X�œo�^
	if( RegisterClassExA( &tempClass ) == NULL )
	{
		//throw "�E�B���h�E�N���X�o�^���s";
	}

	m_hDebugWnd = CreateWindowA(
		"DebugWindow",	// �E�B���h�E�N���X��
		"�e�X�g������p",	// �^�C�g���o�[������
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// �E�B���h�E�X�^�C��(�T�C�Y�ύX����)
		1200,			// x���W
		0,				// y���W
		400,			// ��
		tempRect.bottom,// ����
		m_hWnd,			// �e�n���h��
		NULL,			// ���j���[�n���h��
		m_hInst,		// �C���X�^���X�n���h��
		NULL );			// ����f�[�^

	// �t�H�[�J�X�̓��C���E�B���h�E��
	SetForegroundWindow( m_hWnd );
}

// ���b�Z�[�W���[�v
void CApplicationWindow::MessageLoop( FNMSGLOOPFUNC fnloopFunc )
{
	MSG	Msg;

	do{
		if( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &Msg );
			DispatchMessage( &Msg );
		}
		else
		{
			if( fnloopFunc )
			{
				fnloopFunc();
			}
		}
	}while( Msg.message != WM_QUIT );
}