//==================================================================================
/// @file	ApplicationWindow.h
/// @author 藤井章暢
//==================================================================================
#ifndef __AKILIB_APPLICATIONWINDOW_H__
#define __AKILIB_APPLICATIONWINDOW_H__

namespace AkiLib
{
	

	//==============================================================================
	/// @brief デフォルトウィンドウクラス(Unicode用)
	//==============================================================================
	struct WindowClassW : public WNDCLASSEXW
	{
		/// @brief コンストラクタ(デフォルト値を設定する)
		WindowClassW()
		{
			cbSize			= sizeof( WindowClassW );									// 構造体の大きさ
			style			= CS_HREDRAW | CS_VREDRAW;								// ウィンドウスタイル
			lpfnWndProc		= NULL;													// プロシージャ登録
			hInstance		= NULL;													// インスタンスハンドル
			hIcon			= LoadIcon( NULL, IDI_APPLICATION );					// アイコン
			hCursor			= LoadCursor( NULL, IDC_ARROW );						// カーソル
			hbrBackground	= static_cast<HBRUSH>( GetStockObject( COLOR_WINDOW ) );// 背景色
			lpszClassName	= NULL;													// クラス名
			hIconSm			= NULL;													// 小さいアイコン
			cbWndExtra		= 0;													// 追加メモリ
			cbClsExtra		= 0;													// 追加メモリ
			lpszMenuName	= NULL;													// メニューハンドル
		}
	};

	//==============================================================================
	/// @brief デフォルトウィンドウクラス(マルチバイト用)
	//==============================================================================
	struct WindowClassA : public WNDCLASSEXA
	{
		/// @brief コンストラクタ(デフォルト値を設定する)
		WindowClassA()
		{
			cbSize			= sizeof( WindowClassA );									// 構造体の大きさ
			style			= CS_HREDRAW | CS_VREDRAW;								// ウィンドウスタイル
			lpfnWndProc		= NULL;													// プロシージャ登録
			hInstance		= NULL;													// インスタンスハンドル
			hIcon			= LoadIcon( NULL, IDI_APPLICATION );					// アイコン
			hCursor			= LoadCursor( NULL, IDC_ARROW );						// カーソル
			hbrBackground	= static_cast<HBRUSH>( GetStockObject( COLOR_WINDOW ) );// 背景色
			lpszClassName	= NULL;													// クラス名
			hIconSm			= NULL;													// 小さいアイコン
			cbWndExtra		= 0;													// 追加メモリ
			cbClsExtra		= 0;													// 追加メモリ
			lpszMenuName	= NULL;													// メニューハンドル
		}
	};

	// 文字コード対応
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
	/// @brief	アプリケーションウィンドウ生成クラス(シングルトン)
	//==============================================================================
	class CApplicationWindow : public CSingletonTemplate<CApplicationWindow>
	{
	public:
		//==============================================================================
		// フレンドクラス指定
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// 型宣言
		//==============================================================================
		typedef void (*FNMSGLOOPFUNC)();	///< メッセージループ実行関数

		//==============================================================================
		// メソッド
		//==============================================================================

		/// @brief	デストラクタ
		~CApplicationWindow(){}

		/// @brief デフォルトのウィンドウプロシージャ(Unicode用)
		/// @param[in]	hwnd	ウィンドウハンドル
		/// @param[in]	msg		メッセージ
		/// @param[in]	wparam	パラメータ
		/// @param[in]	lparam	パラメータ
		/// @return		基本的に0を返す
		static LRESULT CALLBACK DefaultWindowProcedureW( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

		/// @brief デフォルトのウィンドウプロシージャ(マルチバイト用)
		/// @param[in]	hwnd	ウィンドウハンドル
		/// @param[in]	msg		メッセージ
		/// @param[in]	wparam	パラメータ
		/// @param[in]	lparam	パラメータ
		/// @return		基本的に0を返す
		static LRESULT CALLBACK DefaultWindowProcedureA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

		/// @brief ウィンドウを中心に作成(Unicode用)
		/// @param[in]	hinst		インスタンスハンドル
		/// @param[in]	windowName	ウィンドウネーム(タイトルバーの名前にもなる)
		/// @param[in]	width		ウィンドウの幅
		/// @param[in]	height		ウィンドウの高さ
		/// @param[in]	windowStyle	ウィンドウスタイル(デフォルトはサイズ変更無効ウィンドウ)
		/// @param[in]	proc		ウィンドウプロシージャ(設定しなければデフォルトウィンドウプロシージャが設定される)
		/// @param[in]	windowClass	ウィンドウクラス(設定しなければ、デフォルトのウィンドウクラスが渡される)
		bool InitWindow( 
			HINSTANCE hinst,
			LPWSTR windowName,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXW& windowClass = WindowClassW() );

		/// @brief ウィンドウを中心に作成(マルチバイト用)
		/// @param[in]	hinst		インスタンスハンドル
		/// @param[in]	windowName	ウィンドウネーム(タイトルバーの名前にもなる)
		/// @param[in]	width		ウィンドウの幅
		/// @param[in]	height		ウィンドウの高さ
		/// @param[in]	windowStyle	ウィンドウスタイル(デフォルトはサイズ変更無効ウィンドウ)
		/// @param[in]	proc		ウィンドウプロシージャ(設定しなければデフォルトウィンドウプロシージャが設定される)
		/// @param[in]	windowClass	ウィンドウクラス(設定しなければ、デフォルトのウィンドウクラスが渡される)
		bool InitWindow( 
			HINSTANCE hinst,
			LPSTR windowName,
			UINT width,
			UINT height,
			UINT windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			WNDPROC proc = DefaultWindowProcedure,
			WNDCLASSEXA& windowClass = WindowClassA() );

		/// @brief ウィンドウを指定座標に作成(Unicode用)
		/// @param[in]	hinst		インスタンスハンドル
		/// @param[in]	windowName	ウィンドウネーム(タイトルバーの名前にもなる)
		/// @param[in]	x			ウィンドウのY座標
		/// @param[in]	y			ウィンドウのY座標
		/// @param[in]	width		ウィンドウの幅
		/// @param[in]	height		ウィンドウの高さ
		/// @param[in]	windowStyle	ウィンドウスタイル(デフォルトはサイズ変更無効ウィンドウ)
		/// @param[in]	proc		ウィンドウプロシージャ(設定しなければデフォルトウィンドウプロシージャが設定される)
		/// @param[in]	windowClass	ウィンドウクラス(設定しなければ、デフォルトのウィンドウクラスが渡される)
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

		/// @brief ウィンドウを指定座標に作成(マルチバイト用)
		/// @param[in]	hinst		インスタンスハンドル
		/// @param[in]	windowName	ウィンドウネーム(タイトルバーの名前にもなる)
		/// @param[in]	x			ウィンドウのY座標
		/// @param[in]	y			ウィンドウのY座標
		/// @param[in]	width		ウィンドウの幅
		/// @param[in]	height		ウィンドウの高さ
		/// @param[in]	windowStyle	ウィンドウスタイル(デフォルトはサイズ変更無効ウィンドウ)
		/// @param[in]	proc		ウィンドウプロシージャ(設定しなければデフォルトウィンドウプロシージャが設定される)
		/// @param[in]	windowClass	ウィンドウクラス(設定しなければ、デフォルトのウィンドウクラスが渡される)
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

		/// @brief	コンソールウィンドウを立ち上げる
		void CreateConsoleWindow();

		/// @brief	デバッグウィンドウを立ち上げる
		void CreateDebugWindowsW();

		/// @brief	デバッグウィンドウを立ち上げる
		void CreateDebugWindowsA();

		/// @brief メッセージループ
		/// @param[in]	fnloopFunc	メッセージループで実行する関数
		void MessageLoop( FNMSGLOOPFUNC fnloopFunc );

		/// @brief インスタンスハンドル獲得
		/// @return	インスタンスハンドルを返す
		inline HINSTANCE GetInsttanceHandle() const
		{
			return m_hInst;
		}

		/// @brief ウィンドウハンドル獲得
		/// @return	ウィンドウハンドルを返す
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
		// フィールド
		//==============================================================================
		HINSTANCE	m_hInst;			///< インスタンスハンドル
		HWND		m_hWnd;				///< ウィンドウハンドル
		HWND		m_hDebugWnd;		///< ウィンドウハンドル
		UINT		m_Width;			///< ウィンドウの幅
		UINT		m_Height;			///< ウィンドウの高さ
		std::string	m_DebugStringsA[50];///< デバッグ文字列
		CRITICAL_SECTION	m_CriticalSection;

		//==============================================================================
		// メソッド
		//==============================================================================

		/// @brief コンストラクタ
		CApplicationWindow():
			m_hInst	( NULL ),
			m_hWnd	( NULL ),
			m_Width	( 0 ),
			m_Height( 0 )
		{
			for( int i = 0; i < _countof(m_DebugStringsA); ++i )
			{
				m_DebugStringsA[i] = "詰まってないよ";
			}
			
		}
	};

	inline LRESULT CALLBACK DebugProcA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
	{
		// メッセージ処理
		switch( msg )
		{
		case WM_DESTROY:	// ウィンドウが破棄されたとき
			PostQuitMessage( 0 );
			break;

		case WM_PAINT:	// キーボード押されたとき
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
		// メッセージ処理
		switch( msg )
		{
		case WM_DESTROY:	// ウィンドウが破棄されたとき
			PostQuitMessage( 0 );
			break;

		case WM_PAINT:	// キーボード押されたとき
			PAINTSTRUCT lptemp;
			TEXTMETRICW metrix;

			HDC hdc = BeginPaint( hwnd, &lptemp );
			GetTextMetricsW( hdc, &metrix );

			TextOutW( hdc, 0, 0, L"Hello World", strlen("HellowWorld") );
			TextOutW( hdc, 0, 20, L"日本語も行けまっせ", strlen("日本語も行けまっせ") / 2 );
			TextOutW( hdc, 0, 40, L"日本語も行けまっせ", strlen("日本語も行けまっせ") / 2 );
			EndPaint( hwnd, &lptemp );
		}

		return DefWindowProcW( hwnd, msg, wparam, lparam );
	}
}

#define APP AkiLib::CApplicationWindow::GetInstance()

#endif	// #ifndef __AKILIB_APPLICATIONWINDOW_H__