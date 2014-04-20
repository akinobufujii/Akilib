//==============================================================================
/// @file	Vector.h
/// @brief	�x�N�g���֘A
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_VECTOR_H__
#define __AKILIB_VECTOR_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	3�����x�N�g��
	//==============================================================================
	struct Vector3 : public XMFLOAT3
	{
		/// @brief	�R���X�g���N�^
		inline Vector3()
		{
			x = y = z = 0;
		}

		/// @brief	�����t���R���X�g���N�^
		/// @param[in]	x	X���W
		/// @param[in]	y	X���W
		/// @param[in]	Z	X���W
		inline Vector3( float x, float y, float z )
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/// @brief	�݊��p�R���X�g���N�^
		/// @param[in]	obj	3�����x�N�g���^
		inline Vector3( const XMFLOAT3& obj )
		{
			XMFLOAT3::operator=( obj );
		}

		/// @brief	�݊��p�R���X�g���N�^
		/// @param[in]	obj	�x�N�^�^
		inline Vector3( const XMVECTOR& obj )
		{
			XMStoreFloat3( this, obj );
		}

		/// @brief	�݊��p������Z�q
		/// @param[in]	obj	3�����x�N�g���^
		const Vector3& operator=( const XMFLOAT3& obj )
		{
			XMFLOAT3::operator=( obj );
			return *this;
		}

		/// @brief	�݊��p������Z�q
		/// @param[in]	obj	�x�N�^�^
		const Vector3& operator=( const XMVECTOR& obj )
		{
			XMStoreFloat3( this, obj );

			return *this;
		}

		/// @brief	�����Z
		/// @param[in]	obj	3�����x�N�g���^
		inline Vector3 operator-( const XMFLOAT3& obj )
		{
			XMFLOAT3 temp = *this;

			temp.x -= obj.x;
			temp.y -= obj.y;
			temp.z -= obj.z;
			
			return temp;
		}

		/// @brief	+=���Z�q
		/// @param[in]	obj	3�����x�N�g���^
		inline const Vector3& operator+=( const XMFLOAT3& obj )
		{
			this->x += obj.x;
			this->y += obj.y;
			this->z += obj.z;
			
			return *this;
		}

		/// @brief	�X�J��
		/// @param[in]	scala
		inline const Vector3& operator*=( float scala )
		{
			this->x *= scala;
			this->y *= scala;
			this->z *= scala;
			
			return *this;
		}

		/// @brief	�x�N�^�^�ɕϊ�
		/// @return	�ϊ������x�N�^�^
		inline operator XMVECTOR()
		{
			return XMLoadFloat3( this );
		}

		/// @brief	�O�όv�Z
		/// @param[in]	vec1	1�ڂ̃x�N�g��
		/// @param[in]	vec2	2�ڂ̃x�N�g��
		/// @return	���g�̎Q��
		inline const Vector3& SetCross( const XMFLOAT3& vec1, const XMFLOAT3& vec2 )
		{
			return *this = XMVector3Cross( Vector3( vec1 ), Vector3( vec2 ) );
		}

		/// @brief	�O�όv�Z
		/// @param[in]	vec	�x�N�g��
		/// @return	���g�̎Q��
		inline const Vector3& SetCross( const XMFLOAT3& vec )
		{
			return *this = XMVector3Cross( *this, Vector3( vec ) );
		}

		/// @brief	���K��(����͕ω����Ȃ�)
		/// @param[out]	���K�������x�N�g�����o�͂���|�C���^(NULL�ł���)
		/// @return	���g�̐��K���x�N�g��
		inline Vector3 GetNormal( XMFLOAT3* out = nullptr )
		{	
			if( out )
			{
				return *out = Vector3( XMVector3Normalize( *this ) );
			}

			return Vector3( XMVector3Normalize( *this ) );
		}

		/// @brief	���K��
		/// @return	���g�̎Q��
		inline const Vector3& Normalize()
		{
			return *this = XMVector3Normalize( *this );
		}
	};
}

#endif // #define __AKILIB_VECTOR_H__