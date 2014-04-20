//==============================================================================
/// @file	FontManager.cpp
/// @brief	フォント関連実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CFontManager::CFontManager()
{
}

// デストラクタ
CFontManager::~CFontManager()
{
}

// 指定文字テクスチャ生成(Unicode用)
SPCHARDATA CFontManager::CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::wstring& charactor, UINT fontsize)
{
	// フォントの生成
	LOGFONTW logFont = 
	{
		fontsize,
		0, 0, 0, 0, 0, 0, 0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"ＭＳ 明朝"
	};

	HFONT hFont = CreateFontIndirectW( &logFont );
	if( hFont == nullptr )
	{
		return false;
	}

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC( nullptr );
	HFONT oldFont = reinterpret_cast<HFONT>( SelectObject( hdc, hFont ) );

	// 文字コード取得
	UINT code = static_cast<UINT>( charactor[0] );

	// フォントビットマップ取得
	// テキストメトリックス取得
	TEXTMETRICW textMetrix;
	GetTextMetricsW( hdc, &textMetrix );

	// グリフ(文字の視覚的な表現)を取得
	GLYPHMETRICS glyphMetrix;
	const MAT2 identityMatrix = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };	// 文字の変換行列(単位行列)

	// まずは配列作成のための大きさを取得する
	DWORD size = GetGlyphOutlineW( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, 0, nullptr, &identityMatrix );
	std::shared_ptr<BYTE> spbitmapInfo( AKINEW BYTE[size] );

	// グリフ取得
	GetGlyphOutlineW( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, size, spbitmapInfo.get(), &identityMatrix );

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( nullptr, hdc );

	// テクスチャ作成
	SPCHARDATA	spTemp( AKINEW CHARACTORDATA() );

	D3D11_TEXTURE2D_DESC texture2Desc;
	ZeroMemory( &texture2Desc, sizeof( texture2Desc ) );
	texture2Desc.Width				= glyphMetrix.gmCellIncX;
	texture2Desc.Height				= textMetrix.tmHeight;
	texture2Desc.MipLevels			= 1;
	texture2Desc.ArraySize			= 1;
	texture2Desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2Desc.SampleDesc.Count	= 1;
	texture2Desc.Usage				= D3D11_USAGE_DYNAMIC;
	texture2Desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texture2Desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	// 幅高さを保存
	spTemp->Size.x = static_cast<float>( texture2Desc.Width );
	spTemp->Size.y = static_cast<float>( texture2Desc.Height );

	// テクスチャ作成
	if( FAILED( lpdevice->CreateTexture2D( &texture2Desc, 0, &spTemp->lpTexture ) ) )
	{
		ErrorMessageBox( L"フォントテクスチャ作成失敗" );
	}

	// リソースビュー作成
	if( FAILED( lpdevice->CreateShaderResourceView( spTemp->lpTexture, nullptr, &spTemp->lpResourceView ) ) )
	{
		ErrorMessageBox( L"フォントリソースビュー作成失敗" );
	}

	// フォント情報の書き込み
	// 書き出し位置(左上)
	int offsetX = glyphMetrix.gmptGlyphOrigin.x;
	int offsetY = textMetrix.tmAscent - glyphMetrix.gmptGlyphOrigin.y;
	
	// フォントビットマップの幅高
	int bitmapWidth = glyphMetrix.gmBlackBoxX + ( 4 - ( glyphMetrix.gmBlackBoxX % 4 ) ) % 4;	// 4バイト境界のため
	int bitmapHeight = glyphMetrix.gmBlackBoxY;

	// アルファ値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int Level = 17;
	DWORD alpha, color;

	// テクスチャに書き込み
	D3D11_MAPPED_SUBRESOURCE mapResource;
	lpcontext->Map( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ), D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

	// 0クリアしておく
	ZeroMemory( mapResource.pData, mapResource.RowPitch * textMetrix.tmHeight );

	// テクスチャに文字情報を白色で書き込んでいく
	for( int y = offsetY; y < offsetY + bitmapHeight; y++ )
	{
		for( int x = offsetX; x < offsetX + bitmapWidth; x++ )
		{
			alpha = ( 0xff * spbitmapInfo.get()[x - offsetX + bitmapWidth * ( y - offsetY )] ) / ( Level - 1 );
			color = 0x00ffffff | ( alpha << 24 );// ARGBの順番

			memcpy( (BYTE*)mapResource.pData + mapResource.RowPitch * y + 4 * x, &color, sizeof( DWORD ) );
		}
	}

	lpcontext->Unmap( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ) );

	return spTemp;
}

// 指定文字テクスチャ生成(マルチバイト用)
SPCHARDATA CFontManager::CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::string& charactor, UINT fontsize)
{
	// フォントの生成
	LOGFONTA logFont = 
	{
		fontsize,
		0, 0, 0, 0, 0, 0, 0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"ＭＳ 明朝"
	};

	HFONT hFont = CreateFontIndirectA( &logFont );
	if( hFont == nullptr )
	{
		return false;
	}

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC( nullptr );
	HFONT oldFont = reinterpret_cast<HFONT>( SelectObject( hdc, hFont ) );

	// 文字コード取得
	UINT code = 0;

	// 2バイト文字のコードは[先導コード]*256 + [文字コード]
	if( IsDBCSLeadByte( charactor[0] ) )
	{
		code = charactor[0] * 256 | static_cast<BYTE>( charactor[1] );
	}
	else
	{
		code = charactor[0];
	}

	// フォントビットマップ取得
	// テキストメトリックス取得
	TEXTMETRICA textMetrix;
	GetTextMetricsA( hdc, &textMetrix );

	// グリフ(文字の視覚的な表現)を取得
	GLYPHMETRICS glyphMetrix;
	const MAT2 identityMatrix = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };	// 文字の変換行列(単位行列)

	// まずは配列作成のための大きさを取得する
	DWORD size = GetGlyphOutlineA( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, 0, nullptr, &identityMatrix );
	std::shared_ptr<BYTE> spbitmapInfo( AKINEW BYTE[size] );

	// グリフ取得
	GetGlyphOutlineA( hdc, code, GGO_GRAY4_BITMAP, &glyphMetrix, size, spbitmapInfo.get(), &identityMatrix );

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( nullptr, hdc );

	// テクスチャ作成
	SPCHARDATA	spTemp( AKINEW CHARACTORDATA() );

	D3D11_TEXTURE2D_DESC texture2Desc;
	ZeroMemory( &texture2Desc, sizeof( texture2Desc ) );
	texture2Desc.Width				= glyphMetrix.gmCellIncX;
	texture2Desc.Height				= textMetrix.tmHeight;
	texture2Desc.MipLevels			= 1;
	texture2Desc.ArraySize			= 1;
	texture2Desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2Desc.SampleDesc.Count	= 1;
	texture2Desc.Usage				= D3D11_USAGE_DYNAMIC;
	texture2Desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texture2Desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	// 幅高さを保存
	spTemp->Size.x = static_cast<float>( texture2Desc.Width );
	spTemp->Size.y = static_cast<float>( texture2Desc.Height );

	// テクスチャ作成
	if( FAILED( lpdevice->CreateTexture2D( &texture2Desc, 0, &spTemp->lpTexture ) ) )
	{
		ErrorMessageBox( "フォントテクスチャ作成失敗" );
	}

	// リソースビュー作成
	if( FAILED( lpdevice->CreateShaderResourceView( spTemp->lpTexture, nullptr, &spTemp->lpResourceView ) ) )
	{
		ErrorMessageBox( "フォントリソースビュー作成失敗" );
	}

	// フォント情報の書き込み
	// 書き出し位置(左上)
	int offsetX = glyphMetrix.gmptGlyphOrigin.x;
	int offsetY = textMetrix.tmAscent - glyphMetrix.gmptGlyphOrigin.y;
	
	// フォントビットマップの幅高
	int bitmapWidth = glyphMetrix.gmBlackBoxX + ( 4 - ( glyphMetrix.gmBlackBoxX % 4 ) ) % 4;	// 4バイト境界のため
	int bitmapHeight = glyphMetrix.gmBlackBoxY;

	// アルファ値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int Level = 17;
	DWORD alpha, color;

	// テクスチャに書き込み
	D3D11_MAPPED_SUBRESOURCE mapResource;
	lpcontext->Map( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ), D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

	// 0クリアしておく
	ZeroMemory( mapResource.pData, mapResource.RowPitch * textMetrix.tmHeight );

	// テクスチャに文字情報を白色で書き込んでいく
	for( int y = offsetY; y < offsetY + bitmapHeight; y++ )
	{
		for( int x = offsetX; x < offsetX + bitmapWidth; x++ )
		{
			alpha = ( 0xff * spbitmapInfo.get()[x - offsetX + bitmapWidth * ( y - offsetY )] ) / ( Level - 1 );
			color = 0x00ffffff | ( alpha << 24 );// ARGBの順番

			memcpy( static_cast<BYTE*>(mapResource.pData) + mapResource.RowPitch * y + sizeof( DWORD ) * x, &color, sizeof( DWORD ) );
		}
	}

	lpcontext->Unmap( spTemp->lpTexture, D3D11CalcSubresource( 0, 0, 1 ) );

	return spTemp;
}