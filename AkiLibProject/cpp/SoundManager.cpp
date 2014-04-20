//==============================================================================
/// @file	SoundManager.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CSoundManager::CSoundManager():
	m_lpAudioEngine( nullptr ),
	m_lpMasterVoice( nullptr )
{
}

// デストラクタ
CSoundManager::~CSoundManager()
{
	Release();
}

// 初期化
bool CSoundManager::Init()
{
	try
	{
		// COM初期化
		if( FAILED( CoInitializeEx( nullptr, COINIT_MULTITHREADED ) ) )
		{
			throw "COM初期化失敗";
		}

		// オーディオエンジン初期化
		if( FAILED( XAudio2Create( &m_lpAudioEngine ) ) )
		{
			throw "オーディオエンジン作成失敗";
		}

		// マスターボイス作成
		if( FAILED( m_lpAudioEngine->CreateMasteringVoice( &m_lpMasterVoice ) ) )
		{
			throw "マスターボイス作成失敗";
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

// 解放
void CSoundManager::Release()
{
	m_SoundMapW.clear();
	m_SoundMapA.clear();
	SafeRelease( m_lpAudioEngine );
	CoUninitialize();
}

// wavファイル読み込み(Unicode)
bool CSoundManager::LoadWaveFile(LPWSTR file)
{
	// まずは既に登録されていないか見る
	SOUNDMAPW::iterator it = m_SoundMapW.find( file );

	// 登録されていなければ読み込む
	if( it == m_SoundMapW.end() )
	{
		MMCKINFO	riffckInfo;							// 最上部チャンク（RIFFチャンク）保存用
		MMCKINFO	ckInfo;								// チャンク情報
		SPSOUNDDATA	sptempSoundData( AKINEW SOUNDDATA() );	// サウンドデータ
		
		PCMWAVEFORMAT pcmWaveForm;

		// WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
		HMMIO hmmio = mmioOpenW( file, nullptr, MMIO_ALLOCBUF | MMIO_READ );

		// ファイルポインタをRIFFチャンクの先頭にセットする
		mmioDescend( hmmio, &riffckInfo, nullptr, 0 );

		// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );

		//フォーマットを読み込む
		mmioRead( hmmio, reinterpret_cast<HPSTR>( &pcmWaveForm ), sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData = std::shared_ptr<WAVEFORMATEX>( AKINEW WAVEFORMATEX );
		memcpy( sptempSoundData->spSoundData.get(), &pcmWaveForm, sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData->cbSize = 0;
		mmioAscend( hmmio, &ckInfo, 0 );

		// WAVファイル内の音データの読み込み	
		ckInfo.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
		sptempSoundData->Size		= ckInfo.cksize;
		sptempSoundData->spBuffer	= std::shared_ptr<BYTE>( AKINEW BYTE[ckInfo.cksize] );
		mmioRead( hmmio, reinterpret_cast<HPSTR>( sptempSoundData->spBuffer.get() ), sptempSoundData->Size );
		
		//ソースボイスにデータを詰め込む	
		if( FAILED( m_lpAudioEngine->CreateSourceVoice( &sptempSoundData->lpSourceVoice, sptempSoundData->spSoundData.get() ) ) )
		{
			std::wstring errorMessage = L"サウンドデータ作成失敗\nファイル名:";
			errorMessage += file;

			ErrorMessageBox( errorMessage.c_str() );

			return false;
		}

		// マップに登録
		SOUNDDATALIST tempList;
		tempList.push_back( sptempSoundData );
		m_SoundMapW.insert( SOUNDMAPAIRW( file, tempList ) );
	}

	return true;
}

// エフェクト設定
void CSoundManager::SetEffect(IXAudio2SourceVoice* target, ESoundEffectType type)
{
	
	IUnknown * lptempEffect;
	
	// エフェクト分岐
	switch ( type )
	{
	case SET_REVERB:	// リバーブエフェクト
		XAudio2CreateReverb( &lptempEffect );
		break;

	case 1:	// ボリュームメーター
		XAudio2CreateVolumeMeter( &lptempEffect );
		break;

	default:
		ErrorMessageBox( "無効なサウンドエフェクトタイプ値" );
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

	// エフェクト設定
	target->SetEffectChain( &chain );
	lptempEffect->Release();
}

// wavファイル読み込み(マルチバイト)
bool CSoundManager::LoadWaveFile(LPSTR file)
{
	// まずは既に登録されていないか見る
	SOUNDMAPA::iterator it = m_SoundMapA.find( file );

	// 登録されていなければ読み込む
	if( it == m_SoundMapA.end() )
	{
		MMCKINFO	riffckInfo;								// 最上部チャンク（RIFFチャンク）保存用
		MMCKINFO	ckInfo;									// チャンク情報
		SPSOUNDDATA	sptempSoundData( AKINEW SOUNDDATA() );	// サウンドデータ
		
		PCMWAVEFORMAT pcmWaveForm;

		// WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
		HMMIO hmmio = mmioOpenA( file, nullptr, MMIO_ALLOCBUF | MMIO_READ );

		// ファイルポインタをRIFFチャンクの先頭にセットする
		mmioDescend( hmmio, &riffckInfo, nullptr, 0 );

		// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );

		//フォーマットを読み込む
		mmioRead( hmmio, reinterpret_cast<HPSTR>( &pcmWaveForm ), sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData = std::shared_ptr<WAVEFORMATEX>( AKINEW WAVEFORMATEX );
		memcpy( sptempSoundData->spSoundData.get(), &pcmWaveForm, sizeof( pcmWaveForm ) );
		sptempSoundData->spSoundData->cbSize = 0;
		mmioAscend( hmmio, &ckInfo, 0 );

		// WAVファイル内の音データの読み込み	
		ckInfo.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
		mmioDescend( hmmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
		sptempSoundData->Size		= ckInfo.cksize;
		sptempSoundData->spBuffer	= std::shared_ptr<BYTE>( AKINEW BYTE[ckInfo.cksize] );
		mmioRead( hmmio, reinterpret_cast<HPSTR>( sptempSoundData->spBuffer.get() ), sptempSoundData->Size );
		
		//ソースボイスにデータを詰め込む	
		if( FAILED( m_lpAudioEngine->CreateSourceVoice( &sptempSoundData->lpSourceVoice, sptempSoundData->spSoundData.get() ) ) )
		{
			std::string errorMessage = "サウンドデータ作成失敗\nファイル名:";
			errorMessage += file;

			ErrorMessageBox( errorMessage.c_str() );

			return false;
		}

		// マップに登録
		SOUNDDATALIST tempList;
		tempList.push_back( sptempSoundData );
		m_SoundMapA.insert( SOUNDMAPAIRA( file, tempList ) );
	}

	return true;
}

// サウンド再生（Unicode）
void CSoundManager::Play( LPCWSTR file )
{
	SOUNDMAPW::iterator it = m_SoundMapW.find( file );

	if( it != m_SoundMapW.end() )
	{
		XAUDIO2_BUFFER			buffer	= { 0 };
		XAUDIO2_VOICE_STATE		state	= { 0 };
		SOUNDDATALIST::iterator listIt	= it->second.begin();
		bool					isPlay	= false;

		// バッファにデータを渡す
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.pAudioData	= it->second.front()->spBuffer.get();
		buffer.AudioBytes	= it->second.front()->Size;

		// リストの中に再生できるものがないか探す
		while( listIt != it->second.end() )
		{
			// 状態を調べる
			(*listIt)->lpSourceVoice->GetState( &state );

			// キューにたまっていないなら再生されていない
			if( state.BuffersQueued == 0 )
			{
				if( FAILED( (*listIt)->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				{
					ErrorMessageBox( "ソースボイスにサブミット失敗" );
					return;
				}

				// 再生
				(*listIt)->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
				isPlay = true;

				break;
			}
			++listIt;
		}

		// 再生できなければリストに新しく挿入して再生
		if( isPlay == false )
		{
			// マスターデータのコピーを取って新しく作る
			SPSOUNDDATA tempData( AKINEW SOUNDDATA( *it->second.front() ) );

			if( FAILED( m_lpAudioEngine->CreateSourceVoice( &tempData->lpSourceVoice, tempData->spSoundData.get() ) ) )
			{
				std::wstring errorMessage = L"サウンドデータ複製失敗\nファイル名:";
				errorMessage += file;

				ErrorMessageBox( errorMessage.c_str() );
				return;
			}

			if( FAILED( tempData->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
			{
				ErrorMessageBox( L"ソースボイスにサブミット失敗" );
				return;
			}

			tempData->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
		}
	}
}

// サウンド再生（マルチバイト）
void CSoundManager::Play( LPCSTR file )
{
	SOUNDMAPA::iterator it = m_SoundMapA.find( file );

	if( it != m_SoundMapA.end() )
	{
		XAUDIO2_BUFFER			buffer	= { 0 };
		XAUDIO2_VOICE_STATE		state	= { 0 };
		SOUNDDATALIST::iterator listIt	= it->second.begin();
		bool					isPlay	= false;

		// バッファにデータを渡す
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.pAudioData	= it->second.front()->spBuffer.get();
		buffer.AudioBytes	= it->second.front()->Size;

		// リストの中に再生できるものがないか探す
		while( listIt != it->second.end() )
		{
			// 状態を調べる
			(*listIt)->lpSourceVoice->GetState( &state );

			// キューにたまっていないなら再生されていない
			if( state.BuffersQueued == 0 )
			{
				if( FAILED( (*listIt)->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				{
					ErrorMessageBox( "ソースボイスにサブミット失敗" );
					return;
				}

				// 再生
				(*listIt)->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
				isPlay = true;

				break;
			}
			++listIt;
		}

		// 再生できなければリストに新しく挿入して再生
		if( isPlay == false )
		{
			// マスターデータのコピーを取って新しく作る
			SPSOUNDDATA tempData( AKINEW SOUNDDATA( *it->second.front() ) );

			if( FAILED( m_lpAudioEngine->CreateSourceVoice( &tempData->lpSourceVoice, tempData->spSoundData.get() ) ) )
			{
				std::string errorMessage = "サウンドデータ複製失敗\nファイル名:";
				errorMessage += file;

				ErrorMessageBox( errorMessage.c_str() );
				return;
			}

 			if( FAILED( tempData->lpSourceVoice->SubmitSourceBuffer( &buffer ) ) )
			{
				ErrorMessageBox( "ソースボイスにサブミット失敗" );
				return;
			}

			tempData->lpSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
		}
	}
}