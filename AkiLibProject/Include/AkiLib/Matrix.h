//==============================================================================
/// @file	Matrix.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_MATRIX__
#define __AKILIB_MATRIX__

namespace AkiLib
{
	//==============================================================================
	/// @brief	行列クラス
	//==============================================================================
	struct Matrix : public XMMATRIX
	{
		//==============================================================================
		// 定数
		//==============================================================================
		static const XMMATRIX IDENTITIY;	///< 単位行列

		//==============================================================================
		// メソッド
		//==============================================================================
		
		/// @brief	コンストラクタ
		inline explicit Matrix()
		{
			SetIdentity();
		}

		/// @brief	コピーコンストラクタ
		inline Matrix( const XMMATRIX& obj )
		{
			operator=( obj );
		}

		/// @brief デストラクタ
		inline ~Matrix(){}

		/// @brief 代入演算子
		/// @return	自身の参照
		inline const Matrix& operator=( const XMMATRIX& obj )
		{
			XMMATRIX::operator=( obj );
			return *this;
		}

		/// @brief	単位行列設定
		/// @return	自身の参照
		inline const Matrix& SetIdentity()
		{
			*this = XMMatrixIdentity();
			return *this;
		}

		/// @brief	逆行列を設定する
		/// @return	自身の参照
		inline const Matrix& SetInverse()
		{
			*this = XMMatrixInverse( NULL, *this );
			return *this;
		}

		/// @brief	逆行列を獲得
		/// @param[out]	out	このクラスの逆行列を返す
		inline void GetInverse( Matrix* out ) const
		{
			if( out )
			{
				*out = XMMatrixInverse( NULL, *this );
			}
		}

		/// @brief	ワールド座標設定
		/// @param[in]	x	X座標
		/// @param[in]	y	Y座標
		/// @param[in]	z	Z座標
		/// @return	自身の参照
		inline const Matrix& SetWorldTrans( float x, float y, float z )
		{
			*this = XMMatrixTranslation( x, y, z );
			return *this;
		}

		/// @brief	ワールド座標設定
		/// @param[in]	pos	位置
		/// @return	自身の参照
		inline const Matrix& SetWorldTrans( const XMFLOAT3& pos )
		{
			return SetWorldTrans( pos.x, pos.y, pos.z );
		}

		/// @brief	ワールドX軸回転設定
		/// @param[in]	X軸回転角度
		/// @return	自身の参照
		inline const Matrix& SetWorldRotateX( float degree )
		{
			*this = XMMatrixRotationX( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	ワールドX軸回転設定
		/// @param[in]	X軸回転角度
		/// @return	自身の参照
		inline const Matrix& SetWorldRotateY( float degree )
		{
			*this = XMMatrixRotationY( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	ワールドX軸回転設定
		/// @param[in]	X軸回転角度
		/// @return	自身の参照
		inline const Matrix& SetWorldRotateZ( float degree )
		{
			*this = XMMatrixRotationZ( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	ワールドの書く回転軸を設定する
		/// @param[in]	x	X軸拡大率
		/// @param[in]	y	Y軸拡大率
		/// @param[in]	z	Z軸拡大率
		/// @return	自身の参照
		inline const Matrix& SetWorldRotate( float x, float y, float z )
		{
			*this = XMMatrixRotationRollPitchYaw( XMConvertToRadians( x ), XMConvertToRadians( y ), XMConvertToRadians( x ) );
			return *this;
		}

		/// @brief	ワールドの書く回転軸を設定する
		/// @param[in]	rotate	各軸の回転角度
		/// @return	自身の参照
		inline const Matrix& SetWorldRotate( XMFLOAT3 rotate )
		{
			return SetWorldRotate( rotate.x, rotate.y, rotate.z );
		}

		/// @brief	ワールド拡大率設定
		/// @param[in]	x	X軸拡大率
		/// @param[in]	y	Y軸拡大率
		/// @param[in]	z	Z軸拡大率
		/// @return	自身の参照
		inline const Matrix& SetWorldScaling( float x, float y, float z )
		{
			*this = XMMatrixScaling( x, y, z );
			return *this;
		}

		/// @brief	ワールド拡大率設定
		/// @param[in]	scale	各軸の拡大率
		/// @return	自身の参照
		inline const Matrix& SetWorldScaling( XMFLOAT4 scale )
		{
			return SetWorldScaling( scale.x, scale.y, scale.z );
		}

		/// @brief	ワールド拡大率設定
		/// @param[in]	scale	統一の拡大率
		/// @return	自身の参照
		inline const Matrix& SetWorldScaling( float scale )
		{
			return SetWorldScaling( scale, scale, scale );
		}

		/// @brief	ビュー行列設定
		/// @param[in]	pos		位置
		/// @param[in]	look	注視点
		/// @param[in]	up		上方向
		/// @return	自信の参照
		inline const Matrix& SetLockAtLH( const XMFLOAT3& pos, const XMFLOAT3& lock, const XMFLOAT3& up = XMFLOAT3( 0, 1, 0 ) )
		{
			*this = XMMatrixLookAtLH(
				XMVectorSet( pos.x, pos.y, pos.z, 1 ),
				XMVectorSet( lock.x, lock.y, lock.z, 1 ),
				XMVectorSet( up.x, up.y, up.z, 1 ) );

			return *this;
		}

		/// @brief	投影行列設定
		/// @param[in]	angleDegree	画角(角度で指定)
		/// @param[in]	apect		アスペクト比
		/// @param[in]	near		ニアクリップ
		/// @param[in]	far			ファークリップ
		/// @return	自信の参照
		inline const Matrix& SetPerspectiveFovLH( float angleDegree, float apect, float nearClip = 1, float farClip = 1000 )
		{
			*this = XMMatrixPerspectiveFovLH( XMConvertToRadians( angleDegree ), apect, nearClip, farClip );

			return *this;
		}
	};
}

#endif	// #define __AKILIB_MATRIX__