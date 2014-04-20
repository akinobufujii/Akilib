//==============================================================================
/// @file	Shader.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_SHADER_H__
#define __AKILIB_SHADER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	シェーダラッピングクラス(コピー禁止)
	//==============================================================================
	class CShader : private CNoCopyable
	{
	public:
		
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CShader();

		/// @brief	デストラクタ
		~CShader();

		/// @brief	ファイル名設定(すべてメソッドの前に呼ぶ)
		/// @param[in]	file	読み込むファイル名
		/// @param[in]	lpevice	デバイス
		inline void Init( LPCSTR file, ID3D11Device1* lpdevice )
		{
			m_FileName = file;
			m_lpDevice = lpdevice;
		}

		// 解放
		/// @brief	すべてのシェーダを開放する
		void Release();

		/// @brief	頂点シェーダー作成
		/// @param[in]	funcName		頂点シェーダー名
		/// @param[in]	version			コンパイルバージョン
		/// @param[in]	layout			頂点レイアウト(レイアウトを作成するならば必須)
		/// @param[in]	layoutNum		頂点レイアウト数(レイアウトを作成するならば必須)
		/// @param[out]	lppoutLayout	作成した頂点レイアウトを返す(レイアウトを作成するならば必須)
		/// @retval true	成功
		/// @retval false	失敗
		bool CreateVertexShader( 
			LPCSTR							funcName,
			LPCSTR							version,
			const D3D11_INPUT_ELEMENT_DESC* layout		= nullptr,
			size_t							layoutNum	= 0,
			ID3D11InputLayout**				lppoutLayout= nullptr );

		/// @brief	ピクセルシェーダー作成
		/// @param[in]	funcName		ピクセルシェーダー名
		/// @param[in]	version			コンパイルバージョン
		/// @retval true	成功
		/// @retval false	失敗
		bool CreatePixelShader( LPCSTR funcName, LPCSTR version );

		/// @brief	コンピュートシェーダー作成
		/// @param[in]	funcName		コンピュートシェーダー名
		/// @param[in]	version			コンパイルバージョン
		/// @retval true	成功
		/// @retval false	失敗
		bool CreateComputeShader( LPCSTR funcName, LPCSTR version );

		/// @brief	ハルシェーダー作成
		/// @param[in]	funcName		ハルシェーダー名
		/// @param[in]	version			コンパイルバージョン
		/// @retval true	成功
		/// @retval false	失敗
		bool CreateHallShader( LPCSTR funcName, LPCSTR version );

		/// @brief	ドメインシェーダー作成
		/// @param[in]	funcName		ドメインシェーダー名
		/// @param[in]	version			コンパイルバージョン
		/// @retval true	成功
		/// @retval false	失敗
		bool CreateDomainShader( LPCSTR funcName, LPCSTR version );

		/// @brief	頂点シェーダを獲得
		/// @param[in]	funcName	頂点シェーダ名
		/// @return	作成済みの頂点シェーダオブジェクト
		inline ID3D11VertexShader* GetVertexShader( LPCSTR funcName )
		{
			return MapGet<VERTEXSHADERMAP, ID3D11VertexShader*>( m_VertexShaderMap, funcName );
		}

		/// @brief	ピクセルシェーダを獲得
		/// @param[in]	funcName	ピクセルシェーダ名
		/// @return	作成済みのピクセルシェーダオブジェクト
		inline ID3D11PixelShader* GetPixelShader( LPCSTR funcName )
		{
			return MapGet<PIXELSHADERMAP, ID3D11PixelShader*>( m_PixelShaerMap, funcName );
		}

		/// @brief	コンピュートシェーダを獲得
		/// @param[in]	funcName	コンピュートシェーダ名
		/// @return	作成済みのコンピュートシェーダオブジェクト
		inline ID3D11ComputeShader* GetComputeShader( LPCSTR funcName )
		{
			return MapGet<COMPUTESHADERMAP, ID3D11ComputeShader*>( m_ComputeShaderMap, funcName );
		}

		/// @brief	ハルシェーダを獲得
		/// @param[in]	funcName	ハルシェーダ名
		/// @return	作成済みのハルシェーダオブジェクト
		inline ID3D11HullShader* GetHullShader( LPCSTR funcName )
		{
			return MapGet<HALLSHADERMAP, ID3D11HullShader*>( m_HallShaderMap, funcName );
		}

		/// @brief	ドメインシェーダを獲得
		/// @param[in]	funcName	ドメインシェーダ名
		/// @return	作成済みのドメインシェーダオブジェクト
		inline ID3D11DomainShader* GetDomainShader( LPCSTR funcName )
		{
			return MapGet<DOMAINSHADERMAP, ID3D11DomainShader*>( m_DomainShaderMap, funcName );
		}
		
	private:
		//==============================================================================
		// 型宣言
		//==============================================================================
		typedef std::map<std::string, ID3D11VertexShader*>		VERTEXSHADERMAP;		///< 頂点シェーダマップ
		typedef std::pair<std::string, ID3D11VertexShader*>		VERTEXSHADERMAPPAIR;	///< 頂点シェーダマップ要素
		typedef std::map<std::string, ID3D11PixelShader*>		PIXELSHADERMAP;			///< ピクセルシェーダマップ
		typedef std::pair<std::string, ID3D11PixelShader*>		PIXELSHADERMAPPAIR;		///< ピクセルシェーダマップ要素
		typedef std::map<std::string, ID3D11ComputeShader*>		COMPUTESHADERMAP;		///< コンピュートシェーダマップ
		typedef std::pair<std::string, ID3D11ComputeShader*>	COMPUTESHADERMAPPAIR;	///< コンピュートシェーダマップ要素
		typedef std::map<std::string, ID3D11HullShader*>		HALLSHADERMAP;			///< ハルシェーダマップ
		typedef std::pair<std::string, ID3D11HullShader*>		HALLSHADERMAPPAIR;		///< ハルシェーダマップ要素
		typedef std::map<std::string, ID3D11DomainShader*>		DOMAINSHADERMAP;		///< ドメインシェーダマップ
		typedef std::pair<std::string, ID3D11DomainShader*>		DOMAINSHADERMAPPAIR;	///< ドメインシェーダマップ要素

		//==============================================================================
		// フィールド
		//==============================================================================
		std::string		m_FileName;		///< ファイル名
		ID3D11Device1*	m_lpDevice;		///< デバイス

		VERTEXSHADERMAP		m_VertexShaderMap;	///< 頂点シェーダマップ
		PIXELSHADERMAP		m_PixelShaerMap;	///< ピクセルシェーダマップ
		COMPUTESHADERMAP	m_ComputeShaderMap;	///< コンピュートシェーダマップ
		HALLSHADERMAP		m_HallShaderMap;	///< ハルシェーダマップ
		DOMAINSHADERMAP		m_DomainShaderMap;	///< ドメインシェーダマップ

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	マップ内容を削除する
		/// @param[in]	obj	マップ内容
		template<typename TYPE>
		inline static void MapRelease( TYPE obj )
		{
			SafeRelease( obj.second );
		}

		template<typename MAP, typename ELEMENT>
		inline ELEMENT MapGet( MAP map, LPCSTR name )
		{
			MAP::iterator it = map.find( name );

			if( it != map.end() )
			{
				return it->second;
			}

			return nullptr;
		}

		/// @brief	ファイルから読み込んでコンパイル
		/// @param[in]	funcName	HLSL関数名
		/// @param[in]	version		コンパイルバージョン
		/// @return		コンパイル済みのブロブ
		ID3DBlob* CompileShaderFromFile( LPCSTR funcName, LPCSTR version );
	};
}

#endif	// !__AKILIB_SHADER_H__