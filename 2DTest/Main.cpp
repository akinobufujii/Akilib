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
AkiLib::CBoard		g_Board;			// �|��
AkiLib::CShader		g_TestShader;		// �V�F�[�_
ID3D11InputLayout*	g_lpInputLayout;	// ���̓��C�A�E�g


XMFLOAT2	g_PolygonPos(0, 0);			// �|���S�����������߂̍��W
float		g_PolygonScale = 150;		// �|���S���̑傫��
float		g_PolygonRotate = 0;		// �|���S���̉�]
AkiLib::CTexture		g_Texture;			// �e�N�X�`��
AkiLib::CSamplerState	g_Sampler;			// �T���v���X�e�[�g

AkiLib::SpriteInfo g_SpriteInfo;	// 2D�X�v���C�g���

// �R���X�^���g�o�b�t�@��16�o�C�g���E�ŃA���C�������g
struct CBUFFER
{
	AKIALIGN16	AkiLib::Matrix	mWVP;
};

AkiLib::ConstantBuffer<CBUFFER> g_ConstantBuffer;

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

	// �|���S���X�P�[��
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGUP, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonScale += 1;
	}
	if(AKIINPUTMGR->IsKeyboardState(DIK_PGDN, AkiLib::CInputManager::PUSH | AkiLib::CInputManager::HOLD))
	{
		g_PolygonScale -= 1;
	}

	// �|���S����]
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

	// �|���S���J���[�ύX
	g_Board.SetColor(AkiLib::ColorRGBA(0xff, 0xff, 0xff, 0xff));

	// �X�v���C�g
	g_SpriteInfo.SetTextureAndDrawSize(&g_Texture, &g_Sampler);
	//g_SpriteInfo.drawTexXY.x = 256.f;
	//g_SpriteInfo.drawWH.x *= 0.5f;
}

void Draw()
{
	// �v���C�}���[�o�b�t�@�N���A
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// �V�F�[�_�l��n��
	AkiLib::Matrix mScale, mTrans, mRotate;
	AkiLib::Matrix mProj, mBaseTrans;

	// �s��ݒ�
	// 2D�`�悷�铊�e�s��ݒ�
	mProj.m[0][0] = 2 / static_cast<float>(SCREEN_WIDTH);
	mProj.m[1][1] = -2 / static_cast<float>(SCREEN_HEIGHT);
	mProj.m[3][0] = -1;
	mProj.m[3][1] = 1;

	// �傫��2�̔|���S���Ȃ̂Ŕ����ɏk�����ď㉺0.5��������
	mBaseTrans.m[0][0] = mBaseTrans.m[3][0] = mBaseTrans.m[3][1] = 0.5f;
	mBaseTrans.m[1][1] = -0.5f;

	// �D���Ȃ悤�Ɋg��ƈړ���������
	mScale.SetWorldScaling(g_PolygonScale);
	mTrans.SetWorldTrans(XMFLOAT3(g_PolygonPos.x, g_PolygonPos.y, 0));
	mRotate.SetWorldRotateZ(g_PolygonRotate);

	// �l��n��
	g_ConstantBuffer.mWVP = XMMatrixTranspose( (mRotate * mBaseTrans * mScale * mTrans * mProj) );
	AKID3D11MGR->SendValueToShader(0, g_ConstantBuffer);

	// ���̓��C�A�E�g�ݒ�
	AKID3D11CONTEXT->IASetInputLayout( g_lpInputLayout );

	// �V�F�[�_�ݒ�
	AKID3D11CONTEXT->VSSetShader( g_TestShader.GetVertexShader( "VS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PSTex" ), nullptr, 0 );

	// �V�F�[�_�[���\�[�X�ݒ�
	g_Texture.SetTexture(AKID3D11CONTEXT, 0);

	// �T���v���X�e�[�g�ݒ�
	g_Sampler.SetSamplerState(AKID3D11CONTEXT, 0);

	// �`��
	g_Board.Draw(AKID3D11CONTEXT);

	// �X�v���C�g�`��
	AKISPRITEMGR->Draw(g_SpriteInfo);

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

	// �V�F�[�_�ǂݍ���
	g_TestShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	g_TestShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &g_lpInputLayout );
	g_TestShader.CreatePixelShader( "PS", "ps_5_0" );
	g_TestShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// �|��
	g_Board.Init(AKID3D11DEVICE);
	
	// �R���X�^���g�o�b�t�@�쐬
	g_ConstantBuffer.Init(AKID3D11DEVICE);

	// ���C�����[�v
	APP->MessageLoop( MainLoop );

	// �|�����
	g_Board.Release();

	// �V�F�[�_���
	g_TestShader.Release();

	// ���̑����
	g_ConstantBuffer.Release();
	AkiLib::SafeRelease(g_lpInputLayout);

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