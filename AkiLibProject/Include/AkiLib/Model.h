//==============================================================================
/// @file	Model.h
/// @brief	モデル基底クラス
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_MODEL_H__
#define __AKILIB_MODEL_H__

namespace AkiLib
{
	// メッシュの頂点情報
	struct MESHVERTEX
	{
		Vector3		Pos;	///< 頂点位置
		Vector3		Normal;	///< 法線
		XMFLOAT2	UV;		///< テクスチャ座標
	};

	// 面
	struct FACE
	{
		std::vector<UINT>		IndexArray;	///< 添字配列
		UINT					Material;	///< 参照材質番号
		std::vector<XMFLOAT2>	UV;			///< UV情報
	};

	// マテリアル
	struct MATERIAL
	{
		std::string	Name;		///< マテリアル名
		XMFLOAT4	Color;		///< 色
		float		Diffuse;	///< 拡散光
		float		Ambient;	///< 環境光
		float		Emissive;	///< 自己照明
		float		Specular;	///< 反射光
		float		Power;		///< 反射の強さ
		std::string	TexName;	///< テクスチャ名
		CTexture*	lpTexture;	///< テクスチャ
	};

	//==============================================================================
	/// @brief	モデルクラス(データ管理のみ)
	//==============================================================================
	class CModel
	{
	public:
		//==============================================================================
		// 定数
		//==============================================================================
		static const D3D11_INPUT_ELEMENT_DESC	VERTEX_LAYOUT[];	/// 頂点レイアウト

		//==============================================================================
		// 型宣言
		//==============================================================================
		struct MeshObject
		{
			std::vector<MESHVERTEX>	Vertex;		///< 頂点情報
			std::vector<FACE>		Face;		///< 面情報
		};

		struct BUFFER
		{
			ID3D11Buffer*	lpIndex;	///< インデックスバッファ
			ID3D11Buffer*	lpVertex;	///< 頂点バッファ
			UINT			NumIndex;	///< インデックスの数
		};

		typedef std::map<std::string, MeshObject>		MESHOBJECTMAP;
		typedef std::pair<std::string, MeshObject>		MESHOBJECTMAPPAIR;
		typedef std::vector<MATERIAL>					MATERIALARRAY;
		typedef std::vector<BUFFER>						BUFFERARRAY;

		//==============================================================================
		// フィールド
		//==============================================================================
		MESHOBJECTMAP		m_MeshObjectMap;	///< メッシュオブジェクトマップ
		MATERIALARRAY		m_MaterialArray;	///< 材質配列
		BUFFERARRAY			m_BufferArray;		///< バッファ配列(オブジェクト数分できる)
		
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CModel();

		/// @brief	デストラクタ
		virtual ~CModel();

		/// @brief	メッシュのバッファ作成
		/// @param[in]	lpdeviceContext	デバイスコンテキスト
		void CreateMeshBuffer(ID3D11Device1* lpdevice );

		/// @brief	描画
		/// @param[in]	lpdeviceContext	デバイスコンテキスト
		/// @param[in]	slot			設定するスロット(デフォルトは0)
		virtual void Draw(ID3D11DeviceContext1* lpdeviceContex, UINT slot = 0);
	};	
}

#endif	// #define __AKILIB_MODEL_H__