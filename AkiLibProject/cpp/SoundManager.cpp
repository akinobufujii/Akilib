//==============================================================================
/// @file	SoundManager.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CSoundManager::CSoundManager():
	m_lpAudioEngine( nullptr ),
	m_lpMasterVoice( nullptr )
{
}

// �f�X�g���N�^
CSoundManager::~CSoundManager()
{
	Release();
}

// ������
bool CSoundManager::Init()
{
	try
	{
		// COM������
		if( FAILED( CoInitializeEx( nullptr, COINIT_MULTITHREADED ) ) )
		{
			throw "COM���������s";
		}

		// �I�[�f�B�I�G���W��������
		if( FAILED( XAudio2Create( &m_lpAudioEngine ) ) )
		{
			throw "�I�[�f�B�I�G���W���쐬���s";
		}

		// �}�X�^�[�{�C�X�쐬
		if( FAILED( m_lpAudioEngine->CreateMasteringVoice( &m_lpMasterVoice ) ) )
		{
			throw "�}�X�^�[�{�C�X�쐬���s";
		}
	}
	catch( LPSTR& error )
	{
		Release();
		ErrorMessageBox( error );
		return false;
	}

	return true;
}

// ���
void CSoundManager::Release()
{
	m_SoundMapW.clear();
	m_SoundMapA.clear();
	SafeRelease( m_lpAudioEngine );
	CoUninitialize();
}

// wav�t�@�C���ǂݍ���(Unicode)
bool CSoundManager::LoadWaveFile(LPWSTR file)
{
	// �܂��͊��ɓo�^����Ă��Ȃ�������
	SOUNDMAPW::iterator it = m_SoundMapW.find( file );

	// �o�^����Ă��Ȃ���Γǂݍ���
	if( it == m_SoundMapW.end() )
	{
		MMCKINFO	riffckInfo;							// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
		MMCKINFO	ckInfo;								// �`�����N���
		SPSOUNDDATA	sptempSoundData( AKINEW SOUNDDATA() );	// �T�E���h�f�[�^
		
		PCMWAVEFORMAT pcmWaveForm;

		// WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
		HMMIO hmmio = mmioOpenW( file, nullptr, MMIO_ALLOCBUF | MMIO_READ );

		// �t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
		mmioDescend( hmmio, &riffckInfo, nullptr, 0 );

		// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );

		//�t�H�[�}�b�g��ǂݍ���
		mmioRead( hmmio, reinterpret_cast<HPSTR>( &pcmWaveForm ), sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData = std::shared_ptr<WAVEFORMATEX>( AKINEW WAVEFORMATEX );
		memcpy( sptempSoundData->spSoundData.get(), &pcmWaveForm, sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData->cbSize = 0;
		mmioAscend( hmmio, &ckInfo, 0 );

		// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
		ckInfo.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
		sptempSoundData->Size		= ckInfo.cksize;
		sptempSoundData->spBuffer	= std::shared_ptr<BYTE>( AKINEW BYTE[ckInfo.cksize] );
		mmioRead( hmmio, reinterpret_cast<HPSTR>( sptempSoundData->spBuffer.get() ), sptempSoundData->Size );
		
		//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
		if( FAILED( m_lpAudioEngine->CreateSourceVoice( &sptempSoundData->lpSourceVoice, sptempSoundData->spSoundData.get() ) ) )
		{
			std::wstring errorMessage = L"�T�E���h�f�[�^�쐬���s\n�t�@�C����:";
			errorMessage += file;

			ErrorMessageBox( errorMessage.c_str() );

			return false;
		}

		// �}�b�v�ɓo�^
		SOUNDDATALIST tempList;
		tempList.push_back( sptempSoundData );
		m_SoundMapW.insert( SOUNDMAPAIRW( file, tempList ) );
	}

	return true;
}

// �G�t�F�N�g�ݒ�
void CSoundManager::SetEffect(IXAudio2SourceVoice* target, ESoundEffectType type)
{
	
	IUnknown * lptempEffect;
	
	// �G�t�F�N�g����
	switch ( type )
	{
	case SET_REVERB:	// ���o�[�u�G�t�F�N�g
		XAudio2CreateReverb( &lptempEffect );
		break;

	case 1:	// �{�����[�����[�^�[
		XAudio2CreateVolumeMeter( &lptempEffect );
		break;

	default:
		ErrorMessageBox( "�����ȃT�E���h�G�t�F�N�g�^�C�v�l" );
		return;
		break;
	}
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = true;
	desc.OutputChannels = 1;
	desc.pEffect = lptempEffect;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &desc;

	// �G�t�F�N�g�ݒ�
	target->SetEffectChain( &chain );
	lptempEffect->Release();
}

// wav�t�@�C���ǂݍ���(�}���`�o�C�g)
bool CSoundManager::LoadWaveFile(LPSTR file)
{
	// �܂��͊��ɓo�^����Ă��Ȃ�������
	SOUNDMAPA::iterator it = m_SoundMapA.find( file );

	// �o�^����Ă��Ȃ���Γǂݍ���
	if( it == m_SoundMapA.end() )
	{
		MMCKINFO	riffckInfo;								// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
		MMCKINFO	ckInfo;									// �`�����N���
		SPSOUNDDATA	sptempSoundData( AKINEW SOUNDDATA() );	// �T�E���h�f�[�^
		
		PCMWAVEFORMAT pcmWaveForm;

		// WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
		HMMIO hmmio = mmioOpenA( file, nullptr, MMIO_ALLOCBUF | MMIO_READ );

		// �t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
		mmioDescend( hmmio, &riffckInfo, nullptr, 0 );

		// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );

		//�t�H�[�}�b�g��ǂݍ���
		mmioRead( hmmio, reinterpret_cast<HPSTR>( &pcmWaveForm ), sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData = std::shared_ptr<WAVEFORMATEX>( AKINEW WAVEFORMATEX );
		memcpy( sptempSoundData->spSoundData.get(), &pcmWaveForm, sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData->cbSize = 0;
		mmioAscend( hmmio, &ckInfo, 0 );

		// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
		ckInfo.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
		sptempSoundData->Size		= ckInfo.cksize;
		sptempSoundData->spBuffer	= std::shared_ptr<BYTE>( AKINEW BYTE[ckInfo.cksize] );
		mmioRead( hmmio, reinterpret_cast<HPSTR>( sptempSoundData->spBuffer.get() ), sptempSoundData->Size );
		
		//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
		if( FAILED( m_lpAudioEngine->CreateSourceVoice( &sptempSoundData->lpSourceVoice, sptempSoundData->spSoundData.get() ) ) )
		{
			std::string errorMessage = "�T�E���h�f�[�^�쐬���s\n�t�@�C����:";
			errorMessage += file;

			ErrorMessageBox( errorMessage.c_str() );

			return false;
		}

		// �}�b�v�ɓo�^
		SOUNDDATALIST tempList;
		tempList.push_back( sptempSoundData );
		m_SoundMapA.insert( SOUNDMAPAIRA( file, tempList ) );
	}

	return true;
}

// �T�E���h�Đ��iUnicode�j
void CSoundManager::Play( LPCWSTR file )
{
	SOUNDMAPW::iterator it = m_SoundMapW.find( file );

	if( it != m_SoundMapW.end() )
	{
		XAUDIO2_BUFFER			buffer	= { 0 };
		XAUDIO2_VOICE_STATE		state	= { 0 };
		SOUNDDATALIST::iterator listIt	= it->second.begin();
		bool					isPlay	= false;

		// �o�b�t�@�Ƀf�[�^��n��
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.pAudioData	= it->second.front()->spBuffer.get();
		buffer.AudioBytes	= it->second.front()->Size;

		// ���X�g�̒��ɍĐ��ł�����̂��Ȃ����T��
		while( listIt != it->second.end() )
		{
			// ��Ԃ𒲂ׂ�
			(*listIt)->lpSourceVoice->GetState( &state );

			// �L���[�ɂ��܂��Ă��Ȃ��Ȃ�Đ�����Ă��Ȃ�
			if( state.BuffersQueued == 0 )
			{
				if( FAILED( (*listIt)->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				{
					ErrorMessageBox( "�\�[�X�{�C�X�ɃT�u�~�b�g���s" );
					return;
				}

				// �Đ�
				(*listIt)->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
				isPlay = true;

				break;
			}
			++listIt;
		}

		// �Đ��ł��Ȃ���΃��X�g�ɐV�����}�����čĐ�
		if( isPlay == false )
		{
			// �}�X�^�[�f�[�^�̃R�s�[������ĐV�������
			SPSOUNDDATA tempData( AKINEW SOUNDDATA( *it->second.front() ) );

			if( FAILED( m_lpAudioEngine->CreateSourceVoice( &tempData->lpSourceVoice, tempData->spSoundData.get() ) ) )
			{
				std::wstring errorMessage = L"�T�E���h�f�[�^�������s\n�t�@�C����:";
				errorMessage += file;

				ErrorMessageBox( errorMessage.c_str() );
				return;
			}

			if( FAILED( tempData->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
			{
				ErrorMessageBox( L"�\�[�X�{�C�X�ɃT�u�~�b�g���s" );
				return;
			}

			tempData->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
		}
	}
}

// �T�E���h�Đ��i�}���`�o�C�g�j
void CSoundManager::Play( LPCSTR file )
{
	SOUNDMAPA::iterator it = m_SoundMapA.find( file );

	if( it != m_SoundMapA.end() )
	{
		XAUDIO2_BUFFER			buffer	= { 0 };
		XAUDIO2_VOICE_STATE		state	= { 0 };
		SOUNDDATALIST::iterator listIt	= it->second.begin();
		bool					isPlay	= false;

		// �o�b�t�@�Ƀf�[�^��n��
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.pAudioData	= it->second.front()->spBuffer.get();
		buffer.AudioBytes	= it->second.front()->Size;

		// ���X�g�̒��ɍĐ��ł�����̂��Ȃ����T��
		while( listIt != it->second.end() )
		{
			// ��Ԃ𒲂ׂ�
			(*listIt)->lpSourceVoice->GetState( &state );

			// �L���[�ɂ��܂��Ă��Ȃ��Ȃ�Đ�����Ă��Ȃ�
			if( state.BuffersQueued == 0 )
			{
				if( FAILED( (*listIt)->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				{
					ErrorMessageBox( "�\�[�X�{�C�X�ɃT�u�~�b�g���s" );
					return;
				}

				// �Đ�
				(*listIt)->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
				isPlay = true;

				break;
			}
			++listIt;
		}

		// �Đ��ł��Ȃ���΃��X�g�ɐV�����}�����čĐ�
		if( isPlay == false )
		{
			// �}�X�^�[�f�[�^�̃R�s�[������ĐV�������
			SPSOUNDDATA tempData( AKINEW SOUNDDATA( *it->second.front() ) );

			if( FAILED( m_lpAudioEngine->CreateSourceVoice( &tempData->lpSourceVoice, tempData->spSoundData.get() ) ) )
			{
				std::string errorMessage = "�T�E���h�f�[�^�������s\n�t�@�C����:";
				errorMessage += file;

				ErrorMessageBox( errorMessage.c_str() );
				return;
			}

 			if( FAILED( tempData->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
			{
				ErrorMessageBox( "�\�[�X�{�C�X�ɃT�u�~�b�g���s" );
				return;
			}

			tempData->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
		}
	}
}