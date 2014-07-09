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

AkiLib::CTexture		g_Texture;			// テクスチャ
AkiLib::CSamplerState	g_Sampler;			// サンプラステート

AkiLib::SpriteInfo g_SpriteInfo;	// 2Dスプライト情報

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
		g_SpriteInfo.pos.y -= MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_DOWN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.pos.y += MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_RIGHT, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.pos.x += MOVE_SPEED;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_LEFT, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.pos.x -= MOVE_SPEED;
	}

	// ポリゴンスケール
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGUP, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.scale += 0.01f;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGDN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.scale -= 0.01f;
	}

	// ポリゴン回転
	if(AKIINPUTMGR->IsKeyboardState(DIK_Z, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.rotate = fmodf(g_SpriteInfo.rotate + 1, 360.f);
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_X, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		if((g_SpriteInfo.rotate -= 1) < 0)
		{
			g_SpriteInfo.rotate = 359.f;
		}
	}

	// スプライト
	g_SpriteInfo.SetTextureAndDrawSize(&g_Texture, &g_Sampler);
	//g_SpriteInfo.drawTexXY.x = 256.f;
	//g_SpriteInfo.drawWH.x *= 0.5f;
}

void Draw()
{
	// プライマリーバッファクリア
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// スプライト描画
	AKISPRITEMGR->Draw(g_SpriteInfo);

	// スプライト描画発行
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

	// メインループ
	APP->MessageLoop( MainLoop );

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