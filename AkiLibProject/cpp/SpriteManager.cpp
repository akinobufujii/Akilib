//==============================================================================
/// @file	D3D11Manager.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CSpriteManager::CSpriteManager()
{

}

// デストラクタ
CSpriteManager::~CSpriteManager()
{
	Release();
}

// 初期化
bool CSpriteManager::Init()
{
	// コンスタントバッファ作成
	m_ConstantBuffer.Init(AKID3D11DEVICE);

	// シェーダ読み込み
	m_SpriteShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	m_SpriteShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &m_lpInputLayout );
	m_SpriteShader.CreatePixelShader( "PS", "ps_5_0" );
	m_SpriteShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// 板ポリ
	m_Board.Init(AKID3D11DEVICE);

	return true;
}

// 解放
void CSpriteManager::Release()
{
	m_ConstantBuffer.Release();
}

void CSpriteManager::Draw(const SpriteInfo& info)
{
	m_SpriteQueue.push(info);
}

// すべて描画
void CSpriteManager::Flush()
{
	AkiLib::Matrix mProj, mBaseTrans;

	// ビューポートから画面の幅高さを取得
	D3D11_VIEWPORT vp;
	UINT vpSlot = 1;
	AKID3D11CONTEXT->RSGetViewports(&vpSlot, &vp);

	// 2D描画する投影行列設定
	mProj.m[0][0] = 2 / static_cast<float>(vp.Width);
	mProj.m[1][1] = -2 / static_cast<float>(vp.Height);
	mProj.m[3][0] = -1;
	mProj.m[3][1] = 1;

	// 大きさ2の板ポリゴンなので半分に縮小して上下0.5ずつ動かす
	mBaseTrans.m[0][0] = mBaseTrans.m[3][0] = mBaseTrans.m[3][1] = 0.5f;
	mBaseTrans.m[1][1] = -0.5f;

	while(m_SpriteQueue.size())
	{
		SpriteInfo& info = m_SpriteQueue.front();

		// シェーダ値を渡す
		AkiLib::Matrix mScale, mTrans, mRotate;
		
		// 行列設定
		// 好きなように拡大と移動をさせる
		mScale.SetWorldScaling(info.scale);
		mTrans.SetWorldTrans(XMFLOAT3(info.pos.x, info.pos.y, 0));
		mRotate.SetWorldRotateZ(info.rotate);

		// 値を渡す
		m_ConstantBuffer.mWVP = XMMatrixTranspose( (mRotate * mBaseTrans * mScale * mTrans * mProj) );
		AKID3D11MGR->SendValueToShader(0, m_ConstantBuffer);

		// 入力レイアウト設定
		AKID3D11CONTEXT->IASetInputLayout( m_lpInputLayout );

		// シェーダ設定
		AKID3D11CONTEXT->VSSetShader( m_SpriteShader.GetVertexShader( "VS" ), nullptr, 0 );

		if(info.lpSampler && info.lpTexture)
		{
			// シェーダーリソース設定
			info.lpTexture->SetTexture(AKID3D11CONTEXT, 0);

			// サンプラステート設定
			info.lpSampler->SetSamplerState(AKID3D11CONTEXT, 0);

			AKID3D11CONTEXT->PSSetShader( m_SpriteShader.GetPixelShader( "PSTex" ), nullptr, 0 );
		}
		else
		{
			AKID3D11CONTEXT->PSSetShader( m_SpriteShader.GetPixelShader( "PS" ), nullptr, 0 );
		}

		// 板描画
		m_Board.Draw(AKID3D11CONTEXT);

		m_SpriteQueue.pop();
	}
}