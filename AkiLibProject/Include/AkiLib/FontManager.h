//==============================================================================
/// @file	FontManager.h
/// @brief	フォント関連
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_FONTMANAGER_H__
#define __AKILIB_FONTMANAGER_H__

namespace AkiLib
{
	/// @brief	1文字分のデータ
	struct CHARACTORDATA
	{
		ID3D11Texture2D*			lpTexture;		///< テクスチャ
		ID3D11ShaderResourceView*	lpResourceView;	///< リソースビュー
		XMFLOAT2					Size;			///< フォントの大きさ

		CHARACTORDATA():
			lpTexture		( nullptr ),
			lpResourceView	( nullptr ),
			Size			( 0, 0 )
		{
		}
	};
	typedef std::shared_ptr<CHARACTORDATA>		SPCHARDATA;		///< 文字データ構造体スマートポインタ型

	//==============================================================================
	/// @brief	フォントマネージャークラス(シングルトン)
	//==============================================================================
	class CFontManager : public CSingletonTemplate<CFontManager>
	{
	public:
		//==============================================================================
		// フレンド指定
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// 型宣言
		//==============================================================================
		typedef std::map<std::wstring, SPCHARDATA>	FONTMAPW;		///< 文字データマップ型(Unicode用)
		typedef std::pair<std::wstring, SPCHARDATA>	FONTMAPPAIRW;	///< 文字データマップ要素型
		typedef std::map<std::string, SPCHARDATA>	FONTMAPA;		///< 文字データマップ型(Unicode用)
		typedef std::pair<std::string, SPCHARDATA>	FONTMAPPAIRA;	///< 文字データマップ要素型

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CFontManager();

		/// @brief	デストラクタ
		~CFontManager();

		/// @brief	指定文字テクスチャ生成(Unicode用)
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	lpcontext	デバイスコンテキスト
		/// @param[in]	charactor	作成する文字
		/// @return	1文字分のテクスチャデータを含んだスマートポインタ
		SPCHARDATA CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::wstring& charactor, UINT fontsize );

		/// @brief	指定文字テクスチャ生成(マルチバイト用)
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	lpcontext	デバイスコンテキスト
		/// @param[in]	charactor	作成する文字
		/// @return	1文字分のテクスチャデータを含んだスマートポインタ
		SPCHARDATA CreateFontTexture( ID3D11Device1* lpdevice, ID3D11DeviceContext1* lpcontext, const std::string& charactor, UINT fontsize );

	private:
		FONTMAPW	m_FontMapW;	///< フォントマップ(Unicode用)
		FONTMAPA	m_FontMapA;	///< フォントマップ(マルチバイト用)
	};
}

#define AKIFONTMGR	AkiLib::CFontManager::GetInstance()

#endif // #ifndef __AKILIB_FONTMANAGER_H__