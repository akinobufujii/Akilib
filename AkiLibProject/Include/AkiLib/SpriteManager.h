//==============================================================================
/// @file Sp@riteManager.h
/// @brief	2D�`��}�l�[�W���[
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_SPRITEMANAGER_H__
#define __AKILIB_SPRITEMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�X�v���C�g�̏��(���̏������ɕ`����s��)
	//==============================================================================
	class SpriteInfo
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		SpriteInfo()
			:pos(0, 0)
			,WH(100, 100)
			,scale(1)
			,rotate(0)
			,color(1, 1, 1, 1)
			,drawTexXY(0, 0)
			,drawTexWH(0, 0)
			,lpTexture(nullptr)
			,lpSampler(nullptr)
		{
		}

		/// @brief	�e�N�X�`���ƕ`���`��ݒ肷��
		/// @param[in]	lptex		�e�N�X�`��
		/// @param[in]	lpsampler	�T���v��
		inline void SetTextureAndDrawSize(AkiLib::CTexture* lptex, AkiLib::CSamplerState* lpsampler)
		{
			lpTexture = lptex;
			lpSampler = lpsampler;

			if(lpTexture)
			{
				// �`�敝�����̓e�N�X�`��t�듯���T�C�Y�ɂ���
				WH.x = lpTexture->GetTexture2DDesc().Width;
				WH.y = lpTexture->GetTexture2DDesc().Height;

				// �摜�̋�`���ݒ肷��
				drawTexXY = XMFLOAT2(0, 0);
				drawTexWH = XMFLOAT2(lpTexture->GetTexture2DDesc().Width, lpTexture->GetTexture2DDesc().Height);
			}
		}

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		XMFLOAT2				pos;		//! �|���S�����������߂̍��W
		XMFLOAT2				WH;			//! �`�悷�镝����
		float					scale;		//! �|���S���̑傫��
		float					rotate;		//! �|���S���̉�]
		XMFLOAT4				color;		//! �J���[
		XMFLOAT2				drawTexXY;	//! �`�悷��摜�̈ʒu
		XMFLOAT2				drawTexWH;	//! �`�悷��摜�̕�����
		AkiLib::CTexture*		lpTexture;	//! �e�N�X�`��
		AkiLib::CSamplerState*	lpSampler;	//! �T���v���X�e�[�g
	};

	//==============================================================================
	/// @brief	�X�v���C�g�}�l�[�W���[�i�V���O���g���j
	//==============================================================================
	class CSpriteManager : public CSingletonTemplate<CSpriteManager> 
	{
	public:
		//==============================================================================
		// �t�����h�N���X�w��
		//==============================================================================
		friend CSingletonTemplate;

		/// @brief	������
		/// @retval true	����
		/// @retval false	���s
		bool Init();

		/// @brief	���
		void Release();

		/// @brief	�`��(���ۂɂ̓L���[�ɐςނ���)
		/// @param[in]	info �`����
		void Draw(const SpriteInfo& info);

		/// @brief	�R���X�g���N�^���ׂĕ`��
		void Flush();
		
	private:

		//==============================================================================
		// �錾
		//==============================================================================
		struct CBUFFER
		{
			AKIALIGN16	AkiLib::Matrix	mWVP;
		};

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		AkiLib::ConstantBuffer<CBUFFER> m_ConstantBuffer;	//! 2D�V�F�[�_�p�R���X�^���g�o�b�t�@
		AkiLib::CShader					m_SpriteShader;		//! �X�v���C�g�V�F�[�_
		AkiLib::CBoard					m_Board;			//! �`��p�|���S��
		ID3D11InputLayout*				m_lpInputLayout;	//! ���̓��C�A�E�g
		std::queue<SpriteInfo>			m_SpriteQueue;		//! �X�v���C�g�̃L���[

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�R���X�g���N�^
		CSpriteManager();

		/// @brief	�f�X�g���N�^
		~CSpriteManager();
	};
}

#define AKISPRITEMGR AkiLib::CSpriteManager::GetInstance()

#endif	// #define __AKILIB_SPRITEMANAGER_H__