//==============================================================================
/// @file	FBXManager.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CFBXManager::CFBXManager():
	m_lpFBXManager(nullptr),
	m_lpFBXIO(nullptr),
	m_lpFBXScene(nullptr)
{
}

// �f�X�g���N�^
CFBXManager::~CFBXManager()
{
	Release();
}

// ������
bool CFBXManager::Init()
{
	// FBX�}�l�[�W���[�쐬
	if((m_lpFBXManager = FbxManager::Create()) == nullptr)
	{
		return false;
	}

	// FBX�t�@�C�����C���|�[�g���邽�߂̃I�u�W�F�N�g�쐬
	if((m_lpFBXIO = FbxIOSettings::Create(m_lpFBXManager, IOSROOT)) == nullptr)
	{
		return false;
	}

	// FBX�t�@�C�����C���|�[�g���邽�߂̃I�u�W�F�N�g���}�l�[�W���[�ɐݒ�
	m_lpFBXManager->SetIOSettings(m_lpFBXIO);

	// FBX�V�[���쐬
	if((m_lpFBXScene = FbxScene::Create(m_lpFBXManager, "AkiScene")) == nullptr)
	{
		return false;
	}

	// ���W�n��DirectX�ɂ���
	FbxAxisSystem SceneAxisSystem = m_lpFBXScene->GetGlobalSettings().GetAxisSystem();
	if(SceneAxisSystem != FbxAxisSystem::DirectX)
	{
		FbxAxisSystem::DirectX.ConvertScene(m_lpFBXScene);
	}

	return true;
}

// ���
void CFBXManager::Release()
{
	// FBX�V�[���j��
	if(m_lpFBXScene)
	{
		m_lpFBXScene->Destroy();
		m_lpFBXScene = nullptr;
	}

	// FBX�t�@�C���C���|�[�g�I�u�W�F�N�g�j��
	if(m_lpFBXIO)
	{
		m_lpFBXIO->Destroy();
		m_lpFBXIO = nullptr;
	}

	// FBX�}�l�[�W���[�j��
	if(m_lpFBXManager)
	{
		m_lpFBXManager->Destroy();
		m_lpFBXManager = nullptr;
	}
}

#if 1
// �t�@�C���ǂݍ���
CModel* CFBXManager::LoadFile(ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const char* filename)
{
	CModel* lpModel = AKINEW CModel();

	// �C���|�[�^�[�쐬
	FbxImporter* lpFBXImporter = FbxImporter::Create(m_lpFBXManager,"");

	// FBX�t�@�C����ǂݍ���
	if(lpFBXImporter && lpFBXImporter->Initialize(filename, -1, m_lpFBXManager->GetIOSettings()) == false) 
	{ 
		/*std::cout << "�t�@�C����ǂݍ��߂܂���ł���" << std::endl;
		std::cout << "�G���[���e = " << pFBXImporter->GetStatus().GetErrorString() << std::endl; */
		return false;
	}

	// �V�[���ɓǂݍ���FBX�t�@�C�����C���|�[�g
	lpFBXImporter->Import(m_lpFBXScene);

	// �C���|�[�g�����t�@�C�����폜
	// (�V�[���ɂ͎c���Ă���)
	lpFBXImporter->Destroy();

	// �O�p������
	AllTriangulate(m_lpFBXScene->GetRootNode());

	// ���b�V���ǂݍ���
	FbxNode* lpRootNode = m_lpFBXScene->GetRootNode();
	FbxNode*			lpNode		= nullptr;
	FbxNodeAttribute*	lpAttribute = nullptr;

	if(lpRootNode)
	{
		for(int i = 0; i < lpRootNode->GetChildCount(); ++i)
		{
			lpNode = lpRootNode->GetChild(i);

			// �m�[�h�����ǂ�
			for(int j = 0; j < lpNode->GetNodeAttributeCount(); ++j)
			{
				lpAttribute = lpNode->GetNodeAttributeByIndex(j);

				// ���b�V���Ȃ��񒊏o
				if(lpAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
					LoadMesh(static_cast<FbxMesh*>(lpAttribute), &lpModel);
				}
			}

			// �}�e���A���ǂݍ���
			LoadMaterial(lpNode, lpdevice, lpcontext, &lpModel);
		}
	}

	// �V�[���ɓǂݍ��܂�Ă�����̑S�Ĕj��
	while(lpRootNode->GetChildCount())
	{
		lpRootNode->RemoveChild(lpRootNode->GetChild(0));
	}

	// ���_�o�b�t�@�쐬
	lpModel->CreateMeshBuffer(lpdevice);

	return lpModel;
}

// ���b�V���ǂݍ���
void CFBXManager::LoadMesh(FbxMesh* lpMesh, CModel** lppModel)
{
	int			vertexIndex, uvIndex;
	FbxVector4	normal;
	FbxVector2	uv;

	// ���_�z��l��
	FbxVector4* pVertexArray = lpMesh->GetControlPoints();

	// UV�z��l��
	const FbxLayerElementArrayTemplate<FbxVector2>& UV_ARRAY = lpMesh->GetElementUV(0)->GetDirectArray();

	(*lppModel)->m_MeshObjectMap.insert(CModel::MESHOBJECTMAPPAIR(lpMesh->GetName(), CModel::MeshObject()));
	CModel::MESHOBJECTMAP::iterator it = (*lppModel)->m_MeshObjectMap.find(lpMesh->GetName());

	// ���_���i�[
	int numPoly = lpMesh->GetPolygonCount();

	// �ʐ��m��
	it->second.Face.resize(numPoly);
	it->second.Vertex.resize(lpMesh->GetTextureUVCount());

	for(int j = 0; j < numPoly; j++)
	{
		for(int k = 0, size = lpMesh->GetPolygonSize(j); k < size; k++)
		{
			// �e�C���f�b�N�X���擾
			vertexIndex = lpMesh->GetPolygonVertex(j, k);
			uvIndex = lpMesh->GetTextureUVIndex(j, k);

			// UV�C���f�b�N�X�Ɉˑ����čl����
			it->second.Face[j].IndexArray.push_back(uvIndex);

			// �ʒu
			it->second.Vertex[uvIndex].Pos.x = static_cast<float>(pVertexArray[vertexIndex][0]);
			it->second.Vertex[uvIndex].Pos.y = static_cast<float>(pVertexArray[vertexIndex][1]);
			it->second.Vertex[uvIndex].Pos.z = static_cast<float>(pVertexArray[vertexIndex][2]);

			// UV			
			it->second.Vertex[uvIndex].UV.x = static_cast<float>(UV_ARRAY[uvIndex][0]);
			it->second.Vertex[uvIndex].UV.y = static_cast<float>(1.f - UV_ARRAY[uvIndex][1]);
		}					
	}
}

// �}�e���A���ǂݍ���
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
		// �}�e���A���擾
		lpMaterial = lpNode->GetMaterial(mat);

		// �f�B�t���[�Y�̃v���p�e�B�擾
		fbxProperty = lpMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		// �t�@�C���Q�ƃe�N�X�`�����擾
		lpTexture = static_cast<FbxFileTexture*>(fbxProperty.GetSrcObject(FbxCriteria::ObjectType(FbxFileTexture::ClassId)));

		// �e�N�X�`���ǂݍ���
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
		// �֘A����UV��
		printf_s("UVSet = %s FileName = %s\n" ,lpTexture->UVSet.Get().Buffer() ,lpTexture->GetFileName());
#endif
	}
}

// �O�p��
void CFBXManager::AllTriangulate(FbxNode* pNode)
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	// �A�g���r���[�g�̎�ނ��m���߂�
	if(pNodeAttribute)
	{
		const FbxNodeAttribute::EType NODE_ATTRIBUTE_TYPE = pNodeAttribute->GetAttributeType();

		// ���b�V���A�i�[�o�X�A�i�[�o�X�T�[�t�F�X�A�p�b�`�Ȃ番���\
		if( NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eMesh			||
			NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eNurbs			||
		    NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::eNurbsSurface	||
		    NODE_ATTRIBUTE_TYPE == FbxNodeAttribute::ePatch)
		{
			FbxGeometryConverter converter(pNode->GetFbxManager());
			converter.Triangulate(m_lpFBXScene, true);
		}
	}
	
	// �q�m�[�h��T��
	for(int i = 0; i < pNode->GetChildCount(); ++i)
	{
	    AllTriangulate(pNode->GetChild(i));
	}
}

#endif