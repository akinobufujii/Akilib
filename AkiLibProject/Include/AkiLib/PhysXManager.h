//==============================================================================
/// @file	PhysXManager.h
/// @brief	PhysX関連
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_PHYSXMANAGER_H__
#define __AKILIB_PHYSXMANAGER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	PhysX剛体クラス(PhysXマネージャーからしか生成できない)
	//==============================================================================
	class CPhysXRigidBody
	{
	public:
		//==============================================================================
		// フレンド指定
		//==============================================================================
		friend class CPhysXManager;

		//==============================================================================
		// 定数
		//==============================================================================
		/// @brief	シェイプ列挙体
		enum SHAPETYPE
		{
			ST_BOX,		///< 箱型
			ST_NONE,	///< 意味不明
		};

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	行列獲得
		AkiLib::Matrix GetMatrix() const
		{
			return *reinterpret_cast<AkiLib::Matrix*>( &physx::PxMat44(m_lpRigidBody->getGlobalPose()) );
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		physx::PxRigidBody*		m_lpRigidBody;	///< 剛体
		SHAPETYPE				m_ShapeType;	///< 剛体の種類

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		inline CPhysXRigidBody():
			m_lpRigidBody	( nullptr ),
			m_ShapeType		( ST_NONE )
		{
		}

		/// @brief	デストラクタ
		inline ~CPhysXRigidBody(){}
	};

	//==============================================================================
	/// @brief	PhysXマネージャークラス(シングルトン)
	//==============================================================================
	class CPhysXManager : public CSingletonTemplate<CPhysXManager>
	{
	public:
		//==============================================================================
		// フレンド宣言
		//==============================================================================
		friend CSingletonTemplate;

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	デストラクタ
		~CPhysXManager();

		/// @brief	初期化
		/// @retval true  成功
		/// @retval false 失敗
		bool Init();

		/// @brief	解放処理
		void Release();

		/// @brief	更新
		/// @param[in]	stepSecond	更新する(進める)時間を秒単位で指定
		void Update( float stepSecond );

		/// @brief	箱の剛体作成
		/// @param[in]	normal			板の方向
		/// @param[in]	halfGeometry	原点から法線の方向へどれだけ離れているか
		/// @return	板型の静的な剛体オブジェクト
		physx::PxRigidStatic* CreateRigidStaticPlane(const Vector3& normal, const float distance);

		/// @brief	箱の剛体作成
		/// @param[in]	pos				位置
		/// @param[in]	halfGeometry	箱の大きさの半分
		/// @param[in]	density			密度
		/// @return	箱型の剛体オブジェクト
		CPhysXRigidBody* CreateRigidbodyBox(const Vector3& pos, const Vector3& halfGeometry, const float density);

		/// @brief	剛体を破壊する
		/// @param[in]	lprigid	破壊する剛体のポインタ
		void DestroyRigidbody(CPhysXRigidBody* lprigid);

		/// @brief	剛体を破壊する(静的版)
		/// @param[in]	lprigid	破壊する剛体のポインタ
		void DestroyRigidStatic(physx::PxRigidStatic* lprigid);

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		physx::PxFoundation*			m_lpFoundation;				///< 基礎オブジェクト
		physx::PxProfileZoneManager*	m_lpProfileZoneManager;		///< メモリプロファイリングオブジェクト
		physx::PxPhysics*				m_lpPhysics;				///< 物理演算管理オブジェクト
		physx::PxCooking*				m_lpCooking;				///< クッキングオブジェクト
		physx::PxScene*					m_lpScene;					///< 物理シーンオブジェクト
		bool							m_isInitExtention;			///< 拡張ライブラリが初期化されているか

		physx::PxDefaultAllocator		m_Allocater;				///< PhysXアロケータ
		physx::PxDefaultErrorCallback	m_ErrorCallBack;			///< エラーコールバック
		physx::PxSceneDesc				m_SceneDesc;				///< PhysXシーン設定内容
		physx::PxMaterial*				m_lpMaterial;				///< 剛体の材質

		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CPhysXManager();

		/// @brief	PhysX用解放関数テンプレート
		/// @param[in]	obj	開放するオブジェクト
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