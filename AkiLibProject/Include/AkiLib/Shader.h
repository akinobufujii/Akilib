//==============================================================================
/// @file	Shader.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_SHADER_H__
#define __AKILIB_SHADER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�V�F�[�_���b�s���O�N���X(�R�s�[�֎~)
	//==============================================================================
	class CShader : private CNoCopyable
	{
	public:
		
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CShader();

		/// @brief	�f�X�g���N�^
		~CShader();

		/// @brief	�t�@�C�����ݒ�(���ׂă��\�b�h�̑O�ɌĂ�)
		/// @param[in]	file	�ǂݍ��ރt�@�C����
		/// @param[in]	lpevice	�f�o�C�X
		inline void Init( LPCSTR file, ID3D11Device1* lpdevice )
		{
			m_FileName = file;
			m_lpDevice = lpdevice;
		}

		// ���
		/// @brief	���ׂẴV�F�[�_���J������
		void Release();

		/// @brief	���_�V�F�[�_�[�쐬
		/// @param[in]	funcName		���_�V�F�[�_�[��
		/// @param[in]	version			�R���p�C���o�[�W����
		/// @param[in]	layout			���_���C�A�E�g(���C�A�E�g���쐬����Ȃ�ΕK�{)
		/// @param[in]	layoutNum		���_���C�A�E�g��(���C�A�E�g���쐬����Ȃ�ΕK�{)
		/// @param[out]	lppoutLayout	�쐬�������_���C�A�E�g��Ԃ�(���C�A�E�g���쐬����Ȃ�ΕK�{)
		/// @retval true	����
		/// @retval false	���s
		bool CreateVertexShader( 
			LPCSTR							funcName,
			LPCSTR							version,
			const D3D11_INPUT_ELEMENT_DESC* layout		= nullptr,
			size_t							layoutNum	= 0,
			ID3D11InputLayout**				lppoutLayout= nullptr );

		/// @brief	�s�N�Z���V�F�[�_�[�쐬
		/// @param[in]	funcName		�s�N�Z���V�F�[�_�[��
		/// @param[in]	version			�R���p�C���o�[�W����
		/// @retval true	����
		/// @retval false	���s
		bool CreatePixelShader( LPCSTR funcName, LPCSTR version );

		/// @brief	�R���s���[�g�V�F�[�_�[�쐬
		/// @param[in]	funcName		�R���s���[�g�V�F�[�_�[��
		/// @param[in]	version			�R���p�C���o�[�W����
		/// @retval true	����
		/// @retval false	���s
		bool CreateComputeShader( LPCSTR funcName, LPCSTR version );

		/// @brief	�n���V�F�[�_�[�쐬
		/// @param[in]	funcName		�n���V�F�[�_�[��
		/// @param[in]	version			�R���p�C���o�[�W����
		/// @retval true	����
		/// @retval false	���s
		bool CreateHallShader( LPCSTR funcName, LPCSTR version );

		/// @brief	�h���C���V�F�[�_�[�쐬
		/// @param[in]	funcName		�h���C���V�F�[�_�[��
		/// @param[in]	version			�R���p�C���o�[�W����
		/// @retval true	����
		/// @retval false	���s
		bool CreateDomainShader( LPCSTR funcName, LPCSTR version );

		/// @brief	���_�V�F�[�_���l��
		/// @param[in]	funcName	���_�V�F�[�_��
		/// @return	�쐬�ς݂̒��_�V�F�[�_�I�u�W�F�N�g
		inline ID3D11VertexShader* GetVertexShader( LPCSTR funcName )
		{
			return MapGet<VERTEXSHADERMAP, ID3D11VertexShader*>( m_VertexShaderMap, funcName );
		}

		/// @brief	�s�N�Z���V�F�[�_���l��
		/// @param[in]	funcName	�s�N�Z���V�F�[�_��
		/// @return	�쐬�ς݂̃s�N�Z���V�F�[�_�I�u�W�F�N�g
		inline ID3D11PixelShader* GetPixelShader( LPCSTR funcName )
		{
			return MapGet<PIXELSHADERMAP, ID3D11PixelShader*>( m_PixelShaerMap, funcName );
		}

		/// @brief	�R���s���[�g�V�F�[�_���l��
		/// @param[in]	funcName	�R���s���[�g�V�F�[�_��
		/// @return	�쐬�ς݂̃R���s���[�g�V�F�[�_�I�u�W�F�N�g
		inline ID3D11ComputeShader* GetComputeShader( LPCSTR funcName )
		{
			return MapGet<COMPUTESHADERMAP, ID3D11ComputeShader*>( m_ComputeShaderMap, funcName );
		}

		/// @brief	�n���V�F�[�_���l��
		/// @param[in]	funcName	�n���V�F�[�_��
		/// @return	�쐬�ς݂̃n���V�F�[�_�I�u�W�F�N�g
		inline ID3D11HullShader* GetHullShader( LPCSTR funcName )
		{
			return MapGet<HALLSHADERMAP, ID3D11HullShader*>( m_HallShaderMap, funcName );
		}

		/// @brief	�h���C���V�F�[�_���l��
		/// @param[in]	funcName	�h���C���V�F�[�_��
		/// @return	�쐬�ς݂̃h���C���V�F�[�_�I�u�W�F�N�g
		inline ID3D11DomainShader* GetDomainShader( LPCSTR funcName )
		{
			return MapGet<DOMAINSHADERMAP, ID3D11DomainShader*>( m_DomainShaderMap, funcName );
		}
		
	private:
		//==============================================================================
		// �^�錾
		//==============================================================================
		typedef std::map<std::string, ID3D11VertexShader*>		VERTEXSHADERMAP;		///< ���_�V�F�[�_�}�b�v
		typedef std::pair<std::string, ID3D11VertexShader*>		VERTEXSHADERMAPPAIR;	///< ���_�V�F�[�_�}�b�v�v�f
		typedef std::map<std::string, ID3D11PixelShader*>		PIXELSHADERMAP;			///< �s�N�Z���V�F�[�_�}�b�v
		typedef std::pair<std::string, ID3D11PixelShader*>		PIXELSHADERMAPPAIR;		///< �s�N�Z���V�F�[�_�}�b�v�v�f
		typedef std::map<std::string, ID3D11ComputeShader*>		COMPUTESHADERMAP;		///< �R���s���[�g�V�F�[�_�}�b�v
		typedef std::pair<std::string, ID3D11ComputeShader*>	COMPUTESHADERMAPPAIR;	///< �R���s���[�g�V�F�[�_�}�b�v�v�f
		typedef std::map<std::string, ID3D11HullShader*>		HALLSHADERMAP;			///< �n���V�F�[�_�}�b�v
		typedef std::pair<std::string, ID3D11HullShader*>		HALLSHADERMAPPAIR;		///< �n���V�F�[�_�}�b�v�v�f
		typedef std::map<std::string, ID3D11DomainShader*>		DOMAINSHADERMAP;		///< �h���C���V�F�[�_�}�b�v
		typedef std::pair<std::string, ID3D11DomainShader*>		DOMAINSHADERMAPPAIR;	///< �h���C���V�F�[�_�}�b�v�v�f

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		std::string		m_FileName;		///< �t�@�C����
		ID3D11Device1*	m_lpDevice;		///< �f�o�C�X

		VERTEXSHADERMAP		m_VertexShaderMap;	///< ���_�V�F�[�_�}�b�v
		PIXELSHADERMAP		m_PixelShaerMap;	///< �s�N�Z���V�F�[�_�}�b�v
		COMPUTESHADERMAP	m_ComputeShaderMap;	///< �R���s���[�g�V�F�[�_�}�b�v
		HALLSHADERMAP		m_HallShaderMap;	///< �n���V�F�[�_�}�b�v
		DOMAINSHADERMAP		m_DomainShaderMap;	///< �h���C���V�F�[�_�}�b�v

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�}�b�v���e���폜����
		/// @param[in]	obj	�}�b�v���e
		template<typename TYPE>
		inline static void MapRelease( TYPE obj )
		{
			SafeRelease( obj.second );
		}

		template<typename MAP, typename ELEMENT>
		inline ELEMENT MapGet( MAP map, LPCSTR name )
		{
			MAP::iterator it = map.find( name );

			if( it != map.end() )
			{
				return it->second;
			}

			return nullptr;
		}

		/// @brief	�t�@�C������ǂݍ���ŃR���p�C��
		/// @param[in]	funcName	HLSL�֐���
		/// @param[in]	version		�R���p�C���o�[�W����
		/// @return		�R���p�C���ς݂̃u���u
		ID3DBlob* CompileShaderFromFile( LPCSTR funcName, LPCSTR version );
	};
}

#endif	// !__AKILIB_SHADER_H__