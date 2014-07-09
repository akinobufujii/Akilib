//==============================================================================
// �C���N���[�h
//==============================================================================
#include "AkiLib.h"
#include <iostream>

//==============================================================================
// �O���[�o���萔
//==============================================================================
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
AkiLib::CFPSManager	g_FPS(60);			// FPS����

AkiLib::CTexture		g_Texture;			// �e�N�X�`��
AkiLib::CSamplerState	g_Sampler;			// �T���v���X�e�[�g

AkiLib::SpriteInfo g_SpriteInfo;	// 2D�X�v���C�g���

//==============================================================================
// ���s�֐�
//==============================================================================
void Update()
{
	// FPS����
	g_FPS.Update();

	// �L�[���X�V
	AKIINPUTMGR->UpdateState();

	// �|���S�����W�X�V
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

	// �|���S���X�P�[��
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGUP, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.scale += 0.01f;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGDN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_SpriteInfo.scale -= 0.01f;
	}

	// �|���S����]
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

	// �X�v���C�g
	g_SpriteInfo.SetTextureAndDrawSize(&g_Texture, &g_Sampler);
	//g_SpriteInfo.drawTexXY.x = 256.f;
	//g_SpriteInfo.drawWH.x *= 0.5f;
}

void Draw()
{
	// �v���C�}���[�o�b�t�@�N���A
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// �X�v���C�g�`��
	AKISPRITEMGR->Draw(g_SpriteInfo);

	// �X�v���C�g�`�攭�s
	AKISPRITEMGR->Flush();

	// ��ʔ��f
	AKID3D11MGR->Flip();
}

void MainLoop()
{
	Update();

	Draw();
}

//==============================================================================
// �G���g���[�|�C���g
//==============================================================================
int APIENTRY _tWinMain( HINSTANCE hinst, HINSTANCE hprev, LPTSTR lpcmd, int nshow )
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �E�B���h�E�쐬
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

	// DirectX������
	if( AKID3D11MGR->Init( APP->GetWindowHandle(), SCREEN_WIDTH, SCREEN_HEIGHT, true ) == false )
	{
		return 1;
	}

	// ���͊֌W������
	if( AKIINPUTMGR->Init( hinst, APP->GetWindowHandle() ) == false )
	{
		return 1;
	}

	// �T�E���h������
	if( SOUNDMGR->Init() == false )
	{
		return 1;
	}

	// FBX������
	AKIFBXMGR->Init();

	// �X�v���C�g�}�l�[�W���[������
	AKISPRITEMGR->Init();

	// �e�N�X�`���쐬
	g_Texture.CreateTextureFromFile(AKID3D11DEVICE, AKID3D11CONTEXT, "lena.png");

	// �T���v���X�e�[�g
	g_Sampler.CreateSamplerState(AKID3D11DEVICE);

	// ���C�����[�v
	APP->MessageLoop( MainLoop );

	// �X�v���C�g�}�l�[�W���[���
	AKISPRITEMGR->Release();

	// FBX���
	AKIFBXMGR->Release();

	// �T�E���h���
	SOUNDMGR->Release();

	// ���͉��
	AKIINPUTMGR->Release();

	// DirectX���
	AKID3D11MGR->Release();

	return 0;
}