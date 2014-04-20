//==============================================================================
/// @file	FPSManager.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_FPSMANAGER__
#define __AKILIB_FPSMANAGER__

namespace AkiLib
{
	//==============================================================================
	/// @brief	FPS�Ǘ��N���X
	//==============================================================================
	class CFPSManager
	{
	public:
		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		/// @param[in]	fps	FPS���[�g
		CFPSManager( const UINT fps );

		/// @brief	�f�X�g���N�^
		~CFPSManager(){}

		/// @brief	�X�V
		void Update();

		/// @brief	�w��FPS�����x�����ǂ����𔻒肷��
		/// @retval true  �x��
		/// @retval false �x���Ȃ�
		inline bool IsSlow() const
		{
			return m_SlowFlg;
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		const USHORT			m_FPS;				///< FPS���[�g
		const double			m_FRAME_SECOND;		///< �w��FPS�ɑ΂��鎞��
		int						m_Count;			///< �J�E���^
		int						m_CountTime;		///< �t���[�����ԕۑ��p
		int						m_SaveTime;			///< �O�̃t���[���̎��ԕۑ��p
		std::shared_ptr<int>	m_spFlameAvgArray;	///< 1�����ϗp�z��
		double					m_Avg;				///< ����
		bool					m_SlowFlg;			///< �x�����ǂ���
	};

};

#endif // !__AKILIB_FPSMANAGER__