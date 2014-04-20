//==============================================================================
/// @file	FPSManager.cpp
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
CFPSManager::CFPSManager(const UINT fps):
m_FPS				( fps ),
m_FRAME_SECOND		( static_cast<double>( CLOCKS_PER_SEC ) / fps ),
m_SlowFlg			( false ),
m_Avg				( 0 ),
m_Count				( 0 ),
m_CountTime			( 0 ),
m_SaveTime			( 0 ),
m_spFlameAvgArray	( AKINEW int[fps] )
{
}

// �X�V
void CFPSManager::Update()
{
	DWORD	NowTime;	// ���݂̎��ԕۑ��p
	int		Wait;		// �҂ׂ�����

	// ����̍ŏ�
	if( m_Count == 0 )
	{
		// ���S�ȍŏ��Ȃ�
		if( m_SaveTime == 0 )
		{
			Wait = 0;
		
		}
		// �O�t���[���L�����Ԃ���v�Z
		else
		{	
			Wait = m_CountTime + CLOCKS_PER_SEC - timeGetTime();
		}
	
	}
	// �҂ׂ����Ԃ��v�Z����
	else
	{
		Wait = static_cast<int>( m_CountTime + m_Count * m_FRAME_SECOND ) - timeGetTime();
	}

	// �v�Z�����������҂�
	if( Wait > 0 )
	{
		Sleep( Wait );
		m_SlowFlg = false;
	}
	else
	{
		m_SlowFlg = true;
	}

	NowTime = timeGetTime();	// ���Ԃ��L��

	// 1������쐬
	if( m_Count == 0 )
	{
		m_CountTime = NowTime;
	}

	// ���ԕۑ�
	m_spFlameAvgArray.get()[m_Count] = NowTime - Wait;
	m_SaveTime = NowTime;

	// ���όv�Z
	if( m_Count == m_FPS - 1 )
	{
		m_Avg = 0.0;

		for( int i = 0; i < m_FPS; i++ )
		{
			m_Avg += m_spFlameAvgArray.get()[ i ];
		}

		m_Avg /= m_FPS;

#if 0
		char Str[ 0xff ];

		sprintf_s( Str, sizeof( Str ), "FPS = %lf\n" , m_Avg );

		printf_s( Str );
#endif
	}

	m_Count = (++m_Count) % m_FPS;
}