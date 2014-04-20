//==============================================================================
/// @file	PhysXManager.cpp
/// @brief	PhysX�֘A����
/// @author	����͒�
//==============================================================================

//==============================================================================
// ���O��ԏȗ�
//==============================================================================
using namespace AkiLib;

// �R���X�g���N�^
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

// �f�X�g���N�^
CPhysXManager::~CPhysXManager()
{
}

// ������
bool CPhysXManager::Init()
{
	try
	{
		// ��b�I�u�W�F�N�g�쐬
		m_lpFoundation = PxCreateFoundation(
			PX_PHYSICS_VERSION, 
			m_Allocater,
			m_ErrorCallBack );
		if( m_lpFoundation == nullptr )
		{
			throw "PhysX��b�I�u�W�F�N�g�쐬���s";
		}

		// �g�b�v���x���I�u�W�F�N�g�쐬
		m_lpProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager( m_lpFoundation );
		if( m_lpProfileZoneManager == nullptr )
		{
			throw "PhysX�v���t�@�C�����O�I�u�W�F�N�g�쐬���s";
		}

		// �������Z�Ǘ��I�u�W�F�N�g�쐬
		m_lpPhysics = PxCreatePhysics( 
			PX_PHYSICS_VERSION,
			*m_lpFoundation,
			physx::PxTolerancesScale(),
			true, m_lpProfileZoneManager );
		if( m_lpPhysics == nullptr )
		{
			throw "PhysX�������Z�I�u�W�F�N�g�쐬���s";
		}

		// �N�b�L���O�I�u�W�F�N�g�쐬
		m_lpCooking = PxCreateCooking(
			PX_PHYSICS_VERSION, 
			*m_lpFoundation,
			physx::PxCookingParams() );
		if( m_lpCooking == nullptr )
		{
			throw "PhysX�N�b�L���O�I�u�W�F�N�g�쐬���s";
		}

		// �g�����C�u����������
		if( PxInitExtensions( *m_lpPhysics ) == false )
		{
			throw "PhysX�g�����C�u�������������s";
		}
		else
		{
			m_isInitExtention = true;
		}

		// �V�[���쐬
		m_SceneDesc = physx::PxSceneDesc( m_lpPhysics->getTolerancesScale() );
		m_SceneDesc.gravity = physx::PxVec3( 0, -9.8f, 0 );	// �d�͂͏d�͉����x�̒l�ɂ��Ă���
		
		// �X���b�h�쐬
		if( m_SceneDesc.cpuDispatcher == nullptr )
		{
			m_SceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
			if( m_SceneDesc.cpuDispatcher == nullptr )
			{
				throw "PhysX�X���b�h�쐬���s";
			}
		}

		// �Փ˃t�B���^�����O�֐��ݒ�
		if( m_SceneDesc.filterShader == nullptr )
		{
			m_SceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
		}

		m_lpScene = m_lpPhysics->createScene( m_SceneDesc );
		if( m_lpScene == nullptr )
		{
			throw "PhysX�V�[���쐬���s";
		}

		// �ގ��쐬(�l�͓K���ɓ���Ă���)
		m_lpMaterial = m_lpPhysics->createMaterial( 2, 2, 0.1f );
	}
	catch( LPCSTR error )
	{
		ErrorMessageBox( error );
		return false;
	}
	catch( ... )
	{
		ErrorMessageBox( "PhysX���������Ɍ����s���̃G���[���������܂���" );
		return false;
	}

	return true;
}

// ���
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

// �X�V
void CPhysXManager::Update(float stepSecond)
{
	if( m_lpScene )
	{
		m_lpScene->fetchResults( true );
		m_lpScene->simulate( stepSecond );
		m_lpScene->fetchResults( false );
	}
}

// �̐ÓI���̍쐬
physx::PxRigidStatic* CPhysXManager::CreateRigidStaticPlane(const Vector3& normal, const float distance)
{
	// �̍��̍쐬
	physx::PxRigidStatic* lptemp = 
		PxCreatePlane( 
		*m_lpPhysics,
		physx::PxPlane( physx::PxVec3(normal.x, normal.y, normal.z), 10 ),
		*m_lpMaterial );

	// �V�[���ɒǉ�
	m_lpScene->addActor( *lptemp );
	
	return lptemp;
}

// ���̍��̍쐬
CPhysXRigidBody* CPhysXManager::CreateRigidbodyBox(const Vector3& pos, const Vector3& halfGeometry, const float density)
{
	CPhysXRigidBody* lptemp = AKINEW CPhysXRigidBody();

	// ���̍��̍쐬
	lptemp->m_lpRigidBody =	PxCreateDynamic(
			*m_lpPhysics,										// ����
			physx::PxTransform( physx::PxVec3( pos.x, pos.y, pos.z ), physx::PxQuat::createIdentity() ),	// �ʒu
			physx::PxBoxGeometry( halfGeometry.x, halfGeometry.y, halfGeometry.z ),					// ���̂̎��
			*m_lpMaterial,										// �Փˍޗ�
			density );				

	lptemp->m_ShapeType = CPhysXRigidBody::ST_BOX;

	//lptemp->m_lpRigidBody->createShape(physx::PxBoxGeometry( 1, 1, 1 ), *m_lpMaterial);

	// �V�[���ɒǉ�
	m_lpScene->addActor( *lptemp->m_lpRigidBody );
	
	return lptemp;
}

// ���̔j��
void CPhysXManager::DestroyRigidStatic(physx::PxRigidStatic* lprigid)
{
	m_lpScene->removeActor( *lprigid );
	SafePxRerease( lprigid );
}

// ���̔j��
void CPhysXManager::DestroyRigidbody(CPhysXRigidBody* lprigid)
{
	m_lpScene->removeActor( *lprigid->m_lpRigidBody );
	SafePxRerease( lprigid->m_lpRigidBody );
	delete lprigid;
}