//==============================================================================
/// @file	Model.h
/// @brief	���f�����N���X
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_MODEL_H__
#define __AKILIB_MODEL_H__

namespace AkiLib
{
	// ���b�V���̒��_���
	struct MESHVERTEX
	{
		Vector3		Pos;	///< ���_�ʒu
		Vector3		Normal;	///< �@��
		XMFLOAT2	UV;		///< �e�N�X�`�����W
	};

	// ��
	struct FACE
	{
		std::vector<UINT>		IndexArray;	///< �Y���z��
		UINT					Material;	///< �Q�ƍގ��ԍ�
		std::vector<XMFLOAT2>	UV;			///< UV���
	};

	// �}�e���A��
	struct MATERIAL
	{
		std::string	Name;		///< �}�e���A����
		XMFLOAT4	Color;		///< �F
		float		Diffuse;	///< �g�U��
		float		Ambient;	///< ����
		float		Emissive;	///< ���ȏƖ�
		float		Specular;	///< ���ˌ�
		float		Power;		///< ���˂̋���
		std::string	TexName;	///< �e�N�X�`����
		CTexture*	lpTexture;	///< �e�N�X�`��
	};

	//==============================================================================
	/// @brief	���f���N���X(�f�[�^�Ǘ��̂�)
	//==============================================================================
	class CModel
	{
	public:
		//==============================================================================
		// �萔
		//==============================================================================
		static const D3D11_INPUT_ELEMENT_DESC	VERTEX_LAYOUT[];	/// ���_���C�A�E�g

		//==============================================================================
		// �^�錾
		//==============================================================================
		struct MeshObject
		{
			std::vector<MESHVERTEX>	Vertex;		///< ���_���
			std::vector<FACE>		Face;		///< �ʏ��
		};

		struct BUFFER
		{
			ID3D11Buffer*	lpIndex;	///< �C���f�b�N�X�o�b�t�@
			ID3D11Buffer*	lpVertex;	///< ���_�o�b�t�@
			UINT			NumIndex;	///< �C���f�b�N�X�̐�
		};

		typedef std::map<std::string, MeshObject>		MESHOBJECTMAP;
		typedef std::pair<std::string, MeshObject>		MESHOBJECTMAPPAIR;
		typedef std::vector<MATERIAL>					MATERIALARRAY;
		typedef std::vector<BUFFER>						BUFFERARRAY;

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		MESHOBJECTMAP		m_MeshObjectMap;	///< ���b�V���I�u�W�F�N�g�}�b�v
		MATERIALARRAY		m_MaterialArray;	///< �ގ��z��
		BUFFERARRAY			m_BufferArray;		///< �o�b�t�@�z��(�I�u�W�F�N�g�����ł���)
		
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CModel();

		/// @brief	�f�X�g���N�^
		virtual ~CModel();

		/// @brief	���b�V���̃o�b�t�@�쐬
		/// @param[in]	lpdeviceContext	�f�o�C�X�R���e�L�X�g
		void CreateMeshBuffer(ID3D11Device1* lpdevice );

		/// @brief	�`��
		/// @param[in]	lpdeviceContext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	slot			�ݒ肷��X���b�g(�f�t�H���g��0)
		virtual void Draw(ID3D11DeviceContext1* lpdeviceContex, UINT slot = 0);
	};	
}

#endif	// #define __AKILIB_MODEL_H__