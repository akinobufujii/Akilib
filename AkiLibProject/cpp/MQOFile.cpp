//==============================================================================
/// @file	MQOFile.cpp
/// @brief	Metasequoiaファイル読み込み実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CMQOFile::CMQOFile()
{
}

// デストラクタ
CMQOFile::~CMQOFile()
{
}

// 読み込み
bool CMQOFile::LoadFile( ID3D11Device1* lpdevice, LPCSTR file )
{
	// ファイルオープン
	std::fstream File( file );
	char temp[0xff];
	UINT tempNum;

	if( File.is_open() )
	{
		// ファイル終端まで読み込む
		while( File.eof() == false )
		{
			File >> temp;

			// マテリアル読み込み
			if( strcmp( temp, "Material" ) == 0 )
			{
				// マテリアル数を獲得して一気に読み込む
				File >> tempNum;
				m_MaterialArray.resize( tempNum );
				File.ignore( 4 );

				for( UINT i = 0; i < m_MaterialArray.size(); ++i )
				{
					// マテリアル名取得
					File >> m_MaterialArray[i].Name;

					// ダブルクォーテーション削除
					m_MaterialArray[i].Name.erase( 0, 1 );
					m_MaterialArray[i].Name.erase( m_MaterialArray[i].Name.size() - 2 );

					// 色読み込み
					File.ignore( 15 );
					File >> m_MaterialArray[i].Color.x >> m_MaterialArray[i].Color.y >> m_MaterialArray[i].Color.z >> m_MaterialArray[i].Color.w;
					
					// 拡散光読み込み
					File.ignore( 6 );
					File >> m_MaterialArray[i].Diffuse;

					// 環境光読み込み
					File.ignore( 6 );
					File >> m_MaterialArray[i].Ambient;

					// 自己発光読み込み
					File.ignore( 6 );
					File >> m_MaterialArray[i].Emissive;

					// 反射光読み込み
					File.ignore( 6 );
					File >> m_MaterialArray[i].Specular;

					// 反射の強さ読み込み
					File.ignore( 8 );
					File >> m_MaterialArray[i].Power;

					// テクスチャ名読み込み
					File.ignore( 1 );
					File >> m_MaterialArray[i].TexName;
					m_MaterialArray[i].TexName.erase( 0, 5 );
					m_MaterialArray[i].TexName.erase( m_MaterialArray[i].TexName.size() - 2 );

					m_MaterialArray[i].lpTexture = AKINEW CTexture();
					m_MaterialArray[i].lpTexture->CreateTextureFromFile(lpdevice, AKID3D11CONTEXT, m_MaterialArray[i].TexName.c_str());
				}
			}

			// オブジェクト読み込み
			if( strcmp( temp, "Object" ) == 0 )
			{
				// オブジェクト名読み込み
				std::string objectName;
				File >> objectName;

				// ダブルクォーテーションを取り除く
				objectName.erase( 0, 1 );
				objectName.erase( objectName.size() - 2 );

				// 頂点情報を読み込む
				while( strcmp( temp, "vertex" ) )
				{
					File >> temp;
				}

				MeshObject tempObj;

				// 頂点数取得
				File >> tempNum;
				tempObj.Vertex.resize( tempNum );
				File.getline( temp, sizeof( temp ) );

				// 頂点情報を格納していく
				for( UINT i = 0; i < tempObj.Vertex.size(); ++i )
				{
					File >> tempObj.Vertex[i].Pos.x >> tempObj.Vertex[i].Pos.y >> tempObj.Vertex[i].Pos.z;
					tempObj.Vertex[i].Pos.z	*= -1;	// 左手系座標なので反転させる

					tempObj.Vertex[i].Pos *= 0.01f;

					tempObj.Vertex[i].Normal = XMFLOAT3( 0, 0, 0 );
				}

				// 面情報を読み込む
				while( strcmp( temp, "face" ) )
				{
					File >> temp;
				}

				// 面数獲得
				File >> tempNum;
				tempObj.Face.resize( tempNum );
				File.getline( temp, sizeof( temp ) );

				// 面情報を格納していく
				for( UINT i = 0; i < tempObj.Face.size(); ++i )
				{
					File >> tempNum;	// 面構成頂点数獲得
					tempObj.Face[i].IndexArray.resize( tempNum );
					tempObj.Face[i].UV.resize( tempNum );

					// インデックス読み込み
					File.ignore( 3 );
					for( UINT j = 0; j < tempObj.Face[i].IndexArray.size(); ++j )
					{
						File >> tempObj.Face[i].IndexArray[j];
					}

					// 参照マテリアル番号取得
					File.ignore( 4 );
					File >> tempObj.Face[i].Material;

					// テクスチャ座標読み込み
					File.ignore( 5 );
					for( UINT j = 0; j < tempObj.Face[i].UV.size(); ++j )
					{
						File >> tempObj.Face[i].UV[j].x >> tempObj.Face[i].UV[j].y;

						// 頂点にテクスチャ座標を格納
						tempObj.Vertex[tempObj.Face[i].IndexArray[j]].UV = tempObj.Face[i].UV[j];
					}
					File.ignore( 1 );
				}

				// マップに登録
				m_MeshObjectMap.insert( MESHOBJECTMAPPAIR( objectName, tempObj ) );
			}
		}

		// 読み込み終了
		File.close();

		// バッファ作成
		CreateMeshBuffer( lpdevice );

		return true;
	}
	else
	{
		return false;
	}
}
