//==================================================================================
/// @file	ApplicationWindow.h
/// @author ����͒�
//==================================================================================
#ifndef __AKILIB_APPLICATIONWINDOW_H__
#define __AKILIB_APPLICATIONWINDOW_H__

namespace AkiLib
{
	

	//==============================================================================
	/// @brief �f�t�H���g�E�B���h�E�N���X(Unicode�p)
	//==============================================================================
	struct WindowClassW : public WNDCLASSEXW
	{
		/// @brief �R���X�g���N�^(�f�t�H���g�l��ݒ肷��)
		WindowClassW()
		{
			cbSize			= sizeof( WindowClassW );									// �\���̂̑傫��
			style			= CS_HREDRAW | CS_VREDRAW;								// �E�B���h�E�X�^�C��
			lpfnWndProc		= NULL;													// �v���V�[�W���o�^
			hInstance		= NULL;													// �C���X�^���X�n���h��
			hIcon			= LoadIcon( NULL, IDI_APPLICATION );					// �A�C�R��
			hCursor			= LoadCursor( NULL, IDC_ARROW );						// �J�[�\��
			hbrBackground	= static_cast<HBRUSH>( GetStockObject( COLOR_WINDOW ) );// �w�i�F
			lpszClassName	= NULL;													// �N���X��
			hIconSm			= NULL;													// �������A�C�R��
			cbWndExtra		= 0;													// �ǉ�������
			cbClsExtra		= 0;													// �ǉ�������
			lpszMenuName	= NULL;													// ���j���[�n���h��
		}
	};

	//==============================================================================
	/// @brief �f�t�H���g�E�B���h�E�N���X(�}���`�o�C�g�p)
	//==============================================================================
	struct WindowClassA : public WNDCLASSEXA
	{
		/// @brief �R���X�g���N�^(�f�t�H���g�l��ݒ肷��)
		WindowClassA()
		{
			cbSize			= sizeof( WindowClassA );									// �\���̂̑傫��
			style			= CS_HREDRAW | CS_VREDRAW;								// �E�B���h�E�X�^�C��
			lpfnWndProc		= NULL;													// �v���V�[�W���o�^
			hInstance		= NULL;													// �C���X�^���X�n���h��
			hIcon			= LoadIcon( NULL, IDI_APPLICATION );					// �A�C�R��
			hCursor			= LoadCursor( NULL, IDC_ARROW );						// �J�[�\��
			hbrBackground	= static_cast<HBRUSH>( GetStockObject( COLOR_WINDOW ) );// �w�i�F
			lpszClassName	= NULL;													// �N���X��
			hIconSm			= NULL;													// �������A�C�R��
			cbWndExtra		= 0;													// �ǉ�������
			cbClsExtra		= 0;													// �ǉ�������
			lpszMenuName	= NULL;													// ���j���[�n���h��
		}
	};

	// �����R�[�h�Ή�
#ifdef UNICODE
	typedef WindowClassW WindowClass;
#define DefaultWindowProcedure DefaultWindowProcedureW
#define CreateDebugWindows CreateDebugWindowsW
#else
	typedef WindowClassA WindowClass;
#define DefaultWindowProcedure DefaultWindowProcedureA
#define CreateDebugWindows CreateDebugWindowsA

#ifdef _DEBUG
#define AkiDebugPrintf( index, str ) AkiLib::CApplicationWindow::GetInstance()->DebugStringA( index, str )
#else
#define AkiDebugPrintf( index, str )
#endif // DEBUG

#endif

	//==============================================================================
	/// @brief	�A�v���P�[�V�����E�B���h�E�����N���X(�V���O���g��)
	//==============================================================================
	class CApplicationWindow : public CSingletonTemplate<CApplicationWindow>
	{
	public:
		//==============================================================================
		// �t�����h�N���X�w��
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// �^�錾
		//==============================================================================
		typedef void (*FNMSGLOOPFUNC)();	///< ���b�Z�[�W���[�v���s�֐�

		//==============================================================================
		// ���\�b�h
		//==============================================================================

		/// @brief	�f�X�g���N�^
		~CApplicationWindow(){}

		/// @brief �f�t�H���g�̃E�B���h�E�v���V�[�W��(Unicode�p)
		/// @param[in]	hwnd	�E�B���h�E�n���h��
		/// @param[in]	msg		���b�Z�[�W
		/// @param[in]	wparam	�p�����[�^
		/// @param[in]	lparam	�p�����[�^
		/// @return		��{�I��0��Ԃ�
		static LRESULT CALLBACK DefaultWindowProcedureW( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

		/// @brief �f�t�H���g�̃E�B���h�E�v���V�[�W��(�}���`�o�C�g�p)
		/// @param[in]	hwnd	�E�B���h�E�n���h��
		/// @param[in]	msg		���b�Z�[�W
		/// @param[in]	wparam	�p�����[�^
		/// @param[in]	lparam	�p�����[�^
		/// @return		��{�I��0��Ԃ�
		static LRESULT CALLBACK DefaultWindowProcedureA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

		/// @brief �E�B���h�E�𒆐S�ɍ쐬(Unicode�p)
		/// @param[in]	hinst		�C���X�^���X�n���h��
		/// @param[in]	windowName	�E�B���h�E�l�[��(�^�C�g���o�[�̖��O�ɂ��Ȃ�)
		/// @param[in]	width		�E�B���h�E�̕�
		/// @param[in]	height		�E�B���h�E�̍���
		/// @param[in]	windowStyle	�E�B���h�E�X�^�C��(�f�t�H���g�̓T�C�Y�ύX�����E�B���h�E)
		/// @param[in]	proc		�E�B���h�E�v���V�[�W��(�ݒ肵�Ȃ���΃f�t�H���g�E�B���h�E�v���V�[�W�����ݒ肳���)
		/// @param[in]	windowClass	�E�B���h�E�N���X(�ݒ肵�Ȃ���΁A�f�t�H���g�̃E�B���h�E�N���X���n�����)
		bool InitWindow( 
			HINSTANCE hinst,
			LPWSTR windowName,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXW& windowClass = WindowClassW() );

		/// @brief �E�B���h�E�𒆐S�ɍ쐬(�}���`�o�C�g�p)
		/// @param[in]	hinst		�C���X�^���X�n���h��
		/// @param[in]	windowName	�E�B���h�E�l�[��(�^�C�g���o�[�̖��O�ɂ��Ȃ�)
		/// @param[in]	width		�E�B���h�E�̕�
		/// @param[in]	height		�E�B���h�E�̍���
		/// @param[in]	windowStyle	�E�B���h�E�X�^�C��(�f�t�H���g�̓T�C�Y�ύX�����E�B���h�E)
		/// @param[in]	proc		�E�B���h�E�v���V�[�W��(�ݒ肵�Ȃ���΃f�t�H���g�E�B���h�E�v���V�[�W�����ݒ肳���)
		/// @param[in]	windowClass	�E�B���h�E�N���X(�ݒ肵�Ȃ���΁A�f�t�H���g�̃E�B���h�E�N���X���n�����)
		bool InitWindow( 
			HINSTANCE hinst,
			LPSTR windowName,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXA& windowClass = WindowClassA() );

		/// @brief �E�B���h�E���w����W�ɍ쐬(Unicode�p)
		/// @param[in]	hinst		�C���X�^���X�n���h��
		/// @param[in]	windowName	�E�B���h�E�l�[��(�^�C�g���o�[�̖��O�ɂ��Ȃ�)
		/// @param[in]	x			�E�B���h�E��Y���W
		/// @param[in]	y			�E�B���h�E��Y���W
		/// @param[in]	width		�E�B���h�E�̕�
		/// @param[in]	height		�E�B���h�E�̍���
		/// @param[in]	windowStyle	�E�B���h�E�X�^�C��(�f�t�H���g�̓T�C�Y�ύX�����E�B���h�E)
		/// @param[in]	proc		�E�B���h�E�v���V�[�W��(�ݒ肵�Ȃ���΃f�t�H���g�E�B���h�E�v���V�[�W�����ݒ肳���)
		/// @param[in]	windowClass	�E�B���h�E�N���X(�ݒ肵�Ȃ���΁A�f�t�H���g�̃E�B���h�E�N���X���n�����)
		bool InitWindow( 
			HINSTANCE hinst,
			LPWSTR windowName,
			UINT x,
			UINT y,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXW& windowClass = WindowClassW() );

		/// @brief �E�B���h�E���w����W�ɍ쐬(�}���`�o�C�g�p)
		/// @param[in]	hinst		�C���X�^���X�n���h��
		/// @param[in]	windowName	�E�B���h�E�l�[��(�^�C�g���o�[�̖��O�ɂ��Ȃ�)
		/// @param[in]	x			�E�B���h�E��Y���W
		/// @param[in]	y			�E�B���h�E��Y���W
		/// @param[in]	width		�E�B���h�E�̕�
		/// @param[in]	height		�E�B���h�E�̍���
		/// @param[in]	windowStyle	�E�B���h�E�X�^�C��(�f�t�H���g�̓T�C�Y�ύX�����E�B���h�E)
		/// @param[in]	proc		�E�B���h�E�v���V�[�W��(�ݒ肵�Ȃ���΃f�t�H���g�E�B���h�E�v���V�[�W�����ݒ肳���)
		/// @param[in]	windowClass	�E�B���h�E�N���X(�ݒ肵�Ȃ���΁A�f�t�H���g�̃E�B���h�E�N���X���n�����)
		bool InitWindow( 
			HINSTANCE hinst,
			LPSTR windowName,
			UINT x,
			UINT y,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXA& windowClass = WindowClassA() );

		/// @brief	�R���\�[���E�B���h�E�𗧂��グ��
		void CreateConsoleWindow();

		/// @brief	�f�o�b�O�E�B���h�E�𗧂��グ��
		void CreateDebugWindowsW();

		/// @brief	�f�o�b�O�E�B���h�E�𗧂��グ��
		void CreateDebugWindowsA();

		/// @brief ���b�Z�[�W���[�v
		/// @param[in]	fnloopFunc	���b�Z�[�W���[�v�Ŏ��s����֐�
		void MessageLoop( FNMSGLOOPFUNC fnloopFunc );

		/// @brief �C���X�^���X�n���h���l��
		/// @return	�C���X�^���X�n���h����Ԃ�
		inline HINSTANCE GetInsttanceHandle() const
		{
			return m_hInst;
		}

		/// @brief �E�B���h�E�n���h���l��
		/// @return	�E�B���h�E�n���h����Ԃ�
		inline HWND GetWindowHandle() const
		{
			return m_hWnd;
		}

		void DebugStringA( int index, LPCSTR str )
		{
			EnterCriticalSection( &m_CriticalSection );
			if( index >= 0 && index < _countof( m_DebugStringsA ) )
			{
				m_DebugStringsA[index] = str;
			}
			LeaveCriticalSection( &m_CriticalSection );
			PostMessageA( m_hDebugWnd, WM_PAINT, 0, 0 );
		}

	private:
		friend LRESULT CALLBACK DebugProcA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		HINSTANCE	m_hInst;			///< �C���X�^���X�n���h��
		HWND		m_hWnd;				///< �E�B���h�E�n���h��
		HWND		m_hDebugWnd;		///< �E�B���h�E�n���h��
		UINT		m_Width;			///< �E�B���h�E�̕�
		UINT		m_Height;			///< �E�B���h�E�̍���
		std::string	m_DebugStringsA[50];///< �f�o�b�O������
		CRITICAL_SECTION	m_CriticalSection;

		//==============================================================================
		// ���\�b�h
		//==============================================================================

		/// @brief �R���X�g���N�^
		CApplicationWindow():
			m_hInst	( NULL ),
			m_hWnd	( NULL ),
			m_Width	( 0 ),
			m_Height( 0 )
		{
			for( int i = 0; i < _countof(m_DebugStringsA); ++i )
			{
				m_DebugStringsA[i] = "�l�܂��ĂȂ���";
			}
			
		}
	};

	inline LRESULT CALLBACK DebugProcA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
	{
		// ���b�Z�[�W����
		switch( msg )
		{
		case WM_DESTROY:	// �E�B���h�E���j�����ꂽ�Ƃ�
			PostQuitMessage( 0 );
			break;

		case WM_PAINT:	// �L�[�{�[�h�����ꂽ�Ƃ�
			PAINTSTRUCT lptemp;
			TEXTMETRICA metrix;

			HDC hdc = BeginPaint( hwnd, &lptemp );
			GetTextMetricsA( hdc, &metrix );
			RECT rect;
			GetClientRect( hwnd, &rect );
			SetTextColor( hdc, RGB( 0xff, 0, 0 ) );

			EnterCriticalSection( &AkiLib::CApplicationWindow::GetInstance()->m_CriticalSection );

			//for( int i = 0; i < _countof(AkiLib::CApplicationWindow::GetInstance()->m_DebugStringsA); ++i )
			//{
			//	static int cnt = 0;
			//	cnt++;
			//	std::string str = AkiLib::CApplicationWindow::GetInstance()->m_DebugStringsA[i];
			//	char str2[0xff];
			//	sprintf( str2, "%d", cnt );
			//	rect.top += DrawTextA( hdc, str2, -1, &rect, DT_LEFT );
			//}

			LeaveCriticalSection( &AkiLib::CApplicationWindow::GetInstance()->m_CriticalSection );
			EndPaint( hwnd, &lptemp );
		}

		return DefWindowProcA( hwnd, msg, wparam, lparam );
	}

	inline LRESULT CALLBACK DebugProcW( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
	{
		// ���b�Z�[�W����
		switch( msg )
		{
		case WM_DESTROY:	// �E�B���h�E���j�����ꂽ�Ƃ�
			PostQuitMessage( 0 );
			break;

		case WM_PAINT:	// �L�[�{�[�h�����ꂽ�Ƃ�
			PAINTSTRUCT lptemp;
			TEXTMETRICW metrix;

			HDC hdc = BeginPaint( hwnd, &lptemp );
			GetTextMetricsW( hdc, &metrix );

			TextOutW( hdc, 0, 0, L"Hello World", strlen("HellowWorld") );
			TextOutW( hdc, 0, 20, L"���{����s���܂���", strlen("���{����s���܂���") / 2 );
			TextOutW( hdc, 0, 40, L"���{����s���܂���", strlen("���{����s���܂���") / 2 );
			EndPaint( hwnd, &lptemp );
		}

		return DefWindowProcW( hwnd, msg, wparam, lparam );
	}
}

#define APP AkiLib::CApplicationWindow::GetInstance()

#endif	// #ifndef __AKILIB_APPLICATIONWINDOW_H__