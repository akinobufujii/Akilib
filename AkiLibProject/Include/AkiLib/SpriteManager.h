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
	struct SpriteInfo
	{
		XMFLOAT2				pos;		//! �|���S�����������߂̍��W
		float					scale;		//! �|���S���̑傫��
		float					rotate;		//! �|���S���̉�]
		AkiLib::CTexture*		lpTexture;	//! �e�N�X�`��
		AkiLib::CSamplerState*	lpSampler;	//! �T���v���X�e�[�g
		
		SpriteInfo()
			:pos(0, 0)
			,scale(1)
			,rotate(0)
			,lpTexture(nullptr)
			,lpSampler(nullptr)
		{
		}
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