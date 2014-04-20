//==============================================================================
// �C���N���[�h
//==============================================================================
#include "AkiLib.h"
#include <iostream>

//==============================================================================
// �O���[�o���萔
//==============================================================================
static const UINT WINDOW_WIDTH = 1280;
static const UINT WINDOW_HEIGHT = 720;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
AkiLib::CFPSManager		g_FPS(60);			// FPS����
AkiLib::CBoard			g_Board;			// �|��
AkiLib::CShader			g_TestShader;		// �V�F�[�_
AkiLib::CTexture		g_Texture;			// �e�N�X�`��
AkiLib::CSamplerState	g_Sampler;			// �T���v���X�e�[�g
ID3D11InputLayout*		g_lpInputLayout;	// ���̓��C�A�E�g

// �R���X�^���g�o�b�t�@��16�o�C�g���E�ŃA���C�������g
struct CBUFFER
{
	AKIALIGN16	AkiLib::Matrix	mWVP;
	AKIALIGN16	AkiLib::Matrix	mWorld;
	AKIALIGN16	XMFLOAT3		LightDir;
};

AkiLib::ConstantBuffer<CBUFFER> g_ConstantBuffer;

//==============================================================================
// ���s�֐�
//==============================================================================
void MainLoop()
{
	// FPS����
	g_FPS.Update();

	// �v���C�}���[�o�b�t�@�N���A
	AKID3D11MGR->ClearPrimaryBuffer(0, 0, 1, 1);

	// �V�F�[�_�l��n��
	AkiLib::Matrix mWorld;
	AkiLib::Matrix mView;
	AkiLib::Matrix mProj;

	// �s��ݒ�
	mProj.SetPerspectiveFovLH( 60, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT) );
	mView.SetLockAtLH( XMFLOAT3( 0, 0, -5 ), XMFLOAT3( 0, 0, 0 ) );
	mWorld.SetWorldRotateY( timeGetTime() / 10 % 360 * 1.f );

	// �l��n��
	g_ConstantBuffer.mWVP = XMMatrixTranspose( (mWorld * mView * mProj) );
	g_ConstantBuffer.mWorld = mWorld;
	AKID3D11MGR->SendValueToShader(0, g_ConstantBuffer);

	// ���̓��C�A�E�g�ݒ�
	AKID3D11CONTEXT->IASetInputLayout( g_lpInputLayout );

	// �V�F�[�_�[���\�[�X�ݒ�
	g_Texture.SetTexture(AKID3D11CONTEXT, 0);

	// �T���v���X�e�[�g�ݒ�
	g_Sampler.SetSamplerState(AKID3D11CONTEXT, 0);

	// �V�F�[�_�ݒ�
	AKID3D11CONTEXT->VSSetShader( g_TestShader.GetVertexShader( "VS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PS" ), nullptr, 0 );
	AKID3D11CONTEXT->PSSetShader( g_TestShader.GetPixelShader( "PSTex" ), nullptr, 0 );

	// �`��
	g_Board.Draw(AKID3D11CONTEXT);
	
	// ��ʔ��f
	AKID3D11MGR->Flip();
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

	// DirectX������
	if( AKID3D11MGR->Init( APP->GetWindowHandle(), WINDOW_WIDTH, WINDOW_HEIGHT, true ) == false )
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

	// �V�F�[�_�ǂݍ���
	g_TestShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	g_TestShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &g_lpInputLayout );
	g_TestShader.CreatePixelShader( "PS", "ps_5_0" );
	g_TestShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// �e�N�X�`���쐬
	g_Texture.CreateTextureFromFile(AKID3D11DEVICE, AKID3D11CONTEXT, "tex.png");

	// �T���v���X�e�[�g
	g_Sampler.CreateSamplerState(AKID3D11DEVICE);

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

	// �T�E���h���
	SOUNDMGR->Release();

	// ���͉��
	AKIINPUTMGR->Release();

	// DirectX���
	AKID3D11MGR->Release();

	return 0;
}