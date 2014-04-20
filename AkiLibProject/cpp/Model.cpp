//==============================================================================
/// @file	MQOFile.cpp
/// @brief	���f�����N���X�̎���
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

//==============================================================================
// �ÓI�����o���̉�
//==============================================================================
const D3D11_INPUT_ELEMENT_DESC	CModel::VERTEX_LAYOUT[] = 
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// �R���X�g���N�^
CModel::CModel()
{
}

// �f�X�g���N�^
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

// �o�b�t�@�쐬
void CModel::CreateMeshBuffer(ID3D11Device1* lpdevice)
{
	D3D11_BUFFER_DESC		bufferDesc = { 0 };
	D3D11_SUBRESOURCE_DATA	bufferData = { 0 };
	std::vector<UINT>		tempIndexArray;
  
	// �o�b�t�@�z��m��
	m_BufferArray.resize( m_MeshObjectMap.size() );

	// �I�u�W�F�N�g���Ƃɍ쐬
	UINT count = 0;
	MESHOBJECTMAP::iterator it = m_MeshObjectMap.begin();
	while( it != m_MeshObjectMap.end() )
	{
		// �ǂݍ��񂾃C���f�b�N�X�𕪊�
		for( UINT i = 0; i < it->second.Face.size(); ++i )
		{
			// ����3�ȏ�Ȃ�Ε�������
			if( it->second.Face[i].IndexArray.size() > 3 )
			{
				// �g���C�A���O���t�@��(��`)�ɂȂ�
				for( UINT j = 0; j < it->second.Face[i].IndexArray.size() - 2; ++j )
				{
					tempIndexArray.push_back( it->second.Face[i].IndexArray[0] );
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j + 1] );
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j + 2] );
				}
			}
			// �����łȂ���Ώ��Ԃɋl�ߍ���
			else
			{
				for( UINT j = 0; j < it->second.Face[i].IndexArray.size(); ++j )
				{
					tempIndexArray.push_back( it->second.Face[i].IndexArray[j] );
				}
			}
		}

		// �C���f�b�N�X�o�b�t�@�쐬
		m_BufferArray[count].NumIndex	= tempIndexArray.size();
		bufferDesc.Usage				= D3D11_USAGE_DEFAULT;				// �A�N�Z�X����
		bufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;			// �o�b�t�@�̎��
		bufferDesc.ByteWidth			= m_BufferArray[count].NumIndex * sizeof( UINT );
		bufferData.pSysMem				= &tempIndexArray.front();

		if( FAILED( lpdevice->CreateBuffer( &bufferDesc, &bufferData, &m_BufferArray[count].lpIndex ) ) )
		{
			ErrorMessageBox( "���b�V���̃C���f�b�N�X�o�b�t�@�쐬���s" );
		}

		// ���_���i�[
		// ���_�ʒu�ƃe�N�X�`�����W�͂��łɊi�[���Ă���̂ő��̏����i�[���Ă���
		Vector3 temp;
		for( UINT i = 0; i < tempIndexArray.size(); i += 3 )
		{
			// �@�������v�Z���Ċi�[
			// �v�Z�����́A�ʂ̖@�����O�ςŊ���o���n�[�t�x�N�g���Œ��Ԃ̖@��������o��
			// �܂��͊O�ς��g���Ėʂ̖@�����o��
			temp.SetCross( 
				it->second.Vertex[tempIndexArray[i]].Pos - it->second.Vertex[tempIndexArray[i + 1]].Pos,
				it->second.Vertex[tempIndexArray[i]].Pos - it->second.Vertex[tempIndexArray[i + 2]].Pos );

			// �v�Z�����@���𑫂����݁A���K�����ăn�[�t�x�N�g���ɂ���
			it->second.Vertex[tempIndexArray[i]].Normal += temp.Normalize();
			it->second.Vertex[tempIndexArray[i + 1]].Normal += temp.Normalize();
			it->second.Vertex[tempIndexArray[i + 2]].Normal += temp.Normalize();
			
			it->second.Vertex[tempIndexArray[i]].Normal.Normalize();
			it->second.Vertex[tempIndexArray[i + 1]].Normal.Normalize();
			it->second.Vertex[tempIndexArray[i + 2]].Normal.Normalize();
		}

		// ���_�o�b�t�@�쐬(���������\�Ȃ悤�ɂ��Ă���)
		bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;				// �A�N�Z�X����
		bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;			// ���������\
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;			// �o�b�t�@�̎��
		bufferDesc.ByteWidth		= it->second.Vertex.size() * sizeof( MESHVERTEX );
		bufferData.pSysMem			= &it->second.Vertex.front();

		if( FAILED( lpdevice->CreateBuffer( &bufferDesc, &bufferData, &m_BufferArray[count++].lpVertex ) ) )
		{
			ErrorMessageBox( "���b�V���̒��_�o�b�t�@�쐬���s" );
		}

		++it;
	}

	// �T���v���X�e�[�g�쐬
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
}

// �`��
void CModel::Draw( ID3D11DeviceContext1* lpdeviceContext, UINT slot )
{
	lpdeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// �e�N�X�`���Z�b�g
	//m_MaterialArray[0].lpTexture->SetTexture(lpdeviceContext, slot);

	// �`��
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