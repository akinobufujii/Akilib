//==============================================================================
/// @file	PhysXManager.cpp
/// @brief	PhysX関連実装
/// @author	藤井章暢
//==============================================================================

//==============================================================================
// 名前空間省略
//==============================================================================
using namespace AkiLib;

// コンストラクタ
CPhysXManager::CPhysXManager():
	m_lpFoundation			( nullptr ),
	m_lpProfileZoneManager	( nullptr ),
	m_lpPhysics				( nullptr ),
	m_lpCooking				( nullptr ),
	m_lpScene				( nullptr ),
	m_isInitExtention		( false ),
	m_SceneDesc				( physx::PxTolerancesScale() )
{
}

// デストラクタ
CPhysXManager::~CPhysXManager()
{
}

// 初期化
bool CPhysXManager::Init()
{
	try
	{
		// 基礎オブジェクト作成
		m_lpFoundation = PxCreateFoundation(
			PX_PHYSICS_VERSION, 
			m_Allocater,
			m_ErrorCallBack );
		if( m_lpFoundation == nullptr )
		{
			throw "PhysX基礎オブジェクト作成失敗";
		}

		// トップレベルオブジェクト作成
		m_lpProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager( m_lpFoundation );
		if( m_lpProfileZoneManager == nullptr )
		{
			throw "PhysXプロファイリングオブジェクト作成失敗";
		}

		// 物理演算管理オブジェクト作成
		m_lpPhysics = PxCreatePhysics( 
			PX_PHYSICS_VERSION,
			*m_lpFoundation,
			physx::PxTolerancesScale(),
			true, m_lpProfileZoneManager );
		if( m_lpPhysics == nullptr )
		{
			throw "PhysX物理演算オブジェクト作成失敗";
		}

		// クッキングオブジェクト作成
		m_lpCooking = PxCreateCooking(
			PX_PHYSICS_VERSION, 
			*m_lpFoundation,
			physx::PxCookingParams() );
		if( m_lpCooking == nullptr )
		{
			throw "PhysXクッキングオブジェクト作成失敗";
		}

		// 拡張ライブラリ初期化
		if( PxInitExtensions( *m_lpPhysics ) == false )
		{
			throw "PhysX拡張ライブラリ初期化失敗";
		}
		else
		{
			m_isInitExtention = true;
		}

		// シーン作成
		m_SceneDesc = physx::PxSceneDesc( m_lpPhysics->getTolerancesScale() );
		m_SceneDesc.gravity = physx::PxVec3( 0, -9.8f, 0 );	// 重力は重力加速度の値にしておく
		
		// スレッド作成
		if( m_SceneDesc.cpuDispatcher == nullptr )
		{
			m_SceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
			if( m_SceneDesc.cpuDispatcher == nullptr )
			{
				throw "PhysXスレッド作成失敗";
			}
		}

		// 衝突フィルタリング関数設定
		if( m_SceneDesc.filterShader == nullptr )
		{
			m_SceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
		}

		m_lpScene = m_lpPhysics->createScene( m_SceneDesc );
		if( m_lpScene == nullptr )
		{
			throw "PhysXシーン作成失敗";
		}

		// 材質作成(値は適当に入れておく)
		m_lpMaterial = m_lpPhysics->createMaterial( 2, 2, 0.1f );
	}
	catch( LPCSTR error )
	{
		ErrorMessageBox( error );
		return false;
	}
	catch( ... )
	{
		ErrorMessageBox( "PhysX初期化中に原因不明のエラーが発生しました" );
		return false;
	}

	return true;
}

// 解放
void CPhysXManager::Release()
{
	SafePxRerease( m_lpMaterial );
	delete m_SceneDesc.cpuDispatcher;
	m_SceneDesc.cpuDispatcher = nullptr;

	SafePxRerease( m_lpScene );

	if( m_isInitExtention )
	{
		PxCloseExtensions();
		m_isInitExtention = false;
	}

	SafePxRerease( m_lpCooking );
	SafePxRerease( m_lpPhysics );
	SafePxRerease( m_lpProfileZoneManager );
	SafePxRerease( m_lpFoundation );
}

// 更新
void CPhysXManager::Update(float stepSecond)
{
	if( m_lpScene )
	{
		m_lpScene->fetchResults( true );
		m_lpScene->simulate( stepSecond );
		m_lpScene->fetchResults( false );
	}
}

// 板の静的剛体作成
physx::PxRigidStatic* CPhysXManager::CreateRigidStaticPlane(const Vector3& normal, const float distance)
{
	// 板の剛体作成
	physx::PxRigidStatic* lptemp = 
		PxCreatePlane( 
		*m_lpPhysics,
		physx::PxPlane( physx::PxVec3(normal.x, normal.y, normal.z), 10 ),
		*m_lpMaterial );

	// シーンに追加
	m_lpScene->addActor( *lptemp );
	
	return lptemp;
}

// 箱の剛体作成
CPhysXRigidBody* CPhysXManager::CreateRigidbodyBox(const Vector3& pos, const Vector3& halfGeometry, const float density)
{
	CPhysXRigidBody* lptemp = AKINEW CPhysXRigidBody();

	// 箱の剛体作成
	lptemp->m_lpRigidBody =	PxCreateDynamic(
			*m_lpPhysics,										// 物理
			physx::PxTransform( physx::PxVec3( pos.x, pos.y, pos.z ), physx::PxQuat::createIdentity() ),	// 位置
			physx::PxBoxGeometry( halfGeometry.x, halfGeometry.y, halfGeometry.z ),					// 剛体の種類
			*m_lpMaterial,										// 衝突材料
			density );				

	lptemp->m_ShapeType = CPhysXRigidBody::ST_BOX;

	//lptemp->m_lpRigidBody->createShape(physx::PxBoxGeometry( 1, 1, 1 ), *m_lpMaterial);

	// シーンに追加
	m_lpScene->addActor( *lptemp->m_lpRigidBody );
	
	return lptemp;
}

// 剛体破壊
void CPhysXManager::DestroyRigidStatic(physx::PxRigidStatic* lprigid)
{
	m_lpScene->removeActor( *lprigid );
	SafePxRerease( lprigid );
}

// 剛体破壊
void CPhysXManager::DestroyRigidbody(CPhysXRigidBody* lprigid)
{
	m_lpScene->removeActor( *lprigid->m_lpRigidBody );
	SafePxRerease( lprigid->m_lpRigidBody );
	delete lprigid;
}