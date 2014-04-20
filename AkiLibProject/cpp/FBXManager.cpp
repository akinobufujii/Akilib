//==============================================================================
/// @file	FBXManager.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CFBXManager::CFBXManager():
	m_lpFBXManager(nullptr),
	m_lpFBXIO(nullptr),
	m_lpFBXScene(nullptr)
{
}

// デストラクタ
CFBXManager::~CFBXManager()
{
	Release();
}

// 初期化
bool CFBXManager::Init()
{
	// FBXマネージャー作成
	if((m_lpFBXManager = FbxManager::Create()) == nullptr)
	{
		return false;
	}

	// FBXファイルをインポートするためのオブジェクト作成
	if((m_lpFBXIO = FbxIOSettings::Create(m_lpFBXManager, IOSROOT)) == nullptr)
	{
		return false;
	}

	// FBXファイルをインポートするためのオブジェクトをマネージャーに設定
	m_lpFBXManager->SetIOSettings(m_lpFBXIO);

	// FBXシーン作成
	if((m_lpFBXScene = FbxScene::Create(m_lpFBXManager, "AkiScene")) == nullptr)
	{
		return false;
	}

	// 座標系をDirectXにする
	FbxAxisSystem SceneAxisSystem = m_lpFBXScene->GetGlobalSettings().GetAxisSystem();
	if(SceneAxisSystem != FbxAxisSystem::DirectX)
	{
		FbxAxisSystem::DirectX.ConvertScene(m_lpFBXScene);
	}

	return true;
}

// 解放
void CFBXManager::Release()
{
	// FBXシーン破棄
	if(m_lpFBXScene)
	{
		m_lpFBXScene->Destroy();
		m_lpFBXScene = nullptr;
	}

	// FBXファイルインポートオブジェクト破棄
	if(m_lpFBXIO)
	{
		m_lpFBXIO->Destroy();
		m_lpFBXIO = nullptr;
	}

	// FBXマネージャー破棄
	if(m_lpFBXManager)
	{
		m_lpFBXManager->Destroy();
		m_lpFBXManager = nullptr;
	}
}

#if 1
// ファイル読み込み
CModel* CFBXManager::LoadFile(ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const char* filename)
{
	CModel* lpModel = AKINEW CModel();

	// インポーター作成
	FbxImporter* lpFBXImporter = FbxImporter::Create(m_lpFBXManager,"");

	// FBXファイルを読み込む
	if(lpFBXImporter && lpFBXImporter->Initialize(filename, -1, m_lpFBXManager->GetIOSettings()) == false) 
	{ 
		/*std::cout << "ファイルを読み込めませんでした" << std::endl;
		std::cout << "エラー内容 = " << pFBXImporter->GetStatus().GetErrorString() << std::endl; */
		return false;
	}

	// シーンに読み込んだFBXファイルをインポート
	lpFBXImporter->Import(m_lpFBXScene);

	// インポートしたファイルを削除
	// (シーンには残っている)
	lpFBXImporter->Destroy();

	// 三角化する
	AllTriangulate(m_lpFBXScene->GetRootNode());

	// メッシュ読み込み
	FbxNode* lpRootNode = m_lpFBXScene->GetRootNode();
	FbxNode*			lpNode		= nullptr;
	FbxNodeAttribute*	lpAttribute = nullptr;

	if(lpRootNode)
	{
		for(int i = 0; i < lpRootNode->GetChildCount(); ++i)
		{
			lpNode = lpRootNode->GetChild(i);

			// ノードをたどる
			for(int j = 0; j < lpNode->GetNodeAttributeCount(); ++j)
			{
				lpAttribute = lpNode->GetNodeAttributeByIndex(j);

				// メッシュなら情報抽出
				if(lpAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
					LoadMesh(static_cast<FbxMesh*>(lpAttribute), &lpModel);
				}
			}

			// マテリアル読み込み
			LoadMaterial(lpNode, lpdevice, lpcontext, &lpModel);
		}
	}

	// シーンに読み込まれているもの全て破棄
	while(lpRootNode->GetChildCount())
	{
		lpRootNode->RemoveChild(lpRootNode->GetChild(0));
	}

	// 頂点バッファ作成
	lpModel->CreateMeshBuffer(lpdevice);

	return lpModel;
}

// メッシュ読み込み
void CFBXManager::LoadMesh(FbxMesh* lpMesh, CModel** lppModel)
{
	int			vertexIndex, uvIndex;
	FbxVector4	normal;
	FbxVector2	uv;

	// 頂点配列獲得
	FbxVector4* pVertexArray = lpMesh->GetControlPoints();

	// UV配列獲得
	const FbxLayerElementArrayTemplate<FbxVector2>& UV_ARRAY = lpMesh->GetElementUV(0)->GetDirectArray();

	(*lppModel)->m_MeshObjectMap.insert(CModel::MESHOBJECTMAPPAIR(lpMesh->GetName(), CModel::MeshObject()));
	CModel::MESHOBJECTMAP::iterator it = (*lppModel)->m_MeshObjectMap.find(lpMesh->GetName());

	// 頂点情報格納
	int numPoly = lpMesh->GetPolygonCount();

	// 面数確保
	it->second.Face.resize(numPoly);
	it->second.Vertex.resize(lpMesh->GetTextureUVCount());

	for(int j = 0; j < numPoly; j++)
	{
		for(int k = 0, size = lpMesh->GetPolygonSize(j); k < size; k++)
		{
			// 各インデックスを取得
			vertexIndex = lpMesh->GetPolygonVertex(j, k);
			uvIndex = lpMesh->GetTextureUVIndex(j, k);

			// UVインデックスに依存して考える
			it->second.Face[j].IndexArray.push_back(uvIndex);

			// 位置
			it->second.Vertex[uvIndex].Pos.x = static_cast<float>(pVertexArray[vertexIndex][0]);
			it->second.Vertex[uvIndex].Pos.y = static_cast<float>(pVertexArray[vertexIndex][1]);
			it->second.Vertex[uvIndex].Pos.z = static_cast<float>(pVertexArray[vertexIndex][2]);

			// UV			
			it->second.Vertex[uvIndex].UV.x = static_cast<float>(UV_ARRAY[uvIndex][0]);
			it->second.Vertex[uvIndex].UV.y = static_cast<float>(1.f - UV_ARRAY[uvIndex][1]);
		}					
	}
}

// マテリアル読み込み
void CFBXManager::LoadMaterial(
	FbxNode*				lpNode, 
	ID3D11Device1*			lpdevice,
	ID3D11DeviceContext1*	lpcontext,
	CModel**				lppModel)
{
	FbxSurfaceMaterialPtr	lpMaterial	= nullptr;
	FbxProperty				fbxProperty;
	FbxFileTexture*			lpTexture	= nullptr;

	for(int mat = 0; mat < lpNode->GetMaterialCount(); ++mat)
	{
		// マテリアル取得
		lpMaterial = lpNode->GetMaterial(mat);

		// ディフューズのプロパティ取得
		fbxProperty = lpMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		// ファイル参照テクスチャを取得
		lpTexture = static_cast<FbxFileTexture*>(fbxProperty.GetSrcObject(FbxCriteria::ObjectType(FbxFileTexture::ClassId)));

		// テクスチャ読み込み
		if(lpTexture)
		{
			(*lppModel)->m_MaterialArray.push_back(AkiLib::MATERIAL());
			(*lppModel)->m_MaterialArray[mat].lpTexture = AKINEW CTexture();
			(*lppModel)->m_MaterialArray[mat].lpTexture->CreateTextureFromFile(
				lpdevice,
				lpcontext,
				lpTexture->GetFileName());
		}
#if 0
		// 関連するUV名
		printf_s("UVSet = %s FileName = %s\n" ,lpTexture->UVSet.Get().Buffer() ,lpTexture->GetFileName());
#endif
	}
}

// 三角化
void CFBXManager::AllTriangulate(FbxNode* pNode)
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	// アトリビュートの種類を確かめる
	if(pNodeAttribute)
	{
		const FbxNodeAttribute::EType NODE_ATTRIBUTE_TYPE = pNodeAttribute->GetAttributeType();

		// メッシュ、ナーバス、ナーバスサーフェス、パッチなら分割可能
		if( NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eMesh			||
			NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eNurbs			||
		    NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eNurbsSurface	||
		    NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::ePatch)
		{
			FbxGeometryConverter converter(pNode->GetFbxManager());
			converter.Triangulate(m_lpFBXScene, true);
		}
	}
	
	// 子ノードを探索
	for(int i = 0; i < pNode->GetChildCount(); ++i)
	{
	    AllTriangulate(pNode->GetChild(i));
	}
}

#endif