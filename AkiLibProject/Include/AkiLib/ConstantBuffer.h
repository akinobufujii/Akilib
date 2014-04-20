//==============================================================================
/// @file	ConstantBuffer.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_CONStANTBUFFER_H__
#define __AKILIB_CONStANTBUFFER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�R���X�^���g�o�b�t�@�N���X(�e���v���[�g�N���X)
	/// @brief	���K��16�o�C�g�A���C�����ꂽ�\���̂��e���v���[�g�^�C�v�ɂɂ��Ă�������
	//==============================================================================
	template<typename TYPE>
	class ConstantBuffer : public TYPE
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		inline ConstantBuffer():
			m_lpConstantBuffer(nullptr),
			BUFFER_SIZE(sizeof(TYPE))
		{
		}

		/// @brief	�f�X�g���N�^
		inline ~ConstantBuffer()
		{
			Release();
		}

		/// @brief	������
		/// @param[in]	lpdevice	�f�o�C�X
		/// @retval true	����
		/// @retval false	���s
		inline bool Init(ID3D11Device1* lpdevice)
		{
			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth		= BUFFER_SIZE;
			bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags		= 0;

			if( FAILED( lpdevice->CreateBuffer( &bufferDesc, nullptr, &m_lpConstantBuffer ) ) )
			{
				ErrorMessageBox( "�R���X�^���g�o�b�t�@�쐬���s" );
				return false;
			}

			return true;
		}

		/// @brief	���
		inline void Release()
		{
			AkiLib::SafeRelease(m_lpConstantBuffer);
		}

		/// @brief	�o�b�t�@�|�C���^�[�l��
		/// @return	�o�b�t�@�̃|�C���^�[��Ԃ�
		inline ID3D11Buffer* GetBufferPointer() const
		{
			return m_lpConstantBuffer;
		}

		/// @brief	�e���v���[�g�^�C�v�̓��e�l��
		/// @return	�e���v���[�g�^�C�v�̓��e�|�C���^�[�ŕԂ�
		const TYPE* GetStructPointer() const
		{
			return static_cast<const TYPE*>(this);
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		const size_t	BUFFER_SIZE;		// �R���X�^���g�o�b�t�@�̃T�C�Y
		ID3D11Buffer*	m_lpConstantBuffer;	// �R���X�^���g�o�b�t�@
	};
}

#endif	// __AKILIB_CONStANTBUFFER_H__