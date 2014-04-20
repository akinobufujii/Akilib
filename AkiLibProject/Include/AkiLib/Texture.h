//==============================================================================
/// @file	Texture.h
/// @brief	�e�N�X�`���֘A
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_TEXTURE_H__
#define __AKILIB_TEXTURE_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�e�N�X�`���N���X(�����_�[�^�[�Q�b�g�쐬�@�\�t��)
	//==============================================================================
	class CTexture : private CNoCopyable
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CTexture();

		/// @brief	�f�X�g���N�^
		~CTexture();

		/// @brief	�t�@�C������e�N�X�`�����쐬����
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	lpcontext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	filename	�ǂݍ��ރt�@�C����
		/// @param[in]	filter		�t�B���^�[���[�h(�f�t�H���g�͐��`���)
		/// @param[in]	addressMode	�A�h���b�V���O���[�h(�f�t�H���g�̓��b�s���O���[�h)
		/// @retval true  ����
		/// @retval false ���s
		bool CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCSTR						filename,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	�t�@�C������e�N�X�`�����쐬����
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	lpcontext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	filename	�ǂݍ��ރt�@�C����
		/// @param[in]	filter		�t�B���^�[���[�h(�f�t�H���g�͐��`���)
		/// @param[in]	addressMode	�A�h���b�V���O���[�h(�f�t�H���g�̓��b�s���O���[�h)
		/// @retval true  ����
		/// @retval false ���s
		bool CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCWSTR						filename,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	�����_�[�^�[�Q�b�g�쐬
		/// @param[in]	lpdevice	�f�o�C�X
		/// @param[in]	width		��
		/// @param[in]	height		����
		/// @param[in]	format		�e�N�X�`���̃t�H�[�}�b�g
		/// @param[in]	filter		�t�B���^�[���[�h(�f�t�H���g�͐��`���)
		/// @param[in]	addressMode	�A�h���b�V���O���[�h(�f�t�H���g�̓��b�s���O���[�h)
		/// @retval true  ����
		/// @retval false ���s
		bool CreateRenderTarget( 
			ID3D11Device1*				lpdevice,
			UINT						width,
			UINT						height,
			DXGI_FORMAT					format = DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	�e�N�X�`����ݒ肷��
		/// @param[in]	lpdeviceContext	�f�o�C�X�R���e�L�X�g
		/// @param[in]	slot			�ݒ肷��X���b�g
		void SetTexture( ID3D11DeviceContext1* lpdeviceContext, UINT slot );

		/// @brief	�����_�|�^�[�Q�b�g�r���[���l��
		/// @return	�����_�[�^�[�Q�b�g�r���[��Ԃ�
		inline ID3D11RenderTargetView* GetRenederTargetView() const
		{
			return m_lpRenderTargetView;
		}

		/// @brief	�V�F�[�_�[���\�[�X�r���[���l��
		/// @return �V�F�[�_�[���\�[�X�r���[��Ԃ�
		inline ID3D11ShaderResourceView* const * GetShaderResourceView() const
		{
			return &m_lpTextureView;
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		ID3D11Resource*				m_lpTextureResource;	///< �V�F�[�_���\�[�X
		ID3D11SamplerState*			m_lpSamplerState;		///< �T���v���X�e�[�g
		ID3D11Texture2D*			m_lpTextuer;			///< �e�N�X�`��
		ID3D11ShaderResourceView*	m_lpTextureView;		///< �V�F�[�_�[���\�[�X�r���[
		ID3D11RenderTargetView*		m_lpRenderTargetView;	///< �����_�|�^�[�Q�b�g�r���[
	};
}

#endif // #ifndef __AKILIB_TEXTURE_H__