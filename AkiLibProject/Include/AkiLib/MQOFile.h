//==============================================================================
/// @file	MQOFile.h
/// @brief	Metasequoiaファイル読み込み
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_MQOFILE_H__
#define __AKILIB_MQOFILE_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	Metasequoiaファイル読み込みクラス
	//==============================================================================
	class CMQOFile : public CModel
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CMQOFile();

		/// @brief	デストラクタ
		~CMQOFile();

		/// @brief	読み込み
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	file	ファイル名
		/// @retval true  成功
		/// @retval false 失敗
		bool LoadFile( ID3D11Device1* lpdevice, LPCSTR file );
	};	
}

#endif	// #define __AKILIB_MQOFILE_H__