//==============================================================================
/// @file	Board.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

//==============================================================================
// 静的メンバ実体化
//==============================================================================
const D3D11_INPUT_ELEMENT_DESC CBoard::VERTEX_LAYOUT[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const BYTE CBoard::NUM_VERTEX = 4;
const BYTE CBoard::NUM_VERTEXELEMENT = _countof( CBoard::VERTEX_LAYOUT );

// コンストラクタ
CBoard::CBoard()
{
	m_Vertex[0].Pos = XMFLOAT3( -1,  1, 0 );
	m_Vertex[1].Pos = XMFLOAT3(  1,  1, 0 );
	m_Vertex[2].Pos = XMFLOAT3( -1, -1, 0 );
	m_Vertex[3].Pos = XMFLOAT3(  1, -1, 0 );

	m_Vertex[0].Color = m_Vertex[1].Color = m_Vertex[2].Color = m_Vertex[3].Color = XMFLOAT4( 1, 1, 1, 1 );

	m_Vertex[0].UV = XMFLOAT2( 0, 0 );
	m_Vertex[1].UV = XMFLOAT2( 1, 0 );
	m_Vertex[2].UV = XMFLOAT2( 0, 1 );
	m_Vertex[3].UV = XMFLOAT2( 1, 1 );
}

// デストラクタ
CBoard::~CBoard()
{
	Release();
}

// 初期化
bool CBoard::Init(ID3D11Device1* lpdevice)
{
	// もし頂点バッファがなければ作成する
	if( m_lpVertexBuffer == nullptr )
	{
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth		= sizeof( m_Vertex );
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.pSysMem = m_Vertex;

		if( FAILED( lpdevice->CreateBuffer( &bufferDesc, &initData, &m_lpVertexBuffer ) ) )
		{
			ErrorMessageBox( "頂点バッファ作成失敗" );
			return false;
		}
	}

	return true;
}

// 解放
void CBoard::Release()
{
	SafeRelease( m_lpVertexBuffer );
}

// 描画
void CBoard::Draw(ID3D11DeviceContext1* lpdeviceContext, D3D_PRIMITIVE_TOPOLOGY topology)
{
	D3D11_MAPPED_SUBRESOURCE temp;

	lpdeviceContext->Map(m_lpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &temp );
	memcpy_s( temp.pData, sizeof( m_Vertex ), m_Vertex, sizeof( m_Vertex ) );
	lpdeviceContext->Unmap( m_lpVertexBuffer, 0 );

	lpdeviceContext->IASetVertexBuffers( 
		0,
		1,
		&m_lpVertexBuffer,
		AkiLib::ConvertValue( sizeof( VERTEX ) ), 
		AkiLib::ConvertValue<UINT>( 0 ) );

	lpdeviceContext->IASetPrimitiveTopology( topology );
	lpdeviceContext->Draw( NUM_VERTEX, 0 );
}