//==============================================================================
/// @file	D3D11Manager.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CD3D11Manager::CD3D11Manager():
	m_lpDevice			( nullptr ),
	m_lpDeviceContext	( nullptr ),
	m_lpSwapChain		( nullptr ),
	m_lpBackBufferView	( nullptr ),
	m_lpDepthStencilView( nullptr ),
	m_lpDepthStencil	( nullptr )
{
}

// デストラクタ
CD3D11Manager::~CD3D11Manager()
{
	Release();
}

// 初期化
bool CD3D11Manager::Init(HWND hwnd, UINT width, UINT height, bool iswindow)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd = { 0 };

	sd.BufferCount							= 1;								// バックバッファ数
	sd.BufferDesc.Width						= width;							// バックバッファの幅
	sd.BufferDesc.Height					= height;							// バックバッファの高さ
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;		// バックバッファのフォーマット
	sd.BufferDesc.RefreshRate.Numerator		= 60;								// リフレッシュレート分子
	sd.BufferDesc.RefreshRate.Denominator	= 1;								// リフレッシュレート分母
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファ使用方法
	sd.OutputWindow							= hwnd;								// 描画するウインドウ
	sd.SampleDesc.Count						= 1;								// ピクセル単位のマルチサンプリング数
	sd.SampleDesc.Quality					= 0;								// 品質レベル
	sd.Windowed								= iswindow;							// ウィンドウモード（FALSEでフルスクリーンモード）

	// グラフィックボード機能レベル列挙
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL* lpfeatureLevel = nullptr;

	// デバイスとスワップチェイン作成
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,															// ビデオアダプター
		D3D_DRIVER_TYPE_HARDWARE,										// 作成デバイスタイプ
		nullptr,															// ソフトウェアラスタライズDLL
		0,																// フラグ
		featureLevels,													// 機能レベル
		sizeof( featureLevels ) / sizeof( featureLevels[0] ),			// 配列の要素数
		D3D11_SDK_VERSION,												// SDKバージョン
		&sd,															// スワップチェイン内容
		reinterpret_cast<IDXGISwapChain**>( &m_lpSwapChain ),			// スワップチェインを受け取るポインタ
		reinterpret_cast<ID3D11Device**>( &m_lpDevice ),				// デバイスを受け取るポインタ
		lpfeatureLevel,													// 機能レベルを受け取る
		reinterpret_cast<ID3D11DeviceContext**>( &m_lpDeviceContext ) );// デバイスコンテキストを受け取るポインタ
	try
	{
		if( FAILED( hr ) )
		{
			throw "このPCはハードウェアレベルでのDirectX11に対応していません";	
		}

		// バックバッファビューの作成
		ID3D11Texture2D *lpBackBuffer;
		m_lpSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<LPVOID*>( &lpBackBuffer ) );    
		m_lpDevice->CreateRenderTargetView( lpBackBuffer, nullptr, &m_lpBackBufferView );
		SafeRelease( lpBackBuffer );

		// 深度ステンシルビューの作成
		D3D11_TEXTURE2D_DESC descDepth;

		descDepth.Width					= width;					// 深度バッファの幅
		descDepth.Height				= height;					// 深度バッファの高さ
		descDepth.MipLevels				= 1;						// ミニマップレベル
		descDepth.ArraySize				= 1;						// テクスチャ配列数
		descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// フォーマット
		descDepth.SampleDesc.Count		= 1;						// 数
		descDepth.SampleDesc.Quality	= 0;						// 品質
		descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法
		descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// バインドフラグ
		descDepth.CPUAccessFlags		= 0;						// CPUアクセスフラグ(0で無効)
		descDepth.MiscFlags				= 0;						// リソース識別フラグ

		// 深度ステンシルバッファ作成
		m_lpDevice->CreateTexture2D( &descDepth, nullptr, &m_lpDepthStencil );

		// 深度ステンシルバッファへのビューを作成
		m_lpDevice->CreateDepthStencilView( m_lpDepthStencil, nullptr, &m_lpDepthStencilView );

		// バックバッファビューと深度ステンシルビューをパイプラインにセット
		m_lpDeviceContext->OMSetRenderTargets( 1, &m_lpBackBufferView, m_lpDepthStencilView );

		//ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0;			// X座標
		vp.TopLeftY = 0;			// Y座標
		vp.Width	= width * 1.f;	// 幅
		vp.Height	= height* 1.f;	// 高さ
		vp.MinDepth = 0.0f;			// 最少深度
		vp.MaxDepth = 1.0f;			// 最大深度
		m_lpDeviceContext->RSSetViewports( 1, &vp );

		// ラスタライズ設定
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory( &rdc, sizeof( rdc ) );

		rdc.FillMode				= D3D11_FILL_SOLID;	// 三角形を塗りつぶす
		rdc.CullMode				= D3D11_CULL_NONE;	// カリングしない
		rdc.FrontCounterClockwise	= false;			// 反時計回りならば後ろ向きと判断
		rdc.DepthBias				= 0;				// 指定ピクセルに対しての深度加算値
		rdc.DepthBiasClamp			= 0;				// ピクセルに対しての最大バイアス値
		rdc.SlopeScaledDepthBias	= 0;				// 指定ピクセルに対してのスカラ値
		rdc.DepthClipEnable			= false;			// 深度によってのクリッピング無効
		rdc.ScissorEnable			= false;			// シザー矩形のカリング無効
		rdc.MultisampleEnable		= false;			// マルチサンプリング無効
		rdc.AntialiasedLineEnable	= false;			// アンチエイリアス無効
		
		ID3D11RasterizerState* pIr = nullptr;
		m_lpDevice->CreateRasterizerState( &rdc, &pIr );
		m_lpDeviceContext->RSSetState( pIr );
		SafeRelease( pIr );

		// 0番目のみアルファブレンド有効
		D3D11_BLEND_DESC1 blendDesc = { 0 };
		blendDesc.RenderTarget[0].BlendEnable	= true;
		blendDesc.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ID3D11BlendState1* tempBlend;
		m_lpDevice->CreateBlendState1( &blendDesc, &tempBlend );

		m_lpDeviceContext->OMSetBlendState( tempBlend, nullptr, 0xffffffff );

		tempBlend->Release();
	}
	catch( LPCSTR& lperror )
	{
		ErrorMessageBox( lperror );
	}
	catch( ... )
	{
		ErrorMessageBox( "DirectX初期化時に原因不明のエラーが発生しました" );
	}

	return true;
}

// 解放
void CD3D11Manager::Release()
{
	SafeRelease( m_lpDepthStencil );
	SafeRelease( m_lpDepthStencilView );
	SafeRelease( m_lpBackBufferView	);
	SafeRelease( m_lpSwapChain );
	SafeRelease( m_lpDeviceContext );
	SafeRelease( m_lpDevice	);
}

// バッファクリア
void CD3D11Manager::ClearPrimaryBuffer( float r, float g, float b, float a )
{
	float clearColor[4] = { r, g, b, a };

	// 画面クリア
	m_lpDeviceContext->ClearRenderTargetView( m_lpBackBufferView, clearColor );//画面クリア

	// 深度ステンシルバッファクリア
	m_lpDeviceContext->ClearDepthStencilView( m_lpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

// 指定レンダーターゲットクリア
void CD3D11Manager::ClearRenderTarget( ID3D11RenderTargetView* lprenderTarget, float r, float g, float b, float a )
{
	float clearColor[4] = { r, g, b, a };

	// 画面クリア
	m_lpDeviceContext->ClearRenderTargetView( lprenderTarget, clearColor );//画面クリア
}

// 指定深度ステンシルクリア
void CD3D11Manager::ClearDepthStencil( ID3D11DepthStencilView* lpdepthStencil )
{
	m_lpDeviceContext->ClearDepthStencilView( lpdepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

// 画面更新
void CD3D11Manager::Flip()
{
	m_lpSwapChain->Present( 0, 0 );
}

// レンダーターゲットを戻す
void CD3D11Manager::ResetRenderTarget()
{
	ID3D11RenderTargetView* lprenderTargetViews[8] = { nullptr };

	lprenderTargetViews[0] = m_lpBackBufferView;
	m_lpDeviceContext->OMSetRenderTargets( 8, lprenderTargetViews, m_lpDepthStencilView );
}

// レンダ－ターゲット設定
void CD3D11Manager::SetRenderTargets( 
			ID3D11DepthStencilView* lpdepthStencil,
			ID3D11RenderTargetView* lprenderTarget0,
			ID3D11RenderTargetView* lprenderTarget1,
			ID3D11RenderTargetView* lprenderTarget2,
			ID3D11RenderTargetView* lprenderTarget3,
			ID3D11RenderTargetView* lprenderTarget4,
			ID3D11RenderTargetView* lprenderTarget5,
			ID3D11RenderTargetView* lprenderTarget6,
			ID3D11RenderTargetView* lprenderTarget7)
{
	ID3D11RenderTargetView* lprenderTargetViews[8] =
	{
		lprenderTarget0,
		lprenderTarget1,
		lprenderTarget2,
		lprenderTarget3,
		lprenderTarget4,
		lprenderTarget5,
		lprenderTarget6,
		lprenderTarget7
	};

	m_lpDeviceContext->OMSetRenderTargets( 8, lprenderTargetViews, lpdepthStencil );
}

// 頂点バッファ作成
ID3D11Buffer* CD3D11Manager::CreateVertexBuffer(void* vertexData, size_t sizeVertex)
{
	ID3D11Buffer* lptempBufffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= sizeVertex;
	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = vertexData;

	if( FAILED( m_lpDevice->CreateBuffer( &bufferDesc, &initData, &lptempBufffer ) ) )
	{
		ErrorMessageBox( "頂点バッファ作成失敗" );
		return nullptr;
	}

	return lptempBufffer;
}

// コンスタントバッファ作成
ID3D11Buffer* CD3D11Manager::CreateConstantBuffer(size_t sizeBuffer)
{
	ID3D11Buffer* lptempBufffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth		= sizeBuffer;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;

	if( FAILED( m_lpDevice->CreateBuffer( &bufferDesc, nullptr, &lptempBufffer ) ) )
	{
		ErrorMessageBox( "コンスタントバッファ作成失敗" );
		return nullptr;
	}

	return lptempBufffer;
}

// 構造体バッファ作成
ID3D11Buffer* CD3D11Manager::CreateStructuredBuffer(size_t sizeBuffer, size_t stride, void* lpdata)
{
	ID3D11Buffer* lptempBufffer = nullptr;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	D3D11_SUBRESOURCE_DATA initData = { 0 };

	bufferDesc.ByteWidth			= sizeBuffer;
	bufferDesc.StructureByteStride	= stride;
	bufferDesc.BindFlags			= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	initData.pSysMem				= lpdata;

	if( FAILED( m_lpDevice->CreateBuffer( &bufferDesc, (lpdata)? &initData : nullptr, &lptempBufffer ) ) )
	{
		ErrorMessageBox( "構造体バッファ作成失敗" );
		return nullptr;
	}

	return lptempBufffer;
}

// コンピュートシェーダに値を送るためのビューを作成
ID3D11ShaderResourceView* CD3D11Manager::CreateComputeSRV( ID3D11Buffer* lpstructuredBuffer, UINT numElement )
{
	ID3D11ShaderResourceView* lptempView = nullptr;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );

	SRVDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.Format				= DXGI_FORMAT_UNKNOWN;
	SRVDesc.Buffer.NumElements	= numElement;

	if( FAILED( m_lpDevice->CreateShaderResourceView( lpstructuredBuffer, &SRVDesc, &lptempView ) ) )
	{
		ErrorMessageBox( "アンオーダードアクセスビュー作成失敗" );
		return nullptr;
	}

	return lptempView;
}


// アンオーダードアクセスビュー作成
ID3D11UnorderedAccessView* CD3D11Manager::CreateUAV( ID3D11Buffer* lpstructuredBuffer, UINT numElement )
{
	ID3D11UnorderedAccessView* lptempView = nullptr;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory( &UAVDesc, sizeof( UAVDesc ) );

	UAVDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Format				= DXGI_FORMAT_UNKNOWN;
	UAVDesc.Buffer.NumElements	= numElement;

	if( FAILED( m_lpDevice->CreateUnorderedAccessView( lpstructuredBuffer, &UAVDesc, &lptempView ) ) )
	{
		ErrorMessageBox( "アンオーダードアクセスビュー作成失敗" );
		return nullptr;
	}

	return lptempView;
}

// コンピュートシェーダの結果を受け取る
void CD3D11Manager::GetComputShaderResult(ID3D11Buffer* lpstructuredBuffer, void* lpreseveBuffer, size_t size)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	ID3D11Buffer* lptempBuffer = nullptr;

	lpstructuredBuffer->GetDesc( &bufferDesc );

	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufferDesc.Usage = D3D11_USAGE_STAGING;

	if( FAILED( m_lpDevice->CreateBuffer( &bufferDesc, nullptr, &lptempBuffer ) ) )
	{
		ErrorMessageBox( "コピー用バッファ作成失敗" );
		return;
	}

	// 結果をコピーする
	m_lpDeviceContext->CopyResource( lptempBuffer, lpstructuredBuffer );

	D3D11_MAPPED_SUBRESOURCE subResource;
	if( FAILED( m_lpDeviceContext->Map( lptempBuffer, 0, D3D11_MAP_READ, 0, &subResource ) ) )
	{
		ErrorMessageBox( "コンピュートシェーダの結果取得失敗" );
		return;
	}

	// 結果をコピーする
	memcpy_s( lpreseveBuffer, min( size, subResource.RowPitch ), subResource.pData, subResource.RowPitch );	

	m_lpDeviceContext->Unmap( lptempBuffer, 0 );
	SafeRelease( lptempBuffer );
}

// シェーダに値を渡す
bool CD3D11Manager::SendValueToShader( UINT slot, ID3D11Buffer* buffer, const void* lpdata )
{
	D3D11_MAPPED_SUBRESOURCE resource;

	if( FAILED( m_lpDeviceContext->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource ) ) )
	{
		ErrorMessageBox( "マッピング失敗" );
		return false;
	}
	
	memcpy_s( resource.pData, resource.RowPitch, lpdata, resource.RowPitch );
	m_lpDeviceContext->Unmap( buffer, 0 );

	m_lpDeviceContext->VSSetConstantBuffers( slot, 1, &buffer );
	m_lpDeviceContext->PSSetConstantBuffers( slot, 1, &buffer );
	m_lpDeviceContext->GSSetConstantBuffers( slot, 1, &buffer );
	m_lpDeviceContext->HSSetConstantBuffers( slot, 1, &buffer );
	m_lpDeviceContext->DSSetConstantBuffers( slot, 1, &buffer );
	m_lpDeviceContext->CSSetConstantBuffers( slot, 1, &buffer );

	return true;
}