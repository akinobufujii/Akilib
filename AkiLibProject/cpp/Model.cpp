//==============================================================================
/// @file	MQOFile.cpp
/// @brief	モデル基底クラスの実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

//==============================================================================
// 静的メンバ実体化
//==============================================================================
const D3D11_INPUT_ELEMENT_DESC	CModel::VERTEX_LAYOUT[] = 
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// コンストラクタ
CModel::CModel()
{
}

// デストラクタ
CModel::~CModel()
{
	for( UINT i = 0; i < m_BufferArray.size(); ++i )
	{
		SafeRelease( m_BufferArray[i].lpIndex );
		SafeRelease( m_BufferArray[i].lpVertex );
	}

	for( UINT i = 0; i < m_MaterialArray.size(); ++i )
	{
		SafeDelete(m_MaterialArray[i].lpTexture);
	}
}

// バッファ作成
void CModel::CreateMeshBuffer(ID3D11Device1* lpdevice)
{
	D3D11_BUFFER_DESC		bufferDesc = { 0 };
	D3D11_SUBRESOURCE_DATA	bufferData = { 0 };
	std::vector<UINT>		tempIndexArray;
  
	// バッファ配列確保
	m_BufferArray.resize( m_MeshObjectMap.size() );

	// オブジェクトごとに作成
	UINT count = 0;
	MESHOBJECTMAP::iterator it = m_MeshObjectMap.begin();
	while( it != m_MeshObjectMap.end() )
	{
		// 読み込んだインデックスを分割
		for( UINT i = 0; i < it->second.Face.size(); ++i )
		{
			// もし3つ以上ならば分割する
			if( it->second.Face[i].IndexArray.size() > 3 )
			{
				// トライアングルファン(扇形)につなぐ
				for( UINT j = 0; j < it->second.Face[i].IndexArray.size() - 2; ++j )
				{
					tempIndexArray.push_back( it->second.Face[i].IndexArray[0] );
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j + 1] );
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j + 2] );
				}
			}
			// そうでなければ順番に詰め込む
			else
			{
				for( UINT j = 0; j < it->second.Face[i].IndexArray.size(); ++j )
				{
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j] );
				}
			}
		}

		// インデックスバッファ作成
		m_BufferArray[count].NumIndex	= tempIndexArray.size();
		bufferDesc.Usage				= D3D11_USAGE_DEFAULT;				// アクセス権限
		bufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;			// バッファの種類
		bufferDesc.ByteWidth			= m_BufferArray[count].NumIndex * sizeof( UINT );
		bufferData.pSysMem				= &tempIndexArray.front();

		if( FAILED( lpdevice->CreateBuffer( &bufferDesc, &bufferData, &m_BufferArray[count].lpIndex ) ) )
		{
			ErrorMessageBox( "メッシュのインデックスバッファ作成失敗" );
		}

		// 頂点情報格納
		// 頂点位置とテクスチャ座標はすでに格納しているので他の情報を格納していく
		Vector3 temp;
		for( UINT i = 0; i < tempIndexArray.size(); i += 3 )
		{
			// 法線情報を計算して格納
			// 計算方式は、面の法線を外積で割り出しハーフベクトルで中間の法線を割り出す
			// まずは外積を使って面の法線を出す
			temp.SetCross( 
				it->second.Vertex[tempIndexArray[i]].Pos - it->second.Vertex[tempIndexArray[i + 1]].Pos,
				it->second.Vertex[tempIndexArray[i]].Pos - it->second.Vertex[tempIndexArray[i + 2]].Pos );

			// 計算した法線を足し込み、正規化してハーフベクトルにする
			it->second.Vertex[tempIndexArray[i]].Normal += temp.Normalize();
			it->second.Vertex[tempIndexArray[i + 1]].Normal += temp.Normalize();
			it->second.Vertex[tempIndexArray[i + 2]].Normal += temp.Normalize();
			
			it->second.Vertex[tempIndexArray[i]].Normal.Normalize();
			it->second.Vertex[tempIndexArray[i + 1]].Normal.Normalize();
			it->second.Vertex[tempIndexArray[i + 2]].Normal.Normalize();
		}

		// 頂点バッファ作成(書き換え可能なようにしておく)
		bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;				// アクセス権限
		bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;			// 書き換え可能
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;			// バッファの種類
		bufferDesc.ByteWidth		= it->second.Vertex.size() * sizeof( MESHVERTEX );
		bufferData.pSysMem			= &it->second.Vertex.front();

		if( FAILED( lpdevice->CreateBuffer( &bufferDesc, &bufferData, &m_BufferArray[count++].lpVertex ) ) )
		{
			ErrorMessageBox( "メッシュの頂点バッファ作成失敗" );
		}

		++it;
	}

	// サンプラステート作成
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
}

// 描画
void CModel::Draw( ID3D11DeviceContext1* lpdeviceContext, UINT slot )
{
	lpdeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// テクスチャセット
	//m_MaterialArray[0].lpTexture->SetTexture(lpdeviceContext, slot);

	// 描画
	for( UINT i = 0; i < m_BufferArray.size(); ++i )
	{
		lpdeviceContext->IASetVertexBuffers( 
			0,
			1,
			&m_BufferArray[i].lpVertex,
			AkiLib::ConvertValue( sizeof( MESHVERTEX ) ), 
			AkiLib::ConvertValue<UINT>( 0 ) );

		lpdeviceContext->IASetIndexBuffer( m_BufferArray[i].lpIndex, DXGI_FORMAT_R32_UINT, 0 );

		lpdeviceContext->DrawIndexed( m_BufferArray[i].NumIndex, 0, 0 );
	}
}