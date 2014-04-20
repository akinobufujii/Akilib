//==============================================================================
// インクルード
//==============================================================================
#include "AkiLib.h"
#include <iostream>

//==============================================================================
// グローバル定数
//==============================================================================
static const UINT WINDOW_WIDTH = 1280;
static const UINT WINDOW_HEIGHT = 720;

//==============================================================================
// グローバル変数
//==============================================================================
AkiLib::CFPSManager		g_FPS(60);			// FPS制御
AkiLib::CBoard			g_Board;			// 板ポリ
AkiLib::CShader			g_TestShader;		// シェーダ
AkiLib::CTexture		g_Texture;			// テクスチャ
AkiLib::CSamplerState	g_Sampler;			// サンプラステート
ID3D11InputLayout*		g_lpInputLayout;	// 入力レイアウト

// コンスタントバッファは16バイト境界でアラインメント
struct CBUFFER
{
	AKIALIGN16	AkiLib::Matrix	mWVP;
	AKIALIGN16	AkiLib::Matrix	mWorld;
	AKIALIGN16	XMFLOAT3		LightDir;
};

AkiLib::ConstantBuffer<CBUFFER> g_ConstantBuffer;

//==============================================================================
// 実行関数
//==============================================================================
void MainLoop()
{
	// FPS制御
	g_FPS.Update();

	// プライマリーバッファクリア
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// シェーダ値を渡す
	AkiLib::Matrix mWorld;
	AkiLib::Matrix mView;
	AkiLib::Matrix mProj;

	// 行列設定
	mProj.SetPerspectiveFovLH( 60, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT) );
	mView.SetLockAtLH( XMFLOAT3( 0, 0, -5 ), XMFLOAT3( 0, 0, 0 ) );
	mWorld.SetWorldRotateY( timeGetTime() / 10 % 360 * 1.f );

	// 値を渡す
	g_ConstantBuffer.mWVP = XMMatrixTranspose( (mWorld * mView * mProj) );
	g_ConstantBuffer.mWorld = mWorld;
	AKID3D11MGR->SendValueToShader(0, g_ConstantBuffer);

	// 入力レイアウト設定
	AKID3D11CONTEXT->IASetInputLayout( g_lpInputLayout );

	// シェーダーリソース設定
	g_Texture.SetTexture(AKID3D11CONTEXT, 0);

	// サンプラステート設定
	g_Sampler.SetSamplerState(AKID3D11CONTEXT, 0);

	// シェーダ設定
	AKID3D11CONTEXT->VSSetShader( g_TestShader.GetVertexShader( "VS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PSTex" ), nullptr, 0 );

	// 描画
	g_Board.Draw(AKID3D11CONTEXT);
	
	// 画面反映
	AKID3D11MGR->Flip();
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
	if( APP->InitWindow( hinst, _T( "Skeleton" ), WINDOW_WIDTH, WINDOW_HEIGHT ) == false )
	{
		return 1;
	}
	APP->CreateConsoleWindow();
	//APP->CreateDebugWindows();
#else
	if( APP->InitWindow( hinst, _T( "Skeleton" ), WINDOW_WIDTH, WINDOW_HEIGHT ) == false )
	{
		return 1;
	}
#endif

	// DirectX初期化
	if( AKID3D11MGR->Init( APP->GetWindowHandle(), WINDOW_WIDTH, WINDOW_HEIGHT, true ) == false )
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

	// シェーダ読み込み
	g_TestShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	g_TestShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &g_lpInputLayout );
	g_TestShader.CreatePixelShader( "PS", "ps_5_0" );
	g_TestShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// テクスチャ作成
	g_Texture.CreateTextureFromFile(AKID3D11DEVICE, AKID3D11CONTEXT, "tex.png");

	// サンプラステート
	g_Sampler.CreateSamplerState(AKID3D11DEVICE);

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

	// サウンド解放
	SOUNDMGR->Release();

	// 入力解放
	AKIINPUTMGR->Release();

	// DirectX解放
	AKID3D11MGR->Release();

	return 0;
}