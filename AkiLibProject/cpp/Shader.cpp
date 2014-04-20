//==============================================================================
/// @file	Shader.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CShader::CShader()
{
}

// デストラクタ
CShader::~CShader()
{
	Release();
}

// 解放
void CShader::Release()
{
	// 頂点シェーダ解放
	std::for_each( m_VertexShaderMap.begin(), m_VertexShaderMap.end(), MapRelease<VERTEXSHADERMAPPAIR> );
	m_VertexShaderMap.clear();

	// ピクセルシェーダ解放
	std::for_each( m_PixelShaerMap.begin(), m_PixelShaerMap.end(), MapRelease<PIXELSHADERMAPPAIR> );
	m_PixelShaerMap.clear();

	// コンピュートシェーダ解放
	std::for_each( m_ComputeShaderMap.begin(), m_ComputeShaderMap.end(), MapRelease<COMPUTESHADERMAPPAIR> );
	m_ComputeShaderMap.clear();

	// ハルシェーダ解放
	std::for_each( m_HallShaderMap.begin(), m_HallShaderMap.end(), MapRelease<HALLSHADERMAPPAIR> );
	m_HallShaderMap.clear();

	// ドメインシェーダ解放
	std::for_each( m_DomainShaderMap.begin(), m_DomainShaderMap.end(), MapRelease<DOMAINSHADERMAPPAIR> );
	m_DomainShaderMap.clear();
}

// ファイルから読み込んでコンパイル
ID3DBlob* CShader::CompileShaderFromFile(LPCSTR funcName, LPCSTR version)
{
	LPD3D10BLOB lperror		= nullptr;	// エラー用ブロブ
	LPD3D10BLOB lptempBlob	= nullptr;	// 戻り値用ブロブ
	HANDLE		hfile		= nullptr;	// ファイルハンドル

	// 行列を列優先指定
	UINT flg = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef _DEBUG
	flg |= D3D10_SHADER_OPTIMIZATION_LEVEL0;	// 最適化しない
#else
	flg |= D3D10_SHADER_OPTIMIZATION_LEVEL3;	// 最大最適化	
#endif

	try
	{
		// hlslファイルのハンドルを取得
		hfile = CreateFileA( 
			m_FileName.c_str(),		// ファイル名
			GENERIC_READ,			// アクセスモード
			FILE_SHARE_READ,		// 共有モード
			nullptr,				// セキュリティ記述子
			OPEN_EXISTING,			// 作成方法
			FILE_ATTRIBUTE_NORMAL,	// ファイル属性
			nullptr );				// テンプレートファイルハンドル

		if( hfile == INVALID_HANDLE_VALUE )
		{
			throw "HLSLファイルオープン失敗";
		}

		// ファイルのサイズを取得
		DWORD fileSize = GetFileSize( hfile, nullptr );

		if( fileSize == -1 || fileSize == 0 )
		{
			throw "HlSLファイルサイズ取得失敗";
		}

		// 領域確保
		std::shared_ptr<BYTE> byteCode( AKINEW BYTE[fileSize] );

		// ファイル読み込み
		DWORD numBytesRead;
		if( ReadFile( hfile, byteCode.get(), fileSize, &numBytesRead, nullptr ) == FALSE )
		{
			throw "HLSLファイル読み込み失敗";
		}

		// HLSLコンパイル
		HRESULT hr = D3DCompile(
			reinterpret_cast<void*>( byteCode.get() ),	// HLSLバイトコード
			fileSize,									// ファイルの大きさ
			nullptr,									// マクロ
			nullptr,									// インクルード
			nullptr,									// エントリーポイント名
			funcName,									// コンパイル関数名
			version,									// コンパイルバージョン
			flg,										// コンパイルオプション
			0,											// エフェクトオプション
			&lptempBlob,								// コンパイルコードを受け取るブロブ
			&lperror );									// コンパイルエラーを受け取るブロブ

		if( FAILED( hr ) )
		{
#ifdef _UNICODE
			int strLen = MultiByteToWideChar( CP_ACP, 0, reinterpret_cast<CHAR*>( lperror->GetBufferPointer() ), lperror->GetBufferSize(), nullptr, 0 );

			std::shared_ptr<wchar_t> errorStr( AKINEW wchar_t[strLen] );

			MultiByteToWideChar( CP_ACP, 0, reinterpret_cast<char*>( lperror->GetBufferPointer() ), lperror->GetBufferSize(), errorStr.get(), strLen );

			ErrorMessageBox( errorStr.get() );
#else
			throw reinterpret_cast<LPSTR>( lperror->GetBufferPointer() );
#endif
		}
	}
	catch( LPCSTR& error )
	{
		if( hfile )
		{
			CloseHandle( hfile );
		}
		SafeRelease( lperror );

		ErrorMessageBox( error );
	}

	return lptempBlob;
}

// 頂点シェーダー作成
bool CShader::CreateVertexShader( 
	LPCSTR							funcName,
	LPCSTR							version,
	const D3D11_INPUT_ELEMENT_DESC* layout,
	size_t							layoutNum,
	ID3D11InputLayout**				lppoutLayout )
{
	// ブロブ作成
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );

	if( lpblob )
	{
		// 頂点シェーダ作成
		ID3D11VertexShader* lptempVertexShader = nullptr;
		m_lpDevice->CreateVertexShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempVertexShader );

		if( lptempVertexShader )
		{
			// マップに登録
			m_VertexShaderMap.insert( VERTEXSHADERMAPPAIR( funcName, lptempVertexShader ) );
		}

		// レイアウト作成
		if( layout && layoutNum > 0 && lppoutLayout )
		{
			HRESULT hr = m_lpDevice->CreateInputLayout( layout, layoutNum, lpblob->GetBufferPointer(), lpblob->GetBufferSize(), lppoutLayout );

			if( FAILED( hr ) )
			{
				ErrorMessageBox( "入力レイアウト作成失敗" );
			}
		}

		SafeRelease( lpblob );

		return true;
	}

	return false;
}

// ピクセルシェーダ作成
bool CShader::CreatePixelShader( LPCSTR funcName, LPCSTR version )
{
	// ブロブ作成
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// ピクセルシェーダ作成
		ID3D11PixelShader* lptempPixelShader = nullptr;
		m_lpDevice->CreatePixelShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempPixelShader );

		SafeRelease( lpblob );

		if( lptempPixelShader )
		{
			// マップに登録
			m_PixelShaerMap.insert( PIXELSHADERMAPPAIR( funcName, lptempPixelShader ) );
			return true;
		}
	}

	return false;
}

// コンピュートシェーダ作成
bool CShader::CreateComputeShader( LPCSTR funcName, LPCSTR version )
{
	// ブロブ作成
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// コンピュートシェーダ作成
		ID3D11ComputeShader* lptempComputeShader = nullptr;
		m_lpDevice->CreateComputeShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempComputeShader );

		SafeRelease( lpblob );

		if( lptempComputeShader )
		{
			// マップに登録
			m_ComputeShaderMap.insert( COMPUTESHADERMAPPAIR( funcName, lptempComputeShader ) );
			return true;
		}
	}

	return false;
}

// ハルシェーダ作成
bool CShader::CreateHallShader( LPCSTR funcName, LPCSTR version )
{
	// ブロブ作成
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// ハルシェーダ作成
		ID3D11HullShader* lptempHallShader = nullptr;
		m_lpDevice->CreateHullShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempHallShader );

		SafeRelease( lpblob );

		if( lptempHallShader )
		{
			// マップに登録
			m_HallShaderMap.insert( HALLSHADERMAPPAIR( funcName, lptempHallShader ) );
			return true;
		}
	}

	return false;
}

// ドメインシェーダ作成
bool CShader::CreateDomainShader( LPCSTR funcName, LPCSTR version )
{
	// ブロブ作成
	LPD3DBLOB lpblob = CompileShaderFromFile( funcName, version );
	
	if( lpblob )
	{
		// ドメインシェーダ作成
		ID3D11DomainShader* lptempDomainShader = nullptr;
		m_lpDevice->CreateDomainShader( lpblob->GetBufferPointer(), lpblob->GetBufferSize(), nullptr, &lptempDomainShader );

		SafeRelease( lpblob );

		if( lptempDomainShader )
		{
			// マップに登録
			m_DomainShaderMap.insert( DOMAINSHADERMAPPAIR( funcName, lptempDomainShader ) );
			return true;
		}
	}

	return false;
}