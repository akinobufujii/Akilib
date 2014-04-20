//==============================================================================
/// @file	D3D11Manager.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_D3D11MANAGER_H__
#define __AKILIB_D3D11MANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	D3D11マネージャー（シングルトン）
	//==============================================================================
	class CD3D11Manager : public CSingletonTemplate<CD3D11Manager> 
	{
	public:
		//==============================================================================
		// フレンドクラス指定
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	デストラクタ
		~CD3D11Manager();

		/// @brief 初期化
		/// @param[in]	hwnd		ウィンドウハンドル
		/// @param[in]	width		バックバッファの幅
		/// @param[in]	height		バックバッファの高さ
		/// @param[in]	iswindow	ウィンドウモード(trueでウィンドウモード)
		/// @retval true	成功
		/// @retval false	失敗
		bool Init( HWND hwnd, UINT width, UINT height, bool iswindow = true );

		/// @brief	解放
		void Release();

		/// @brief	プライマリーの画面と深度バッファクリア
		/// @param[in]	r	赤成分(0～1)
		/// @param[in]	g	緑成分(0～1)
		/// @param[in]	b	青成分(0～1)
		/// @param[in]	a	アルファ成分(0～1)
		void ClearPrimaryBuffer( float r, float g, float b, float a );

		/// @brief	指定のレンダーターゲットをクリア
		/// @param[in]	lprenderTarget	クリアするレンダーターゲット
		/// @param[in]	r				赤成分(0～1)
		/// @param[in]	g				緑成分(0～1)
		/// @param[in]	b				青成分(0～1)
		/// @param[in]	a				アルファ成分(0～1)
		void ClearRenderTarget( ID3D11RenderTargetView* lprenderTarget, float r, float g, float b, float a );

		/// @brief	指定の深度ステンシルをクリア
		/// @param[in]	lpdepthStencil	クリアするレンダーターゲット
		void ClearDepthStencil( ID3D11DepthStencilView* lpdepthStencil );

		/// @brief	画面更新
		void Flip();

		/// @brief	レンダーターゲットを戻す
		void ResetRenderTarget();

		/// @brief	レンダーターゲット設定
		/// @param[in]	lpdepthStencil	深度ステンシルビュー
		/// @param[in]	lprenderTarget0	0番目のレンダーターゲット
		/// @param[in]	lprenderTarget1	1番目のレンダーターゲット
		/// @param[in]	lprenderTarget2	2番目のレンダーターゲット
		/// @param[in]	lprenderTarget3	3番目のレンダーターゲット
		/// @param[in]	lprenderTarget4	4番目のレンダーターゲット
		/// @param[in]	lprenderTarget5	5番目のレンダーターゲット
		/// @param[in]	lprenderTarget6	6番目のレンダーターゲット
		/// @param[in]	lprenderTarget7	7番目のレンダーターゲット
		void SetRenderTargets( 
			ID3D11DepthStencilView* lpdepthStencil,
			ID3D11RenderTargetView* lprenderTarget0,
			ID3D11RenderTargetView* lprenderTarget1 = nullptr,
			ID3D11RenderTargetView* lprenderTarget2 = nullptr,
			ID3D11RenderTargetView* lprenderTarget3 = nullptr,
			ID3D11RenderTargetView* lprenderTarget4 = nullptr,
			ID3D11RenderTargetView* lprenderTarget5 = nullptr,
			ID3D11RenderTargetView* lprenderTarget6 = nullptr,
			ID3D11RenderTargetView* lprenderTarget7 = nullptr);

		/// @brief	頂点バッファ作成
		/// @param[in]	vertexData	頂点データ
		/// @param[in]	sizeVertex	頂点の大きさ
		/// @return	作成した頂点バッファ
		ID3D11Buffer* CreateVertexBuffer( void* vertexData, size_t sizeVertex );

		/// @brief	コンスタントバッファ作成
		/// @param[in]	sizeBuffer	データの大きさ
		/// @return	作成したコンスタントバッファ
		ID3D11Buffer* CreateConstantBuffer( size_t sizeBuffer );

		/// @brief	構造体バッファ作成
		/// @param[in]	sizeBuffer	データの大きさ
		/// @param[in]	stride		ストライドの大きさ
		/// @param[in]	lpdata		初期データ(NULLでも可)
		/// @return	作成した構造体バッファ
		ID3D11Buffer* CreateStructuredBuffer( size_t sizeBuffer, size_t stride, void* lpdata );

		/// @brief	コンピュートシェーダに値を送るためのビューを作成
		/// @param[in]	lpstructuredBuffer	作成済み構造体バッファ
		/// @param[in]	numElement			要素数
		/// @return		作成したシェーダーリソースビュー
		ID3D11ShaderResourceView* CreateComputeSRV( ID3D11Buffer* lpstructuredBuffer, UINT numElement );

		/// @brief	アンオーダードアクセスビューを作成
		/// @param[in]	lpstructuredBuffer	作成済み構造体バッファ
		/// @param[in]	numElement			要素数
		/// @return		作成したアンオーダードアクセスビュー
		ID3D11UnorderedAccessView* CreateUAV( ID3D11Buffer* lpstructuredBuffer, UINT numElement );

		/// @brief	コンピュートシェーダの結果を受け取る
		/// @param[in]	lpstructuredBuffer	作成済み構造体バッファ
		/// @param[in]	lpreseveBuffer		結果を受け取るためのバッファ
		/// @param[in]	size				受け取るバッファのサイズ
		void GetComputShaderResult( ID3D11Buffer* lpstructuredBuffer, void* lpreseveBuffer, size_t size );

		/// @brief	シェーダに値を渡す
		/// @param[in]	slot		渡すレジスタ
		/// @param[in]	buffer		反映させるバッファ
		/// @param[in]	lpdata		書き込むデータ
		/// @retval true  成功
		/// @retval false 失敗
		bool SendValueToShader( UINT slot, ID3D11Buffer* buffer, const void* lpdata );

		/// @brief	シェーダに値を渡す
		/// @param[in]	slot		渡すレジスタ
		/// @param[in]	buffer		反映させるバッファ
		/// @param[in]	lpdata		書き込むデータ
		/// @retval true  成功
		/// @retval false 失敗
		template <typename TYPE>
		inline bool SendValueToShader( UINT slot, const ConstantBuffer<TYPE>& buffer )
		{
			return SendValueToShader(slot, buffer.GetBufferPointer(), buffer.GetStructPointer());
		}

		/// @brief	デバイスを獲得
		/// @return	デバイスを返す
		inline ID3D11Device1* GetDevice() const
		{
			return m_lpDevice;
		}

		/// @brief	デバイスコンテキストを獲得
		/// @return	デバイスコンテキストを返す
		inline ID3D11DeviceContext1* GetDeviceContext() const
		{
			return m_lpDeviceContext;
		}

		/// @brief	深度ステンシルビューを獲得
		/// @return	深度ステンシルビューを返す
		inline ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return m_lpDepthStencilView;
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		ID3D11Device1*			m_lpDevice;				///< デバイス
		ID3D11DeviceContext1*	m_lpDeviceContext;		///< デバイスコンテキスト
		IDXGISwapChain1*		m_lpSwapChain;			///< スワップチェイン
		ID3D11RenderTargetView* m_lpBackBufferView;		///< バックバッファターゲットビュー
		ID3D11DepthStencilView* m_lpDepthStencilView;	///< 深度ステンシルバッファターゲットビュー
		ID3D11Texture2D*		m_lpDepthStencil;		///< 深度ステンシルバッファ

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	コンストラクタ
		CD3D11Manager();
	};
}

#define AKID3D11MGR AkiLib::CD3D11Manager::GetInstance()
#define AKID3D11DEVICE AkiLib::CD3D11Manager::GetInstance()->GetDevice()
#define AKID3D11CONTEXT AkiLib::CD3D11Manager::GetInstance()->GetDeviceContext()

#endif	// #define __AKILIB_D3D11MANAGER_H__