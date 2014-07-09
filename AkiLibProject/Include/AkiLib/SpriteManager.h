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
	class SpriteInfo
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		SpriteInfo()
			:pos(0, 0)
			,WH(100, 100)
			,scale(1)
			,rotate(0)
			,color(1, 1, 1, 1)
			,drawTexXY(0, 0)
			,drawTexWH(0, 0)
			,lpTexture(nullptr)
			,lpSampler(nullptr)
		{
		}

		/// @brief	テクスチャと描画矩形を設定する
		/// @param[in]	lptex		テクスチャ
		/// @param[in]	lpsampler	サンプラ
		inline void SetTextureAndDrawSize(AkiLib::CTexture* lptex, AkiLib::CSamplerState* lpsampler)
		{
			lpTexture = lptex;
			lpSampler = lpsampler;

			if(lpTexture)
			{
				// 描画幅高さはテクスチャtろ同じサイズにする
				WH.x = lpTexture->GetTexture2DDesc().Width;
				WH.y = lpTexture->GetTexture2DDesc().Height;

				// 画像の矩形も設定する
				drawTexXY = XMFLOAT2(0, 0);
				drawTexWH = XMFLOAT2(lpTexture->GetTexture2DDesc().Width, lpTexture->GetTexture2DDesc().Height);
			}
		}

		//==============================================================================
		// フィールド
		//==============================================================================
		XMFLOAT2				pos;		//! ポリゴン動かすための座標
		XMFLOAT2				WH;			//! 描画する幅高さ
		float					scale;		//! ポリゴンの大きさ
		float					rotate;		//! ポリゴンの回転
		XMFLOAT4				color;		//! カラー
		XMFLOAT2				drawTexXY;	//! 描画する画像の位置
		XMFLOAT2				drawTexWH;	//! 描画する画像の幅高さ
		AkiLib::CTexture*		lpTexture;	//! テクスチャ
		AkiLib::CSamplerState*	lpSampler;	//! サンプラステート
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

		/// @brief	描画(実際にはキューに積むだけ)
		/// @param[in]	info 描画情報
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