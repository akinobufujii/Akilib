//==============================================================================
/// @file	MQOFile.cpp
/// @brief	Metasequoia�t�@�C���ǂݍ��ݎ���
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CMQOFile::CMQOFile()
{
}

// �f�X�g���N�^
CMQOFile::~CMQOFile()
{
}

// �ǂݍ���
bool CMQOFile::LoadFile( ID3D11Device1* lpdevice, LPCSTR file )
{
	// �t�@�C���I�[�v��
	std::fstream File( file );
	char temp[0xff];
	UINT tempNum;

	if( File.is_open() )
	{
		// �t�@�C���I�[�܂œǂݍ���
		while( File.eof() == false )
		{
			File >> temp;

			// �}�e���A���ǂݍ���
			if( strcmp( temp, "Material" ) == 0 )
			{
				// �}�e���A�������l�����Ĉ�C�ɓǂݍ���
				File >> tempNum;
				m_MaterialArray.resize( tempNum );
				File.ignore( 4 );

				for( UINT i = 0; i < m_MaterialArray.size(); ++i )
				{
					// �}�e���A�����擾
					File >> m_MaterialArray[i].Name;

					// �_�u���N�H�[�e�[�V�����폜
					m_MaterialArray[i].Name.erase( 0, 1 );
					m_MaterialArray[i].Name.erase( m_MaterialArray[i].Name.size() - 2 );

					// �F�ǂݍ���
					File.ignore( 15 );
					File >> m_MaterialArray[i].Color.x >> m_MaterialArray[i].Color.y >> m_MaterialArray[i].Color.z >> m_MaterialArray[i].Color.w;
					
					// �g�U���ǂݍ���
					File.ignore( 6 );
					File >> m_MaterialArray[i].Diffuse;

					// �����ǂݍ���
					File.ignore( 6 );
					File >> m_MaterialArray[i].Ambient;

					// ���Ȕ����ǂݍ���
					File.ignore( 6 );
					File >> m_MaterialArray[i].Emissive;

					// ���ˌ��ǂݍ���
					File.ignore( 6 );
					File >> m_MaterialArray[i].Specular;

					// ���˂̋����ǂݍ���
					File.ignore( 8 );
					File >> m_MaterialArray[i].Power;

					// �e�N�X�`�����ǂݍ���
					File.ignore( 1 );
					File >> m_MaterialArray[i].TexName;
					m_MaterialArray[i].TexName.erase( 0, 5 );
					m_MaterialArray[i].TexName.erase( m_MaterialArray[i].TexName.size() - 2 );

					m_MaterialArray[i].lpTexture = AKINEW CTexture();
					m_MaterialArray[i].lpTexture->CreateTextureFromFile(lpdevice, AKID3D11CONTEXT, m_MaterialArray[i].TexName.c_str());
				}
			}

			// �I�u�W�F�N�g�ǂݍ���
			if( strcmp( temp, "Object" ) == 0 )
			{
				// �I�u�W�F�N�g���ǂݍ���
				std::string objectName;
				File >> objectName;

				// �_�u���N�H�[�e�[�V��������菜��
				objectName.erase( 0, 1 );
				objectName.erase( objectName.size() - 2 );

				// ���_����ǂݍ���
				while( strcmp( temp, "vertex" ) )
				{
					File >> temp;
				}

				MeshObject tempObj;

				// ���_���擾
				File >> tempNum;
				tempObj.Vertex.resize( tempNum );
				File.getline( temp, sizeof( temp ) );

				// ���_�����i�[���Ă���
				for( UINT i = 0; i < tempObj.Vertex.size(); ++i )
				{
					File >> tempObj.Vertex[i].Pos.x >> tempObj.Vertex[i].Pos.y >> tempObj.Vertex[i].Pos.z;
					tempObj.Vertex[i].Pos.z	*= -1;	// ����n���W�Ȃ̂Ŕ��]������

					tempObj.Vertex[i].Pos *= 0.01f;

					tempObj.Vertex[i].Normal = XMFLOAT3( 0, 0, 0 );
				}

				// �ʏ���ǂݍ���
				while( strcmp( temp, "face" ) )
				{
					File >> temp;
				}

				// �ʐ��l��
				File >> tempNum;
				tempObj.Face.resize( tempNum );
				File.getline( temp, sizeof( temp ) );

				// �ʏ����i�[���Ă���
				for( UINT i = 0; i < tempObj.Face.size(); ++i )
				{
					File >> tempNum;	// �ʍ\�����_���l��
					tempObj.Face[i].IndexArray.resize( tempNum );
					tempObj.Face[i].UV.resize( tempNum );

					// �C���f�b�N�X�ǂݍ���
					File.ignore( 3 );
					for( UINT j = 0; j < tempObj.Face[i].IndexArray.size(); ++j )
					{
						File >> tempObj.Face[i].IndexArray[j];
					}

					// �Q�ƃ}�e���A���ԍ��擾
					File.ignore( 4 );
					File >> tempObj.Face[i].Material;

					// �e�N�X�`�����W�ǂݍ���
					File.ignore( 5 );
					for( UINT j = 0; j < tempObj.Face[i].UV.size(); ++j )
					{
						File >> tempObj.Face[i].UV[j].x >> tempObj.Face[i].UV[j].y;

						// ���_�Ƀe�N�X�`�����W���i�[
						tempObj.Vertex[tempObj.Face[i].IndexArray[j]].UV = tempObj.Face[i].UV[j];
					}
					File.ignore( 1 );
				}

				// �}�b�v�ɓo�^
				m_MeshObjectMap.insert( MESHOBJECTMAPPAIR( objectName, tempObj ) );
			}
		}

		// �ǂݍ��ݏI��
		File.close();

		// �o�b�t�@�쐬
		CreateMeshBuffer( lpdevice );

		return true;
	}
	else
	{
		return false;
	}
}
