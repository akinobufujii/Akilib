//==============================================================================
/// @file	SoundManager.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_SOUNDMANAGER_H__
#define __AKILIB_SOUNDMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�T�E���h�f�[�^�\����
	//==============================================================================
	struct SOUNDDATA
	{
		IXAudio2SourceVoice*			lpSourceVoice;	///< �Z�J���_���o�b�t�@
		std::shared_ptr<BYTE>			spBuffer;		///< �g�`�f�[�^
		DWORD							Size;			///< �g�`�f�[�^�̃T�C�Y
		std::shared_ptr<WAVEFORMATEX>	spSoundData;	///< �T�E���h�f�[�^
				
		/// @brief	�R���X�g���N�^
		SOUNDDATA():
			lpSourceVoice	( nullptr ),
			spBuffer		( nullptr ),
			spSoundData		( nullptr ),
			Size			( 0 )
		{
		}
	};

	typedef std::shared_ptr<SOUNDDATA>	SPSOUNDDATA;

	//==============================================================================
	/// @brief	�T�E���h�}�l�[�W���[�i�V���O���g���j
	//==============================================================================
	class CSoundManager : public CSingletonTemplate<CSoundManager>
	{
	public:
		//==============================================================================
		// �t�����h�N���X�w��
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// �萔
		//==============================================================================
		/// @brief	�G�t�F�N�g�^�C�v
		enum ESoundEffectType
		{
			SET_REVERB,			///< ���o�[�u�G�t�F�N�g
			SET_VOLUMEMETER,	///< �{�����[�����[�^�[�G�t�F�N�g
			SET_MAX,			///< �ő吔
		};

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	������
		/// @retval	true	����
		/// @retval	false	���s
		bool Init();

		/// @brief	���
		void Release();

		/// @brief	wav�t�@�C���ǂݍ���(Unicode�p)
		/// @param[in]	�ǂݍ���wav�t�@�C����
		/// @retval true	����
		/// @retval false	���s
		bool LoadWaveFile( LPWSTR file );

		/// @brief	wav�t�@�C���ǂݍ���(�}���`�o�C�g�p)
		/// @param[in]	�ǂݍ���wav�t�@�C����
		/// @retval true	����
		/// @retval false	���s
		bool LoadWaveFile( LPSTR file );

		/// @brief	�Đ��iUnicode�p�j
		void Play( LPCWSTR file );

		/// @brief	�Đ��i�}���`�o�C�g�p�j
		void Play( LPCSTR file );

		/// @brief	�G�t�F�N�g�ݒ�
		void SetEffect( IXAudio2SourceVoice* target, ESoundEffectType type );

	private:
		//==============================================================================
		// �^�錾
		//==============================================================================
		typedef std::list<SPSOUNDDATA>					SOUNDDATALIST;	///< �T�E���h�f�[�^���X�g�^
		typedef std::map<std::wstring, SOUNDDATALIST>	SOUNDMAPW;		///< �T�E���h�f�[�^�}�b�v(Unicode�p)
		typedef std::pair<std::wstring, SOUNDDATALIST>	SOUNDMAPAIRW;	///< �T�E���h�f�[�^�v�f(Unicode�p)
		typedef std::map<std::string, SOUNDDATALIST>	SOUNDMAPA;		///< �T�E���h�f�[�^�}�b�v(�}���`�o�C�g�p)
		typedef std::pair<std::string, SOUNDDATALIST>	SOUNDMAPAIRA;	///< �T�E���h�f�[�^�v�f(�}���`�o�C�g�p)

		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		IXAudio2*				m_lpAudioEngine;	///< �I�[�f�B�I�G���W���C���^�[�t�F�C�X
		IXAudio2MasteringVoice*	m_lpMasterVoice;	///< �}�X�^�[�{�C�X(�v���C�}���[�o�b�t�@)
		SOUNDMAPW				m_SoundMapW;		///< �T�E���h�}�b�v
		SOUNDMAPA				m_SoundMapA;		///< �T�E���h�}�b�v

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�R���X�g���N�^
		CSoundManager();

		/// @brief	�f�X�g���N�^
		~CSoundManager();
	};
}

#define SOUNDMGR	AkiLib::CSoundManager::GetInstance()

#endif // #define __AKILIB_SOUNDMANAGER_H__