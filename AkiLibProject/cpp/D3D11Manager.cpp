//==============================================================================
/// @file	D3D11Manager.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CD3D11Manager::CD3D11Manager():
	m_lpDevice			( nullptr ),
	m_lpDeviceContext	( nullptr ),
	m_lpSwapChain		( nullptr ),
	m_lpBackBufferView	( nullptr ),
	m_lpDepthStencilView( nullptr ),
	m_lpDepthStencil	( nullptr )
{
}

// �f�X�g���N�^
CD3D11Manager::~CD3D11Manager()
{
	Release();
}

// ������
bool CD3D11Manager::Init(HWND hwnd, UINT width, UINT height, bool iswindow)
{
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd = { 0 };

	sd.BufferCount							= 1;								// �o�b�N�o�b�t�@��
	sd.BufferDesc.Width						= width;							// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height					= height;							// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator		= 60;								// ���t���b�V�����[�g���q
	sd.BufferDesc.RefreshRate.Denominator	= 1;								// ���t���b�V�����[�g����
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�g�p���@
	sd.OutputWindow							= hwnd;								// �`�悷��E�C���h�E
	sd.SampleDesc.Count						= 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
	sd.SampleDesc.Quality					= 0;								// �i�����x��
	sd.Windowed								= iswindow;							// �E�B���h�E���[�h�iFALSE�Ńt���X�N���[�����[�h�j

	// �O���t�B�b�N�{�[�h�@�\���x����
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

	// �f�o�C�X�ƃX���b�v�`�F�C���쐬
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,															// �r�f�I�A�_�v�^�[
		D3D_DRIVER_TYPE_HARDWARE,										// �쐬�f�o�C�X�^�C�v
		nullptr,															// �\�t�g�E�F�A���X�^���C�YDLL
		0,																// �t���O
		featureLevels,													// �@�\���x��
		sizeof( featureLevels ) / sizeof( featureLevels[0] ),			// �z��̗v�f��
		D3D11_SDK_VERSION,												// SDK�o�[�W����
		&sd,															// �X���b�v�`�F�C�����e
		reinterpret_cast<IDXGISwapChain**>( &m_lpSwapChain ),			// �X���b�v�`�F�C�����󂯎��|�C���^
		reinterpret_cast<ID3D11Device**>( &m_lpDevice ),				// �f�o�C�X���󂯎��|�C���^
		lpfeatureLevel,													// �@�\���x�����󂯎��
		reinterpret_cast<ID3D11DeviceContext**>( &m_lpDeviceContext ) );// �f�o�C�X�R���e�L�X�g���󂯎��|�C���^
	try
	{
		if( FAILED( hr ) )
		{
			throw "����PC�̓n�[�h�E�F�A���x���ł�DirectX11�ɑΉ����Ă��܂���";	
		}

		// �o�b�N�o�b�t�@�r���[�̍쐬
		ID3D11Texture2D *lpBackBuffer;
		m_lpSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<LPVOID*>( &lpBackBuffer ) );    
		m_lpDevice->CreateRenderTargetView( lpBackBuffer, nullptr, &m_lpBackBufferView );
		SafeRelease( lpBackBuffer );

		// �[�x�X�e���V���r���[�̍쐬
		D3D11_TEXTURE2D_DESC descDepth;

		descDepth.Width					= width;					// �[�x�o�b�t�@�̕�
		descDepth.Height				= height;					// �[�x�o�b�t�@�̍���
		descDepth.MipLevels				= 1;						// �~�j�}�b�v���x��
		descDepth.ArraySize				= 1;						// �e�N�X�`���z��
		descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// �t�H�[�}�b�g
		descDepth.SampleDesc.Count		= 1;						// ��
		descDepth.SampleDesc.Quality	= 0;						// �i��
		descDepth.Usage					= D3D11_USAGE_DEFAULT;		// �g�p���@
		descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// �o�C���h�t���O
		descDepth.CPUAccessFlags		= 0;						// CPU�A�N�Z�X�t���O(0�Ŗ���)
		descDepth.MiscFlags				= 0;						// ���\�[�X���ʃt���O

		// �[�x�X�e���V���o�b�t�@�쐬
		m_lpDevice->CreateTexture2D( &descDepth, nullptr, &m_lpDepthStencil );

		// �[�x�X�e���V���o�b�t�@�ւ̃r���[���쐬
		m_lpDevice->CreateDepthStencilView( m_lpDepthStencil, nullptr, &m_lpDepthStencilView );

		// �o�b�N�o�b�t�@�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���ɃZ�b�g
		m_lpDeviceContext->OMSetRenderTargets( 1, &m_lpBackBufferView, m_lpDepthStencilView );

		//�r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0;			// X���W
		vp.TopLeftY = 0;			// Y���W
		vp.Width	= width * 1.f;	// ��
		vp.Height	= height* 1.f;	// ����
		vp.MinDepth = 0.0f;			// �ŏ��[�x
		vp.MaxDepth = 1.0f;			// �ő�[�x
		m_lpDeviceContext->RSSetViewports( 1, &vp );

		// ���X�^���C�Y�ݒ�
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory( &rdc, sizeof( rdc ) );

		rdc.FillMode				= D3D11_FILL_SOLID;	// �O�p�`��h��Ԃ�
		rdc.CullMode				= D3D11_CULL_NONE;	// �J�����O���Ȃ�
		rdc.FrontCounterClockwise	= false;			// �����v���Ȃ�Ό������Ɣ��f
		rdc.DepthBias				= 0;				// �w��s�N�Z���ɑ΂��Ă̐[�x���Z�l
		rdc.DepthBiasClamp			= 0;				// �s�N�Z���ɑ΂��Ă̍ő�o�C�A�X�l
		rdc.SlopeScaledDepthBias	= 0;				// �w��s�N�Z���ɑ΂��ẴX�J���l
		rdc.DepthClipEnable			= false;			// �[�x�ɂ���ẴN���b�s���O����
		rdc.ScissorEnable			= false;			// �V�U�[��`�̃J�����O����
		rdc.MultisampleEnable		= false;			// �}���`�T���v�����O����
		rdc.AntialiasedLineEnable	= false;			// �A���`�G�C���A�X����
		
		ID3D11RasterizerState* pIr = nullptr;
		m_lpDevice->CreateRasterizerState( &rdc, &pIr );
		m_lpDeviceContext->RSSetState( pIr );
		SafeRelease( pIr );

		// 0�Ԗڂ̂݃A���t�@�u�����h�L��
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
		ErrorMessageBox( "DirectX���������Ɍ����s���̃G���[���������܂���" );
	}

	return true;
}

// ���
void CD3D11Manager::Release()
{
	SafeRelease( m_lpDepthStencil );
	SafeRelease( m_lpDepthStencilView );
	SafeRelease( m_lpBackBufferView	);
	SafeRelease( m_lpSwapChain );
	SafeRelease( m_lpDeviceContext );
	SafeRelease( m_lpDevice	);
}

// �o�b�t�@�N���A
void CD3D11Manager::ClearPrimaryBuffer( float r, float g, float b, float a )
{
	float clearColor[4] = { r, g, b, a };

	// ��ʃN���A
	m_lpDeviceContext->ClearRenderTargetView( m_lpBackBufferView, clearColor );//��ʃN���A

	// �[�x�X�e���V���o�b�t�@�N���A
	m_lpDeviceContext->ClearDepthStencilView( m_lpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

// �w�背���_�[�^�[�Q�b�g�N���A
void CD3D11Manager::ClearRenderTarget( ID3D11RenderTargetView* lprenderTarget, float r, float g, float b, float a )
{
	float clearColor[4] = { r, g, b, a };

	// ��ʃN���A
	m_lpDeviceContext->ClearRenderTargetView( lprenderTarget, clearColor );//��ʃN���A
}

// �w��[�x�X�e���V���N���A
void CD3D11Manager::ClearDepthStencil( ID3D11DepthStencilView* lpdepthStencil )
{
	m_lpDeviceContext->ClearDepthStencilView( lpdepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

// ��ʍX�V
void CD3D11Manager::Flip()
{
	m_lpSwapChain->Present( 0, 0 );
}

// �����_�[�^�[�Q�b�g��߂�
void CD3D11Manager::ResetRenderTarget()
{
	ID3D11RenderTargetView* lprenderTargetViews[8] = { nullptr };

	lprenderTargetViews[0] = m_lpBackBufferView;
	m_lpDeviceContext->OMSetRenderTargets( 8, lprenderTargetViews, m_lpDepthStencilView );
}

// �����_�|�^�[�Q�b�g�ݒ�
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

// ���_�o�b�t�@�쐬
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
		ErrorMessageBox( "���_�o�b�t�@�쐬���s" );
		return nullptr;
	}

	return lptempBufffer;
}

// �R���X�^���g�o�b�t�@�쐬
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
		ErrorMessageBox( "�R���X�^���g�o�b�t�@�쐬���s" );
		return nullptr;
	}

	return lptempBufffer;
}

// �\���̃o�b�t�@�쐬
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
		ErrorMessageBox( "�\���̃o�b�t�@�쐬���s" );
		return nullptr;
	}

	return lptempBufffer;
}

// �R���s���[�g�V�F�[�_�ɒl�𑗂邽�߂̃r���[���쐬
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
		ErrorMessageBox( "�A���I�[�_�[�h�A�N�Z�X�r���[�쐬���s" );
		return nullptr;
	}

	return lptempView;
}


// �A���I�[�_�[�h�A�N�Z�X�r���[�쐬
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
		ErrorMessageBox( "�A���I�[�_�[�h�A�N�Z�X�r���[�쐬���s" );
		return nullptr;
	}

	return lptempView;
}

// �R���s���[�g�V�F�[�_�̌��ʂ��󂯎��
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
		ErrorMessageBox( "�R�s�[�p�o�b�t�@�쐬���s" );
		return;
	}

	// ���ʂ��R�s�[����
	m_lpDeviceContext->CopyResource( lptempBuffer, lpstructuredBuffer );

	D3D11_MAPPED_SUBRESOURCE subResource;
	if( FAILED( m_lpDeviceContext->Map( lptempBuffer, 0, D3D11_MAP_READ, 0, &subResource ) ) )
	{
		ErrorMessageBox( "�R���s���[�g�V�F�[�_�̌��ʎ擾���s" );
		return;
	}

	// ���ʂ��R�s�[����
	memcpy_s( lpreseveBuffer, min( size, subResource.RowPitch ), subResource.pData, subResource.RowPitch );	

	m_lpDeviceContext->Unmap( lptempBuffer, 0 );
	SafeRelease( lptempBuffer );
}

// �V�F�[�_�ɒl��n��
bool CD3D11Manager::SendValueToShader( UINT slot, ID3D11Buffer* buffer, const void* lpdata )
{
	D3D11_MAPPED_SUBRESOURCE resource;

	if( FAILED( m_lpDeviceContext->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource ) ) )
	{
		ErrorMessageBox( "�}�b�s���O���s" );
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