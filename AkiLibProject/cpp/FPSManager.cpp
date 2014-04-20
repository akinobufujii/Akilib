//==============================================================================
/// @file	FPSManager.cpp
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
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

// 更新
void CFPSManager::Update()
{
	DWORD	NowTime;	// 現在の時間保存用
	int		Wait;		// 待つべき時間

	// 一周の最初
	if( m_Count == 0 )
	{
		// 完全な最初なら
		if( m_SaveTime == 0 )
		{
			Wait = 0;
		
		}
		// 前フレーム記憶時間から計算
		else
		{	
			Wait = m_CountTime + CLOCKS_PER_SEC - timeGetTime();
		}
	
	}
	// 待つべき時間を計算する
	else
	{
		Wait = static_cast<int>( m_CountTime + m_Count * m_FRAME_SECOND ) - timeGetTime();
	}

	// 計算した分だけ待つ
	if( Wait > 0 )
	{
		Sleep( Wait );
		m_SlowFlg = false;
	}
	else
	{
		m_SlowFlg = true;
	}

	NowTime = timeGetTime();	// 時間を記憶

	// 1周基準を作成
	if( m_Count == 0 )
	{
		m_CountTime = NowTime;
	}

	// 時間保存
	m_spFlameAvgArray.get()[m_Count] = NowTime - Wait;
	m_SaveTime = NowTime;

	// 平均計算
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