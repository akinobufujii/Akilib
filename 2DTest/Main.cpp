//==============================================================================
// インクルード
//==============================================================================
#include "AkiLib.h"
#include <iostream>

//==============================================================================
// グローバル定数
//==============================================================================
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//==============================================================================
// グローバル変数
//==============================================================================
AkiLib::CFPSManager	g_FPS(60);			// FPS制御
AkiLib::CBoard		g_Board;			// 板ポリ
AkiLib::CShader		g_TestShader;		// シェーダ
ID3D11InputLayout*	g_lpInputLayout;	// 入力レイアウト


XMFLOAT2	g_PolygonPos(0, 0);			// ポリゴン動かすための座標
float		g_PolygonScale = 150;		// ポリゴンの大きさ
float		g_PolygonRotate = 0;		// ポリゴンの回転
AkiLib::CTexture		g_Texture;			// テクスチャ
AkiLib::CSamplerState	g_Sampler;			// サンプラステート

AkiLib::SpriteInfo g_SpriteInfo;	// 2Dスプライト情報

// コンスタントバッファは16バイト境界でアラインメント
struct CBUFFER
{
	AKIALIGN16	AkiLib::Matrix	mWVP;
};

AkiLib::ConstantBuffer<CBUFFER> g_ConstantBuffer;

//==============================================================================
// 実行関数
//==============================================================================
void Update()
{
	// FPS制御
	g_FPS.Update();

	// キー情報更新
	AKIINPUTMGR->UpdateState();

	// ポリゴン座標更新
	static const float MOVE_SPEED = 1;
	if(AKIINPUTMGR->IsKeyboardState(DIK_UP, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonPos.y -= MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_DOWN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonPos.y += MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_RIGHT, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonPos.x += MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_LEFT, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonPos.x -= MOVE_SPEED;
	}

	// ポリゴンスケール
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGUP, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonScale += 1;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGDN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonScale -= 1;
	}

	// ポリゴン回転
	if(AKIINPUTMGR->IsKeyboardState(DIK_Z, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonRotate = fmodf(g_PolygonRotate + 1, 360.f);
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_X, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		if((g_PolygonRotate -= 1) < 0)
		{
			g_PolygonRotate = 359.f;
		}
	}

	// ポリゴンカラー変更
	g_Board.SetColor(AkiLib::ColorRGBA(0xff, 0xff, 0xff, 0xff));

	// スプライト
	g_SpriteInfo.SetTextureAndDrawSize(&g_Texture, &g_Sampler);
	//g_SpriteInfo.drawTexXY.x = 256.f;
	//g_SpriteInfo.drawWH.x *= 0.5f;
}

void Draw()
{
	// プライマリーバッファクリア
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// シェーダ値を渡す
	AkiLib::Matrix mScale, mTrans, mRotate;
	AkiLib::Matrix mProj, mBaseTrans;

	// 行列設定
	// 2D描画する投影行列設定
	mProj.m[0][0] = 2 / static_cast<float>(SCREEN_WIDTH);
	mProj.m[1][1] = -2 / static_cast<float>(SCREEN_HEIGHT);
	mProj.m[3][0] = -1;
	mProj.m[3][1] = 1;

	// 大きさ2の板ポリゴンなので半分に縮小して上下0.5ずつ動かす
	mBaseTrans.m[0][0] = mBaseTrans.m[3][0] = mBaseTrans.m[3][1] = 0.5f;
	mBaseTrans.m[1][1] = -0.5f;

	// 好きなように拡大と移動をさせる
	mScale.SetWorldScaling(g_PolygonScale);
	mTrans.SetWorldTrans(XMFLOAT3(g_PolygonPos.x, g_PolygonPos.y, 0));
	mRotate.SetWorldRotateZ(g_PolygonRotate);

	// 値を渡す
	g_ConstantBuffer.mWVP = XMMatrixTranspose( (mRotate * mBaseTrans * mScale * mTrans * mProj) );
	AKID3D11MGR->SendValueToShader(0, g_ConstantBuffer);

	// 入力レイアウト設定
	AKID3D11CONTEXT->IASetInputLayout( g_lpInputLayout );

	// シェーダ設定
	AKID3D11CONTEXT->VSSetShader( g_TestShader.GetVertexShader( "VS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PSTex" ), nullptr, 0 );

	// シェーダーリソース設定
	g_Texture.SetTexture(AKID3D11CONTEXT, 0);

	// サンプラステート設定
	g_Sampler.SetSamplerState(AKID3D11CONTEXT, 0);

	// 板描画
	g_Board.Draw(AKID3D11CONTEXT);

	// スプライト描画
	AKISPRITEMGR->Draw(g_SpriteInfo);

	AKISPRITEMGR->Flush();

	// 画面反映
	AKID3D11MGR->Flip();
}

void MainLoop()
{
	Update();

	Draw();
}

//==============================================================================
// エントリーポイント
//==============================================================================
int APIENTRY _tWinMain( HINSTANCE hinst, HINSTANCE hprev, LPTSTR lpcmd, int nshow )
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウィンドウ作成
#ifdef _DEBUG
	if( APP->InitWindow( hinst, _T( "2DTest" ), SCREEN_WIDTH, SCREEN_HEIGHT ) == false )
	{
		return 1;
	}
	APP->CreateConsoleWindow();
	//APP->CreateDebugWindows();
#else
	if( APP->InitWindow( hinst, _T( "2DTest" ), SCREEN_WIDTH, SCREEN_HEIGHT ) == false )
	{
		return 1;
	}
#endif

	// DirectX初期化
	if( AKID3D11MGR->Init( APP->GetWindowHandle(), SCREEN_WIDTH, SCREEN_HEIGHT, true ) == false )
	{
		return 1;
	}

	// 入力関係初期化
	if( AKIINPUTMGR->Init( hinst, APP->GetWindowHandle() ) == false )
	{
		return 1;
	}

	// サウンド初期化
	if( SOUNDMGR->Init() == false )
	{
		return 1;
	}

	// FBX初期化
	AKIFBXMGR->Init();

	// スプライトマネージャー初期化
	AKISPRITEMGR->Init();

	// テクスチャ作成
	g_Texture.CreateTextureFromFile(AKID3D11DEVICE, AKID3D11CONTEXT, "lena.png");

	// サンプラステート
	g_Sampler.CreateSamplerState(AKID3D11DEVICE);

	// シェーダ読み込み
	g_TestShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	g_TestShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &g_lpInputLayout );
	g_TestShader.CreatePixelShader( "PS", "ps_5_0" );
	g_TestShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// 板ポリ
	g_Board.Init(AKID3D11DEVICE);
	
	// コンスタントバッファ作成
	g_ConstantBuffer.Init(AKID3D11DEVICE);

	// メインループ
	APP->MessageLoop( MainLoop );

	// 板ポリ解放
	g_Board.Release();

	// シェーダ解放
	g_TestShader.Release();

	// その他解放
	g_ConstantBuffer.Release();
	AkiLib::SafeRelease(g_lpInputLayout);

	// スプライトマネージャー解放
	AKISPRITEMGR->Release();

	// FBX解放
	AKIFBXMGR->Release();

	// サウンド解放
	SOUNDMGR->Release();

	// 入力解放
	AKIINPUTMGR->Release();

	// DirectX解放
	AKID3D11MGR->Release();

	return 0;
}