//==============================================================================
/// @file	Texture.h
/// @brief	テクスチャ関連
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_TEXTURE_H__
#define __AKILIB_TEXTURE_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	テクスチャクラス(レンダーターゲット作成機能付き)
	//==============================================================================
	class CTexture : private CNoCopyable
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CTexture();

		/// @brief	デストラクタ
		~CTexture();

		/// @brief	ファイルからテクスチャを作成する
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	lpcontext	デバイスコンテキスト
		/// @param[in]	filename	読み込むファイル名
		/// @param[in]	filter		フィルターモード(デフォルトは線形補間)
		/// @param[in]	addressMode	アドレッシングモード(デフォルトはラッピングモード)
		/// @retval true  成功
		/// @retval false 失敗
		bool CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCSTR						filename,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	ファイルからテクスチャを作成する
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	lpcontext	デバイスコンテキスト
		/// @param[in]	filename	読み込むファイル名
		/// @param[in]	filter		フィルターモード(デフォルトは線形補間)
		/// @param[in]	addressMode	アドレッシングモード(デフォルトはラッピングモード)
		/// @retval true  成功
		/// @retval false 失敗
		bool CreateTextureFromFile( 
			ID3D11Device1*				lpdevice,
			ID3D11DeviceContext1*		lpcontext,
			LPCWSTR						filename,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	レンダーターゲット作成
		/// @param[in]	lpdevice	デバイス
		/// @param[in]	width		幅
		/// @param[in]	height		高さ
		/// @param[in]	format		テクスチャのフォーマット
		/// @param[in]	filter		フィルターモード(デフォルトは線形補間)
		/// @param[in]	addressMode	アドレッシングモード(デフォルトはラッピングモード)
		/// @retval true  成功
		/// @retval false 失敗
		bool CreateRenderTarget( 
			ID3D11Device1*				lpdevice,
			UINT						width,
			UINT						height,
			DXGI_FORMAT					format = DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_FILTER				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE	addressMode = D3D11_TEXTURE_ADDRESS_WRAP );

		/// @brief	テクスチャを設定する
		/// @param[in]	lpdeviceContext	デバイスコンテキスト
		/// @param[in]	slot			設定するスロット
		void SetTexture( ID3D11DeviceContext1* lpdeviceContext, UINT slot );

		/// @brief	レンダ－ターゲットビューを獲得
		/// @return	レンダーターゲットビューを返す
		inline ID3D11RenderTargetView* GetRenederTargetView() const
		{
			return m_lpRenderTargetView;
		}

		/// @brief	シェーダーリソースビューを獲得
		/// @return シェーダーリソースビューを返す
		inline ID3D11ShaderResourceView* const * GetShaderResourceView() const
		{
			return &m_lpTextureView;
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		ID3D11Resource*				m_lpTextureResource;	///< シェーダリソース
		ID3D11SamplerState*			m_lpSamplerState;		///< サンプラステート
		ID3D11Texture2D*			m_lpTextuer;			///< テクスチャ
		ID3D11ShaderResourceView*	m_lpTextureView;		///< シェーダーリソースビュー
		ID3D11RenderTargetView*		m_lpRenderTargetView;	///< レンダ－ターゲットビュー
	};
}

#endif // #ifndef __AKILIB_TEXTURE_H__