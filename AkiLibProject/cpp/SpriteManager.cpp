//==============================================================================
/// @file	D3D11Manager.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CSpriteManager::CSpriteManager()
{

}

// �f�X�g���N�^
CSpriteManager::~CSpriteManager()
{
	Release();
}

// ������
bool CSpriteManager::Init()
{
	// �R���X�^���g�o�b�t�@�쐬
	m_ConstantBuffer.Init(AKID3D11DEVICE);

	// �V�F�[�_�ǂݍ���
	m_SpriteShader.Init( "Simple.hlsl", AKID3D11DEVICE );
	m_SpriteShader.CreateVertexShader( "VS", "vs_5_0", AkiLib::CBoard::VERTEX_LAYOUT, AkiLib::CBoard::NUM_VERTEXELEMENT, &m_lpInputLayout );
	m_SpriteShader.CreatePixelShader( "PS", "ps_5_0" );
	m_SpriteShader.CreatePixelShader( "PSTex", "ps_5_0" );

	// �|��
	m_Board.Init(AKID3D11DEVICE);

	return true;
}

// ���
void CSpriteManager::Release()
{
	m_ConstantBuffer.Release();
}

void CSpriteManager::Draw(const SpriteInfo& info)
{
	m_SpriteQueue.push(info);
}

// ���ׂĕ`��
void CSpriteManager::Flush()
{
	AkiLib::Matrix mProj, mBaseTrans;

	// �r���[�|�[�g�����ʂ̕��������擾
	D3D11_VIEWPORT vp;
	UINT vpSlot = 1;
	AKID3D11CONTEXT->RSGetViewports(&vpSlot, &vp);

	// 2D�`�悷�铊�e�s��ݒ�
	mProj.m[0][0] = 2 / static_cast<float>(vp.Width);
	mProj.m[1][1] = -2 / static_cast<float>(vp.Height);
	mProj.m[3][0] = -1;
	mProj.m[3][1] = 1;

	// �傫��2�̔|���S���Ȃ̂Ŕ����ɏk�����ď㉺0.5��������
	mBaseTrans.m[0][0] = mBaseTrans.m[3][0] = mBaseTrans.m[3][1] = 0.5f;
	mBaseTrans.m[1][1] = -0.5f;

	while(m_SpriteQueue.size())
	{
		SpriteInfo& info = m_SpriteQueue.front();

		// �V�F�[�_�l��n��
		AkiLib::Matrix mScale, mTrans, mRotate;
		
		// �s��ݒ�
		// �D���Ȃ悤�Ɋg��ƈړ���������
		mScale.SetWorldScaling(info.scale);
		mTrans.SetWorldTrans(XMFLOAT3(info.pos.x, info.pos.y, 0));
		mRotate.SetWorldRotateZ(info.rotate);

		// �l��n��
		m_ConstantBuffer.mWVP = XMMatrixTranspose( (mRotate * mBaseTrans * mScale * mTrans * mProj) );
		AKID3D11MGR->SendValueToShader(0, m_ConstantBuffer);

		// ���̓��C�A�E�g�ݒ�
		AKID3D11CONTEXT->IASetInputLayout( m_lpInputLayout );

		// �V�F�[�_�ݒ�
		AKID3D11CONTEXT->VSSetShader( m_SpriteShader.GetVertexShader( "VS" ), nullptr, 0 );

		if(info.lpSampler && info.lpTexture)
		{
			// �V�F�[�_�[���\�[�X�ݒ�
			info.lpTexture->SetTexture(AKID3D11CONTEXT, 0);

			// �T���v���X�e�[�g�ݒ�
			info.lpSampler->SetSamplerState(AKID3D11CONTEXT, 0);

			AKID3D11CONTEXT->PSSetShader( m_SpriteShader.GetPixelShader( "PSTex" ), nullptr, 0 );
		}
		else
		{
			AKID3D11CONTEXT->PSSetShader( m_SpriteShader.GetPixelShader( "PS" ), nullptr, 0 );
		}

		// �`��
		m_Board.Draw(AKID3D11CONTEXT);

		m_SpriteQueue.pop();
	}
}