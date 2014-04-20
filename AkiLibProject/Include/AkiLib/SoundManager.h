//==============================================================================
/// @file	SoundManager.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_SOUNDMANAGER_H__
#define __AKILIB_SOUNDMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	サウンドデータ構造体
	//==============================================================================
	struct SOUNDDATA
	{
		IXAudio2SourceVoice*			lpSourceVoice;	///< セカンダリバッファ
		std::shared_ptr<BYTE>			spBuffer;		///< 波形データ
		DWORD							Size;			///< 波形データのサイズ
		std::shared_ptr<WAVEFORMATEX>	spSoundData;	///< サウンドデータ
				
		/// @brief	コンストラクタ
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
	/// @brief	サウンドマネージャー（シングルトン）
	//==============================================================================
	class CSoundManager : public CSingletonTemplate<CSoundManager>
	{
	public:
		//==============================================================================
		// フレンドクラス指定
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// 定数
		//==============================================================================
		/// @brief	エフェクトタイプ
		enum ESoundEffectType
		{
			SET_REVERB,			///< リバーブエフェクト
			SET_VOLUMEMETER,	///< ボリュームメーターエフェクト
			SET_MAX,			///< 最大数
		};

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	初期化
		/// @retval	true	成功
		/// @retval	false	失敗
		bool Init();

		/// @brief	解放
		void Release();

		/// @brief	wavファイル読み込み(Unicode用)
		/// @param[in]	読み込むwavファイル名
		/// @retval true	成功
		/// @retval false	失敗
		bool LoadWaveFile( LPWSTR file );

		/// @brief	wavファイル読み込み(マルチバイト用)
		/// @param[in]	読み込むwavファイル名
		/// @retval true	成功
		/// @retval false	失敗
		bool LoadWaveFile( LPSTR file );

		/// @brief	再生（Unicode用）
		void Play( LPCWSTR file );

		/// @brief	再生（マルチバイト用）
		void Play( LPCSTR file );

		/// @brief	エフェクト設定
		void SetEffect( IXAudio2SourceVoice* target, ESoundEffectType type );

	private:
		//==============================================================================
		// 型宣言
		//==============================================================================
		typedef std::list<SPSOUNDDATA>					SOUNDDATALIST;	///< サウンドデータリスト型
		typedef std::map<std::wstring, SOUNDDATALIST>	SOUNDMAPW;		///< サウンドデータマップ(Unicode用)
		typedef std::pair<std::wstring, SOUNDDATALIST>	SOUNDMAPAIRW;	///< サウンドデータ要素(Unicode用)
		typedef std::map<std::string, SOUNDDATALIST>	SOUNDMAPA;		///< サウンドデータマップ(マルチバイト用)
		typedef std::pair<std::string, SOUNDDATALIST>	SOUNDMAPAIRA;	///< サウンドデータ要素(マルチバイト用)

		//==============================================================================
		// フィールド
		//==============================================================================
		IXAudio2*				m_lpAudioEngine;	///< オーディオエンジンインターフェイス
		IXAudio2MasteringVoice*	m_lpMasterVoice;	///< マスターボイス(プライマリーバッファ)
		SOUNDMAPW				m_SoundMapW;		///< サウンドマップ
		SOUNDMAPA				m_SoundMapA;		///< サウンドマップ

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	コンストラクタ
		CSoundManager();

		/// @brief	デストラクタ
		~CSoundManager();
	};
}

#define SOUNDMGR	AkiLib::CSoundManager::GetInstance()

#endif // #define __AKILIB_SOUNDMANAGER_H__