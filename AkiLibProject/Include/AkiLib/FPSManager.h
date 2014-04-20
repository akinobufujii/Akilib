//==============================================================================
/// @file	FPSManager.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_FPSMANAGER__
#define __AKILIB_FPSMANAGER__

namespace AkiLib
{
	//==============================================================================
	/// @brief	FPS管理クラス
	//==============================================================================
	class CFPSManager
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		/// @param[in]	fps	FPSレート
		CFPSManager( const UINT fps );

		/// @brief	デストラクタ
		~CFPSManager(){}

		/// @brief	更新
		void Update();

		/// @brief	指定FPSよりも遅いかどうかを判定する
		/// @retval true  遅い
		/// @retval false 遅くない
		inline bool IsSlow() const
		{
			return m_SlowFlg;
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		const USHORT			m_FPS;				///< FPSレート
		const double			m_FRAME_SECOND;		///< 指定FPSに対する時間
		int						m_Count;			///< カウンタ
		int						m_CountTime;		///< フレーム時間保存用
		int						m_SaveTime;			///< 前のフレームの時間保存用
		std::shared_ptr<int>	m_spFlameAvgArray;	///< 1周平均用配列
		double					m_Avg;				///< 平均
		bool					m_SlowFlg;			///< 遅いかどうか
	};

};

#endif // !__AKILIB_FPSMANAGER__