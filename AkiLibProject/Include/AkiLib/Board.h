//==============================================================================
/// @file	Board.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_BOARD_H__
#define __AKILIB_BOARD_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief 板ポリゴンクラス
	//==============================================================================
	class CBoard
	{
	public:

		//==============================================================================
		// 定数
		//==============================================================================
		static const D3D11_INPUT_ELEMENT_DESC	VERTEX_LAYOUT[];	/// @brief	頂点レイアウト
		static const BYTE						NUM_VERTEXELEMENT;	/// @brief	頂点要素数
		static const BYTE						NUM_VERTEX;			/// @brief	頂点数

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CBoard();

		/// @brief	デストラクタ
		~CBoard();

		/// @brief	初期化
		/// @param[in]	lpdevice			デバイス
		/// @retval true	成功
		/// @retval false	失敗
		bool Init( ID3D11Device1* lpdevice );

		/// @brief	解放
		void Release();

		/// @brief	描画
		/// @param[in]	lpdeviceContext	デバイスコンテキスト
		/// @param[in]	topology		プリミティブのトポロジ(デフォルトはトライアングルストリップ)
		void Draw( ID3D11DeviceContext1* lpdeviceContext, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	private:
		//==============================================================================
		// 型宣言
		//==============================================================================
		/// @brief	頂点構造体
		struct VERTEX
		{
			XMFLOAT3	Pos;	///< 位置
			XMFLOAT4	Color;	///< 色
			XMFLOAT2	UV;		///< UV座標
		};

		//==============================================================================
		// フィールド
		//==============================================================================
		VERTEX			m_Vertex[4];		///< 頂点情報
		ID3D11Buffer*	m_lpVertexBuffer;	///< 頂点バッファ
	};

}

#endif	// #define __AKILIB_BOARD_H__