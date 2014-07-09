//==============================================================================
/// @file	Texture.h
/// @brief	テクスチャ関連実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CTexture::CTexture():
	m_lpTextureView		( nullptr ),
	m_lpTextureResource	( nullptr ),
	m_lpSamplerState	( nullptr ),
	m_lpTextuer			( nullptr ),
	m_lpRenderTargetView( nullptr )
{
}

// デストラクタ
CTexture::~CTexture()
{
	SafeRelease( m_lpRenderTargetView );
	SafeRelease( m_lpTextuer );
	SafeRelease( m_lpSamplerState );
	SafeRelease( m_lpTextureView );
	SafeRelease( m_lpTextureResource );
}

// テクスチャ作成(char)
bool CTexture::CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCSTR						filename,
			D3D11_FILTER				filter,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode )
{
	// テクスチャ読み込み
	wchar_t unicode[0xff];

	MultiByteToWideChar( 
		CP_ACP,
		MB_PRECOMPOSED,
		filename,
		strlen(filename) + 1,
		unicode,
		strlen(filename) + 1 );

	return CreateTextureFromFile(lpdevice, lpcontext, unicode, filter, addressMode);
}

// テクスチャ作成(wchar)
bool CTexture::CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCWSTR						filename,
			D3D11_FILTER				filter,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode )
{
	// サンプラステート作成
	D3D11_SAMPLER_DESC sampDesc;
	
	ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );

	sampDesc.Filter = filter;
	sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = addressMode;

	lpdevice->CreateSamplerState( &sampDesc, &m_lpSamplerState );

	// テクスチャ読み込み
	HRESULT hr;
	if( wcsstr(filename, L"dds") || wcsstr(filename, L"DDS") )
	{
		hr = DirectX::CreateDDSTextureFromFile( lpdevice, filename, &m_lpTextureResource, &m_lpTextureView );
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(lpdevice, lpcontext, filename, &m_lpTextureResource, &m_lpTextureView);
	}

	m_lpTextuer = static_cast<ID3D11Texture2D*>(m_lpTextureResource);
	m_lpTextuer->GetDesc(&m_Texture2DDesc);

	return (FAILED(hr))? false : true;
}

// レンダーターゲット作成
bool CTexture::CreateRenderTarget(
			ID3D11Device1*				lpdevice,
			UINT						width,
			UINT						height,
			DXGI_FORMAT					format,
			D3D11_FILTER				filter,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode)
{
	// サンプラステート作成
	D3D11_SAMPLER_DESC sampDesc;
	
	ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );

	sampDesc.Filter = filter;
	sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = addressMode;

	lpdevice->CreateSamplerState( &sampDesc, &m_lpSamplerState );

	// テクスチャ作成
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory( &textureDesc, sizeof( textureDesc ) );

	textureDesc.Width				= width;
	textureDesc.Height				= height;
	textureDesc.Format				= format;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if( FAILED( lpdevice->CreateTexture2D( &textureDesc, nullptr, &m_lpTextuer ) ) )
	{
		ErrorMessageBox( "テクスチャ作成失敗" );
	}

	// レンダ－ターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory( &renderTargetViewDesc, sizeof( renderTargetViewDesc ) );

	renderTargetViewDesc.Format			= textureDesc.Format;
	renderTargetViewDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;

	if( FAILED( lpdevice->CreateRenderTargetView( m_lpTextuer, &renderTargetViewDesc, &m_lpRenderTargetView ) ) )
	{
		ErrorMessageBox( "レンダーターゲットビュー作成失敗" );
	}

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC shderResourceViewDesc;
	ZeroMemory( &shderResourceViewDesc, sizeof( shderResourceViewDesc ) );

	shderResourceViewDesc.Format				= textureDesc.Format;
	shderResourceViewDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2D;
	shderResourceViewDesc.Texture2D.MipLevels	= 1;

	if( FAILED( lpdevice->CreateShaderResourceView( m_lpTextuer, &shderResourceViewDesc, &m_lpTextureView ) ) )
	{
		ErrorMessageBox( "シェーダーリソースビュー作成失敗" );
	}

	m_lpTextuer->GetDesc(&m_Texture2DDesc);

	return true;
}

// テクスチャ設定
void CTexture::SetTexture( ID3D11DeviceContext1* lpdeviceContext, UINT slot )
{
	// サンプラステート設定
	lpdeviceContext->VSSetSamplers( slot, 1, &m_lpSamplerState );
	lpdeviceContext->GSSetSamplers( slot, 1, &m_lpSamplerState );
	lpdeviceContext->HSSetSamplers( slot, 1, &m_lpSamplerState );
	lpdeviceContext->DSSetSamplers( slot, 1, &m_lpSamplerState );
	lpdeviceContext->PSSetSamplers( slot, 1, &m_lpSamplerState );
	lpdeviceContext->CSSetSamplers( slot, 1, &m_lpSamplerState );

	// テクスチャビュー設定
	lpdeviceContext->VSSetShaderResources( slot, 1, &m_lpTextureView );
	lpdeviceContext->GSSetShaderResources( slot, 1, &m_lpTextureView );
	lpdeviceContext->HSSetShaderResources( slot, 1, &m_lpTextureView );
	lpdeviceContext->DSSetShaderResources( slot, 1, &m_lpTextureView );
	lpdeviceContext->PSSetShaderResources( slot, 1, &m_lpTextureView );
	lpdeviceContext->CSSetShaderResources( slot, 1, &m_lpTextureView );
}