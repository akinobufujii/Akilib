//==============================================================================
/// @file	Vector.h
/// @brief	ベクトル関連
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_VECTOR_H__
#define __AKILIB_VECTOR_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	3次元ベクトル
	//==============================================================================
	struct Vector3 : public XMFLOAT3
	{
		/// @brief	コンストラクタ
		inline Vector3()
		{
			x = y = z = 0;
		}

		/// @brief	引数付きコンストラクタ
		/// @param[in]	x	X座標
		/// @param[in]	y	X座標
		/// @param[in]	Z	X座標
		inline Vector3( float x, float y, float z )
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/// @brief	互換用コンストラクタ
		/// @param[in]	obj	3次元ベクトル型
		inline Vector3( const XMFLOAT3& obj )
		{
			XMFLOAT3::operator=( obj );
		}

		/// @brief	互換用コンストラクタ
		/// @param[in]	obj	ベクタ型
		inline Vector3( const XMVECTOR& obj )
		{
			XMStoreFloat3( this, obj );
		}

		/// @brief	互換用代入演算子
		/// @param[in]	obj	3次元ベクトル型
		const Vector3& operator=( const XMFLOAT3& obj )
		{
			XMFLOAT3::operator=( obj );
			return *this;
		}

		/// @brief	互換用代入演算子
		/// @param[in]	obj	ベクタ型
		const Vector3& operator=( const XMVECTOR& obj )
		{
			XMStoreFloat3( this, obj );

			return *this;
		}

		/// @brief	引き算
		/// @param[in]	obj	3次元ベクトル型
		inline Vector3 operator-( const XMFLOAT3& obj )
		{
			XMFLOAT3 temp = *this;

			temp.x -= obj.x;
			temp.y -= obj.y;
			temp.z -= obj.z;
			
			return temp;
		}

		/// @brief	+=演算子
		/// @param[in]	obj	3次元ベクトル型
		inline const Vector3& operator+=( const XMFLOAT3& obj )
		{
			this->x += obj.x;
			this->y += obj.y;
			this->z += obj.z;
			
			return *this;
		}

		/// @brief	スカラ
		/// @param[in]	scala
		inline const Vector3& operator*=( float scala )
		{
			this->x *= scala;
			this->y *= scala;
			this->z *= scala;
			
			return *this;
		}

		/// @brief	ベクタ型に変換
		/// @return	変換したベクタ型
		inline operator XMVECTOR()
		{
			return XMLoadFloat3( this );
		}

		/// @brief	外積計算
		/// @param[in]	vec1	1つ目のベクトル
		/// @param[in]	vec2	2つ目のベクトル
		/// @return	自身の参照
		inline const Vector3& SetCross( const XMFLOAT3& vec1, const XMFLOAT3& vec2 )
		{
			return *this = XMVector3Cross( Vector3( vec1 ), Vector3( vec2 ) );
		}

		/// @brief	外積計算
		/// @param[in]	vec	ベクトル
		/// @return	自身の参照
		inline const Vector3& SetCross( const XMFLOAT3& vec )
		{
			return *this = XMVector3Cross( *this, Vector3( vec ) );
		}

		/// @brief	正規化(自らは変化しない)
		/// @param[out]	正規化したベクトルを出力するポインタ(NULLでも可)
		/// @return	自身の正規化ベクトル
		inline Vector3 GetNormal( XMFLOAT3* out = nullptr )
		{	
			if( out )
			{
				return *out = Vector3( XMVector3Normalize( *this ) );
			}

			return Vector3( XMVector3Normalize( *this ) );
		}

		/// @brief	正規化
		/// @return	自身の参照
		inline const Vector3& Normalize()
		{
			return *this = XMVector3Normalize( *this );
		}
	};
}

#endif // #define __AKILIB_VECTOR_H__