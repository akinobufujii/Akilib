//==================================================================================
/// @file	ApplicationWindow.cpp
/// @author 藤井章暢
//==================================================================================

//==============================================================================
// インクルード
//==============================================================================
#include "AkiLib/ApplicationWindow.h"

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// デフォルトのウィンドウプロシージャ(Unicode用)
LRESULT CALLBACK CApplicationWindow::DefaultWindowProcedureW( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	// メッセージ処理
	switch( msg )
	{
	case WM_DESTROY:	// ウィンドウが破棄されたとき
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// キーボード押されたとき
		switch( wparam )
		{
		case VK_ESCAPE:
			DestroyWindow( hwnd );
			break;
		}
	}

	return DefWindowProcW( hwnd, msg, wparam, lparam );
}

// デフォルトのウィンドウプロシージャ(マルチバイト用)
LRESULT CALLBACK CApplicationWindow::DefaultWindowProcedureA( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	// メッセージ処理
	switch( msg )
	{
	case WM_DESTROY:	// ウィンドウが破棄されたとき
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// キーボード押されたとき
		switch( wparam )
		{
		case VK_ESCAPE:
			DestroyWindow( hwnd );
			break;
		}
	}

	return DefWindowProcA( hwnd, msg, wparam, lparam );
}

// ウィンドウ作成(Unicode用)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPWSTR windowName,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXW& windowClass )
{
	// ウィンドウ作成(中央に出す)
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

// ウィンドウ作成(マルチバイト用)
bool CApplicationWindow::InitWindow(HINSTANCE hinst,
				LPSTR windowName,
				UINT width,
				UINT height,
				UINT windowStyle,
				WNDPROC proc,
				WNDCLASSEXA& windowClass )
{
	// ウィンドウ作成(中央に出す)
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

// ウィンドウ作成(Unicode用)
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

		// インスタンスハンドル設定
		if( windowClass.hInstance == NULL )
		{
			windowClass.hInstance = m_hInst;
		}

		// プロシージャ設定
		if( windowClass.lpfnWndProc == NULL )
		{
			windowClass.lpfnWndProc = proc;
		}

		// ウィンドウ名設定
		if( windowClass.lpszClassName == NULL )
		{
			windowClass.lpszClassName = windowName;
		}

		// 渡されたウィンドウクラスで登録
		if( RegisterClassExW( &windowClass ) == NULL )
		{
			throw L"ウィンドウクラス登録失敗";
		}

		// ウィンドウ作成(中央に出す)
		RECT Size;
		GetWindowRect( GetDesktopWindow(), &Size );

		m_hWnd = CreateWindowW(
			windowName,	// ウィンドウクラス名
			windowName,	// タイトルバー文字列
			windowStyle,// ウィンドウスタイル(サイズ変更無効)
			x,			// x座標
			y,			// y座標
			width,		// 幅
			height,		// 高さ
			NULL,		// 親ハンドル
			NULL,		// メニューハンドル
			hinst,		// インスタンスハンドル
			NULL );		// 特殊データ

		if( m_hWnd == NULL )
		{
			throw L"ウィンドウ作成失敗";
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
		ErrorMessageBox( L"ウィンドウ作成時に\n原因不明のエラーが発生しました" );
		return false;
	}

	return true;
}

// ウィンドウ作成(マルチバイト用)
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

		// インスタンスハンドル設定
		if( windowClass.hInstance == NULL )
		{
			windowClass.hInstance = m_hInst;
		}

		// プロシージャ設定
		if( windowClass.lpfnWndProc == NULL )
		{
			windowClass.lpfnWndProc = proc;
		}

		// ウィンドウ名設定
		if( windowClass.lpszClassName == NULL )
		{
			windowClass.lpszClassName = windowName;
		}

		// 渡されたウィンドウクラスで登録
		if( RegisterClassExA( &windowClass ) == NULL )
		{
			throw "ウィンドウクラス登録失敗";
		}

		// ウィンドウ作成(中央に出す)
		RECT Size;
		GetWindowRect( GetDesktopWindow(), &Size );

		m_hWnd = CreateWindowA(
			windowName,	// ウィンドウクラス名
			windowName,	// タイトルバー文字列
			windowStyle,// ウィンドウスタイル(サイズ変更無効)
			x,			// x座標
			y,			// y座標
			width,		// 幅
			height,		// 高さ
			NULL,		// 親ハンドル
			NULL,		// メニューハンドル
			hinst,		// インスタンスハンドル
			NULL );		// 特殊データ

		if( m_hWnd == NULL )
		{
			throw "ウィンドウ作成失敗";
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
		ErrorMessageBox( "ウィンドウ作成時に\n原因不明のエラーが発生しました" );
		return false;
	}
	return true;
}

// コンソールウィンドウ作成
void CApplicationWindow::CreateConsoleWindow()
{
	// コンソールを割り当てる
	if( AllocConsole() )
	{
		// コンソールを立ち上げる
		FILE* lptemp;
		freopen_s( &lptemp, "CONOUT$", "w", stdout );

		// サイズはウィンドと同じで
		HWND hwnd = GetConsoleWindow();
		RECT tempRect;
		GetWindowRect( m_hWnd, &tempRect );

		// 右隣に置く
		SetWindowPos( 
			hwnd,
			nullptr,
			tempRect.right,
			tempRect.top,
			400,
			tempRect.bottom - tempRect.top,
			0 );

		// フォーカスはメインウィンドウに
		SetForegroundWindow( m_hWnd );
	}
}

// デバッグウィンドウ作成(Unicode用)
void CApplicationWindow::CreateDebugWindowsW()
{
	RECT tempRect;
	GetWindowRect( GetDesktopWindow(), &tempRect );

	WindowClassW tempClass;

	tempClass.hInstance = m_hInst;
	tempClass.lpfnWndProc = DebugProcW;
	tempClass.lpszClassName = L"DebugWindow";

	// 渡されたウィンドウクラスで登録
	if( RegisterClassExW( &tempClass ) == NULL )
	{
		//throw "ウィンドウクラス登録失敗";
	}

	m_hDebugWnd = CreateWindowW(
		L"DebugWindow",	// ウィンドウクラス名
		L"テスト文字列用",	// タイトルバー文字列
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// ウィンドウスタイル(サイズ変更無効)
		1200,			// x座標
		0,				// y座標
		400,			// 幅
		tempRect.bottom,		// 高さ
		m_hWnd,			// 親ハンドル
		NULL,			// メニューハンドル
		m_hInst,		// インスタンスハンドル
		NULL );			// 特殊データ

	// フォーカスはメインウィンドウに
	SetForegroundWindow( m_hWnd );
}

// デバッグウィンドウ作成(マルチバイト用)
void CApplicationWindow::CreateDebugWindowsA()
{
	// クリティカルセクション
	InitializeCriticalSection( &m_CriticalSection );

	RECT tempRect;
	GetWindowRect( GetDesktopWindow(), &tempRect );

	WindowClassA tempClass;

	tempClass.hInstance = m_hInst;
	tempClass.lpfnWndProc = DebugProcA;
	tempClass.lpszClassName = "DebugWindow";

	// 渡されたウィンドウクラスで登録
	if( RegisterClassExA( &tempClass ) == NULL )
	{
		//throw "ウィンドウクラス登録失敗";
	}

	m_hDebugWnd = CreateWindowA(
		"DebugWindow",	// ウィンドウクラス名
		"テスト文字列用",	// タイトルバー文字列
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// ウィンドウスタイル(サイズ変更無効)
		1200,			// x座標
		0,				// y座標
		400,			// 幅
		tempRect.bottom,// 高さ
		m_hWnd,			// 親ハンドル
		NULL,			// メニューハンドル
		m_hInst,		// インスタンスハンドル
		NULL );			// 特殊データ

	// フォーカスはメインウィンドウに
	SetForegroundWindow( m_hWnd );
}

// メッセージループ
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