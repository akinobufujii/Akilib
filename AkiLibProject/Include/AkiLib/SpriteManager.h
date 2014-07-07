//==============================================================================
/// @file Sp@riteManager.h
/// @brief	2D描画マネージャー
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_SPRITEMANAGER_H__
#define __AKILIB_SPRITEMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	スプライトの情報(この情報を元に描画を行う)
	//==============================================================================
	struct SpriteInfo
	{
		XMFLOAT2				pos;		//! ポリゴン動かすための座標
		float					scale;		//! ポリゴンの大きさ
		float					rotate;		//! ポリゴンの回転
		AkiLib::CTexture*		lpTexture;	//! テクスチャ
		AkiLib::CSamplerState*	lpSampler;	//! サンプラステート
		
		SpriteInfo()
			:pos(0, 0)
			,scale(1)
			,rotate(0)
			,lpTexture(nullptr)
			,lpSampler(nullptr)
		{
		}
	};

	//==============================================================================
	/// @brief	スプライトマネージャー（シングルトン）
	//==============================================================================
	class CSpriteManager : public CSingletonTemplate<CSpriteManager> 
	{
	public:
		//==============================================================================
		// フレンドクラス指定
		//==============================================================================
		friend CSingletonTemplate;

		/// @brief	初期化
		/// @retval true	成功
		/// @retval false	失敗
		bool Init();

		/// @brief	解放
		void Release();

		void Draw(const SpriteInfo& info);

		/// @brief	コンストラクタすべて描画
		void Flush();
		
	private:

		//==============================================================================
		// 宣言
		//==============================================================================
		struct CBUFFER
		{
			AKIALIGN16	AkiLib::Matrix	mWVP;
		};

		//==============================================================================
		// フィールド
		//==============================================================================
		AkiLib::ConstantBuffer<CBUFFER> m_ConstantBuffer;	//! 2Dシェーダ用コンスタントバッファ
		AkiLib::CShader					m_SpriteShader;		//! スプライトシェーダ
		AkiLib::CBoard					m_Board;			//! 描画用板ポリゴン
		ID3D11InputLayout*				m_lpInputLayout;	//! 入力レイアウト
		std::queue<SpriteInfo>			m_SpriteQueue;		//! スプライトのキュー

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	コンストラクタ
		CSpriteManager();

		/// @brief	デストラクタ
		~CSpriteManager();
	};
}

#define AKISPRITEMGR AkiLib::CSpriteManager::GetInstance()

#endif	// #define __AKILIB_SPRITEMANAGER_H__