//==============================================================================
/// @file	D3D11Manager.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_D3D11MANAGER_H__
#define __AKILIB_D3D11MANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	D3D11�}�l�[�W���[�i�V���O���g���j
	//==============================================================================
	class CD3D11Manager : public CSingletonTemplate<CD3D11Manager> 
	{
	public:
		//==============================================================================
		// �t�����h�N���X�w��
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�f�X�g���N�^
		~CD3D11Manager();

		/// @brief ������
		/// @param[in]	hwnd		�E�B���h�E�n���h��
		/// @param[in]	width		�o�b�N�o�b�t�@�̕�
		/// @param[in]	height		�o�b�N�o�b�t�@�̍���
		/// @param[in]	iswindow	�E�B���h�E���[�h(true�ŃE�B���h�E���[�h)
		/// @retval true	����
		/// @retval false	���s
		bool Init( HWND hwnd, UINT width, UINT height, bool iswindow = true );

		/// @brief	���
		void Release();

		/// @brief	�v���C�}���[�̉�ʂƐ[�x�o�b�t�@�N���A
		/// @param[in]	r	�Ԑ���(0�`1)
		/// @param[in]	g	�ΐ���(0�`1)
		/// @param[in]	b	����(0�`1)
		/// @param[in]	a	�A���t�@����(0�`1)
		void ClearPrimaryBuffer( float r, float g, float b, float a );

		/// @brief	�w��̃����_�[�^�[�Q�b�g���N���A
		/// @param[in]	lprenderTarget	�N���A���郌���_�[�^�[�Q�b�g
		/// @param[in]	r				�Ԑ���(0�`1)
		/// @param[in]	g				�ΐ���(0�`1)
		/// @param[in]	b				����(0�`1)
		/// @param[in]	a				�A���t�@����(0�`1)
		void ClearRenderTarget( ID3D11RenderTargetView* lprenderTarget, float r, float g, float b, float a );

		/// @brief	�w��̐[�x�X�e���V�����N���A
		/// @param[in]	lpdepthStencil	�N���A���郌���_�[�^�[�Q�b�g
		void ClearDepthStencil( ID3D11DepthStencilView* lpdepthStencil );

		/// @brief	��ʍX�V
		void Flip();

		/// @brief	�����_�[�^�[�Q�b�g��߂�
		void ResetRenderTarget();

		/// @brief	�����_�[�^�[�Q�b�g�ݒ�
		/// @param[in]	lpdepthStencil	�[�x�X�e���V���r���[
		/// @param[in]	lprenderTarget0	0�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget1	1�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget2	2�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget3	3�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget4	4�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget5	5�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget6	6�Ԗڂ̃����_�[�^�[�Q�b�g
		/// @param[in]	lprenderTarget7	7�Ԗڂ̃����_�[�^�[�Q�b�g
		void SetRenderTargets( 
			ID3D11DepthStencilView* lpdepthStencil,
			ID3D11RenderTargetView* lprenderTarget0,
			ID3D11RenderTargetView* lprenderTarget1 = nullptr,
			ID3D11RenderTargetView* lprenderTarget2 = nullptr,
			ID3D11RenderTargetView* lprenderTarget3 = nullptr,
			ID3D11RenderTargetView* lprenderTarget4 = nullptr,
			ID3D11RenderTargetView* lprenderTarget5 = nullptr,
			ID3D11RenderTargetView* lprenderTarget6 = nullptr,
			ID3D11RenderTargetView* lprenderTarget7 = nullptr);

		/// @brief	���_�o�b�t�@�쐬
		/// @param[in]	vertexData	���_�f�[�^
		/// @param[in]	sizeVertex	���_�̑傫��
		/// @return	�쐬�������_�o�b�t�@
		ID3D11Buffer* CreateVertexBuffer( void* vertexData, size_t sizeVertex );

		/// @brief	�R���X�^���g�o�b�t�@�쐬
		/// @param[in]	sizeBuffer	�f�[�^�̑傫��
		/// @return	�쐬�����R���X�^���g�o�b�t�@
		ID3D11Buffer* CreateConstantBuffer( size_t sizeBuffer );

		/// @brief	�\���̃o�b�t�@�쐬
		/// @param[in]	sizeBuffer	�f�[�^�̑傫��
		/// @param[in]	stride		�X�g���C�h�̑傫��
		/// @param[in]	lpdata		�����f�[�^(NULL�ł���)
		/// @return	�쐬�����\���̃o�b�t�@
		ID3D11Buffer* CreateStructuredBuffer( size_t sizeBuffer, size_t stride, void* lpdata );

		/// @brief	�R���s���[�g�V�F�[�_�ɒl�𑗂邽�߂̃r���[���쐬
		/// @param[in]	lpstructuredBuffer	�쐬�ςݍ\���̃o�b�t�@
		/// @param[in]	numElement			�v�f��
		/// @return		�쐬�����V�F�[�_�[���\�[�X�r���[
		ID3D11ShaderResourceView* CreateComputeSRV( ID3D11Buffer* lpstructuredBuffer, UINT numElement );

		/// @brief	�A���I�[�_�[�h�A�N�Z�X�r���[���쐬
		/// @param[in]	lpstructuredBuffer	�쐬�ςݍ\���̃o�b�t�@
		/// @param[in]	numElement			�v�f��
		/// @return		�쐬�����A���I�[�_�[�h�A�N�Z�X�r���[
		ID3D11UnorderedAccessView* CreateUAV( ID3D11Buffer* lpstructuredBuffer, UINT numElement );

		/// @brief	�R���s���[�g�V�F�[�_�̌��ʂ��󂯎��
		/// @param[in]	lpstructuredBuffer	�쐬�ςݍ\���̃o�b�t�@
		/// @param[in]	lpreseveBuffer		���ʂ��󂯎�邽�߂̃o�b�t�@
		/// @param[in]	size				�󂯎��o�b�t�@�̃T�C�Y
		void GetComputShaderResult( ID3D11Buffer* lpstructuredBuffer, void* lpreseveBuffer, size_t size );

		/// @brief	�V�F�[�_�ɒl��n��
		/// @param[in]	slot		�n�����W�X�^
		/// @param[in]	buffer		���f������o�b�t�@
		/// @param[in]	lpdata		�������ރf�[�^
		/// @retval true  ����
		/// @retval false ���s
		bool SendValueToShader( UINT slot, ID3D11Buffer* buffer, const void* lpdata );

		/// @brief	�V�F�[�_�ɒl��n��
		/// @param[in]	slot		�n�����W�X�^
		/// @param[in]	buffer		���f������o�b�t�@
		/// @param[in]	lpdata		�������ރf�[�^
		/// @retval true  ����
		/// @retval false ���s
		template <typename TYPE>
		inline bool SendValueToShader( UINT slot, const ConstantBuffer<TYPE>& buffer )
		{
			return SendValueToShader(slot, buffer.GetBufferPointer(), buffer.GetStructPointer());
		}

		/// @brief	�f�o�C�X���l��
		/// @return	�f�o�C�X��Ԃ�
		inline ID3D11Device1* GetDevice() const
		{
			return m_lpDevice;
		}

		/// @brief	�f�o�C�X�R���e�L�X�g���l��
		/// @return	�f�o�C�X�R���e�L�X�g��Ԃ�
		inline ID3D11DeviceContext1* GetDeviceContext() const
		{
			return m_lpDeviceContext;
		}

		/// @brief	�[�x�X�e���V���r���[���l��
		/// @return	�[�x�X�e���V���r���[��Ԃ�
		inline ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return m_lpDepthStencilView;
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		ID3D11Device1*			m_lpDevice;				///< �f�o�C�X
		ID3D11DeviceContext1*	m_lpDeviceContext;		///< �f�o�C�X�R���e�L�X�g
		IDXGISwapChain1*		m_lpSwapChain;			///< �X���b�v�`�F�C��
		ID3D11RenderTargetView* m_lpBackBufferView;		///< �o�b�N�o�b�t�@�^�[�Q�b�g�r���[
		ID3D11DepthStencilView* m_lpDepthStencilView;	///< �[�x�X�e���V���o�b�t�@�^�[�Q�b�g�r���[
		ID3D11Texture2D*		m_lpDepthStencil;		///< �[�x�X�e���V���o�b�t�@

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�R���X�g���N�^
		CD3D11Manager();
	};
}

#define AKID3D11MGR AkiLib::CD3D11Manager::GetInstance()
#define AKID3D11DEVICE AkiLib::CD3D11Manager::GetInstance()->GetDevice()
#define AKID3D11CONTEXT AkiLib::CD3D11Manager::GetInstance()->GetDeviceContext()

#endif	// #define __AKILIB_D3D11MANAGER_H__