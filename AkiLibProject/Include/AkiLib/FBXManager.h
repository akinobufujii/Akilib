//==============================================================================
/// @file	FBXManager.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_FBXMANAGER_H__
#define __AKILIB_FBXMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	FBX���f���}�l�[�W���[(�V���O���g��)
	//==============================================================================
	class CFBXManager
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�C���X�^���X�l��
		/// @return	�C���X�^���X��Ԃ�
		inline static CFBXManager* GetInstance()
		{
			static CFBXManager instance;
			return &instance;
		}

		/// @brief	������(���̃��\�b�h��K���ŏ��ɌĂяo��)
		/// @retval	true	����������
		/// @retval	false	���������s
		bool Init();

		/// @brief	���
		void Release();

		/// @brief	�t�@�C����ǂݍ���
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	lpcontext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	filename	�ǂݍ��ރt�@�C����
		/// @return	���f���f�[�^(NULL�Ȃ�ǂݍ��ݎ��s)
		CModel* LoadFile(
			ID3D11Device1*			lpdevice,
			ID3D11DeviceContext1*	lpcontext,
			const char*				filename);

	private:

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		FbxManager*		m_lpFBXManager;	/// FBX�}�l�[�W���[
		FbxIOSettings*	m_lpFBXIO;		/// FBXIO�I�u�W�F�N�g
		FbxScene*		m_lpFBXScene;	/// FBX�V�[��
		//std::map<std::string, >
		
		//==============================================================================
		// ���\�b�h
		//==============================================================================	
		/// @brief	�R���X�g���N�^
		CFBXManager();

		/// @brief	�f�X�g���N�^
		~CFBXManager();

		/// @brief	���b�V���ǂݍ���
		/// @param[in]	lpMesh	���b�V��
		void LoadMesh(FbxMesh* lpMesh, CModel** lppModel);

		/// @brief	�}�e���A���ǂݍ���
		/// @param[in]	lpNode	�m�[�h
		void LoadMaterial(FbxNode* lpNode, ID3D11Device1*			lpdevice,
			ID3D11DeviceContext1*	lpcontext,CModel** lppModel);

		/// @brief	�O�p��
		/// @param[in]	lpNode	�m�[�h
		void AllTriangulate(FbxNode* lpNode);
	};
}

#define AKIFBXMGR	AkiLib::CFBXManager::GetInstance()

#endif	// __AKILIB_FBXMANAGER_H__