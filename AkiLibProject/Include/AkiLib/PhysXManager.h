//==============================================================================
/// @file	PhysXManager.h
/// @brief	PhysX�֘A
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_PHYSXMANAGER_H__
#define __AKILIB_PHYSXMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	PhysX���̃N���X(PhysX�}�l�[�W���[���炵�������ł��Ȃ�)
	//==============================================================================
	class CPhysXRigidBody
	{
	public:
		//==============================================================================
		// �t�����h�w��
		//==============================================================================
		friend class CPhysXManager;

		//==============================================================================
		// �萔
		//==============================================================================
		/// @brief	�V�F�C�v�񋓑�
		enum SHAPETYPE
		{
			ST_BOX,		///< ���^
			ST_NONE,	///< �Ӗ��s��
		};

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�s��l��
		AkiLib::Matrix GetMatrix() const
		{
			return *reinterpret_cast<AkiLib::Matrix*>( &physx::PxMat44(m_lpRigidBody->getGlobalPose()) );
		}

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		physx::PxRigidBody*		m_lpRigidBody;	///< ����
		SHAPETYPE				m_ShapeType;	///< ���̂̎��

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		inline CPhysXRigidBody():
			m_lpRigidBody	( nullptr ),
			m_ShapeType		( ST_NONE )
		{
		}

		/// @brief	�f�X�g���N�^
		inline ~CPhysXRigidBody(){}
	};

	//==============================================================================
	/// @brief	PhysX�}�l�[�W���[�N���X(�V���O���g��)
	//==============================================================================
	class CPhysXManager : public CSingletonTemplate<CPhysXManager>
	{
	public:
		//==============================================================================
		// �t�����h�錾
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�f�X�g���N�^
		~CPhysXManager();

		/// @brief	������
		/// @retval true  ����
		/// @retval false ���s
		bool Init();

		/// @brief	�������
		void Release();

		/// @brief	�X�V
		/// @param[in]	stepSecond	�X�V����(�i�߂�)���Ԃ�b�P�ʂŎw��
		void Update( float stepSecond );

		/// @brief	���̍��̍쐬
		/// @param[in]	normal			�̕���
		/// @param[in]	halfGeometry	���_����@���̕����ւǂꂾ������Ă��邩
		/// @return	�^�̐ÓI�ȍ��̃I�u�W�F�N�g
		physx::PxRigidStatic* CreateRigidStaticPlane(const Vector3& normal, const float distance);

		/// @brief	���̍��̍쐬
		/// @param[in]	pos				�ʒu
		/// @param[in]	halfGeometry	���̑傫���̔���
		/// @param[in]	density			���x
		/// @return	���^�̍��̃I�u�W�F�N�g
		CPhysXRigidBody* CreateRigidbodyBox(const Vector3& pos, const Vector3& halfGeometry, const float density);

		/// @brief	���̂�j�󂷂�
		/// @param[in]	lprigid	�j�󂷂鍄�̂̃|�C���^
		void DestroyRigidbody(CPhysXRigidBody* lprigid);

		/// @brief	���̂�j�󂷂�(�ÓI��)
		/// @param[in]	lprigid	�j�󂷂鍄�̂̃|�C���^
		void DestroyRigidStatic(physx::PxRigidStatic* lprigid);

	private:
		//==============================================================================
		// �t�B�[���h
		//==============================================================================
		physx::PxFoundation*			m_lpFoundation;				///< ��b�I�u�W�F�N�g
		physx::PxProfileZoneManager*	m_lpProfileZoneManager;		///< �������v���t�@�C�����O�I�u�W�F�N�g
		physx::PxPhysics*				m_lpPhysics;				///< �������Z�Ǘ��I�u�W�F�N�g
		physx::PxCooking*				m_lpCooking;				///< �N�b�L���O�I�u�W�F�N�g
		physx::PxScene*					m_lpScene;					///< �����V�[���I�u�W�F�N�g
		bool							m_isInitExtention;			///< �g�����C�u����������������Ă��邩

		physx::PxDefaultAllocator		m_Allocater;				///< PhysX�A���P�[�^
		physx::PxDefaultErrorCallback	m_ErrorCallBack;			///< �G���[�R�[���o�b�N
		physx::PxSceneDesc				m_SceneDesc;				///< PhysX�V�[���ݒ���e
		physx::PxMaterial*				m_lpMaterial;				///< ���̂̍ގ�

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		/// @brief	�R���X�g���N�^
		CPhysXManager();

		/// @brief	PhysX�p����֐��e���v���[�g
		/// @param[in]	obj	�J������I�u�W�F�N�g
		template<typename TYPE>
		void SafePxRerease(TYPE& obj)
		{
			if( obj )
			{
				obj->release();
				obj = nullptr;
			}
		}
	};
}

#define AKIPHYSXMGR	AkiLib::CPhysXManager::GetInstance()

#endif // !__AKILIB_PHYSXMANAGER_H__