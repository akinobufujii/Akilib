//==============================================================================
/// @file	Matrix.h
/// @author	����͒�
//==============================================================================
#ifndef __AKILIB_MATRIX__
#define __AKILIB_MATRIX__

namespace AkiLib
{
	//==============================================================================
	/// @brief	�s��N���X
	//==============================================================================
	struct Matrix : public XMMATRIX
	{
		//==============================================================================
		// �萔
		//==============================================================================
		static const XMMATRIX IDENTITIY;	///< �P�ʍs��

		//==============================================================================
		// ���\�b�h
		//==============================================================================
		
		/// @brief	�R���X�g���N�^
		inline explicit Matrix()
		{
			SetIdentity();
		}

		/// @brief	�R�s�[�R���X�g���N�^
		inline Matrix( const XMMATRIX& obj )
		{
			operator=( obj );
		}

		/// @brief �f�X�g���N�^
		inline ~Matrix(){}

		/// @brief ������Z�q
		/// @return	���g�̎Q��
		inline const Matrix& operator=( const XMMATRIX& obj )
		{
			XMMATRIX::operator=( obj );
			return *this;
		}

		/// @brief	�P�ʍs��ݒ�
		/// @return	���g�̎Q��
		inline const Matrix& SetIdentity()
		{
			*this = XMMatrixIdentity();
			return *this;
		}

		/// @brief	�t�s���ݒ肷��
		/// @return	���g�̎Q��
		inline const Matrix& SetInverse()
		{
			*this = XMMatrixInverse( NULL, *this );
			return *this;
		}

		/// @brief	�t�s����l��
		/// @param[out]	out	���̃N���X�̋t�s���Ԃ�
		inline void GetInverse( Matrix* out ) const
		{
			if( out )
			{
				*out = XMMatrixInverse( NULL, *this );
			}
		}

		/// @brief	���[���h���W�ݒ�
		/// @param[in]	x	X���W
		/// @param[in]	y	Y���W
		/// @param[in]	z	Z���W
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldTrans( float x, float y, float z )
		{
			*this = XMMatrixTranslation( x, y, z );
			return *this;
		}

		/// @brief	���[���h���W�ݒ�
		/// @param[in]	pos	�ʒu
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldTrans( const XMFLOAT3& pos )
		{
			return SetWorldTrans( pos.x, pos.y, pos.z );
		}

		/// @brief	���[���hX����]�ݒ�
		/// @param[in]	X����]�p�x
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldRotateX( float degree )
		{
			*this = XMMatrixRotationX( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	���[���hX����]�ݒ�
		/// @param[in]	X����]�p�x
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldRotateY( float degree )
		{
			*this = XMMatrixRotationY( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	���[���hX����]�ݒ�
		/// @param[in]	X����]�p�x
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldRotateZ( float degree )
		{
			*this = XMMatrixRotationZ( XMConvertToRadians( degree ) );
			return *this;
		}

		/// @brief	���[���h�̏�����]����ݒ肷��
		/// @param[in]	x	X���g�嗦
		/// @param[in]	y	Y���g�嗦
		/// @param[in]	z	Z���g�嗦
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldRotate( float x, float y, float z )
		{
			*this = XMMatrixRotationRollPitchYaw( XMConvertToRadians( x ), XMConvertToRadians( y ), XMConvertToRadians( x ) );
			return *this;
		}

		/// @brief	���[���h�̏�����]����ݒ肷��
		/// @param[in]	rotate	�e���̉�]�p�x
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldRotate( XMFLOAT3 rotate )
		{
			return SetWorldRotate( rotate.x, rotate.y, rotate.z );
		}

		/// @brief	���[���h�g�嗦�ݒ�
		/// @param[in]	x	X���g�嗦
		/// @param[in]	y	Y���g�嗦
		/// @param[in]	z	Z���g�嗦
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldScaling( float x, float y, float z )
		{
			*this = XMMatrixScaling( x, y, z );
			return *this;
		}

		/// @brief	���[���h�g�嗦�ݒ�
		/// @param[in]	scale	�e���̊g�嗦
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldScaling( XMFLOAT4 scale )
		{
			return SetWorldScaling( scale.x, scale.y, scale.z );
		}

		/// @brief	���[���h�g�嗦�ݒ�
		/// @param[in]	scale	����̊g�嗦
		/// @return	���g�̎Q��
		inline const Matrix& SetWorldScaling( float scale )
		{
			return SetWorldScaling( scale, scale, scale );
		}

		/// @brief	�r���[�s��ݒ�
		/// @param[in]	pos		�ʒu
		/// @param[in]	look	�����_
		/// @param[in]	up		�����
		/// @return	���M�̎Q��
		inline const Matrix& SetLockAtLH( const XMFLOAT3& pos, const XMFLOAT3& lock, const XMFLOAT3& up = XMFLOAT3( 0, 1, 0 ) )
		{
			*this = XMMatrixLookAtLH(
				XMVectorSet( pos.x, pos.y, pos.z, 1 ),
				XMVectorSet( lock.x, lock.y, lock.z, 1 ),
				XMVectorSet( up.x, up.y, up.z, 1 ) );

			return *this;
		}

		/// @brief	���e�s��ݒ�
		/// @param[in]	angleDegree	��p(�p�x�Ŏw��)
		/// @param[in]	apect		�A�X�y�N�g��
		/// @param[in]	near		�j�A�N���b�v
		/// @param[in]	far			�t�@�[�N���b�v
		/// @return	���M�̎Q��
		inline const Matrix& SetPerspectiveFovLH( float angleDegree, float apect, float nearClip = 1, float farClip = 1000 )
		{
			*this = XMMatrixPerspectiveFovLH( XMConvertToRadians( angleDegree ), apect, nearClip, farClip );

			return *this;
		}
	};
}

#endif	// #define __AKILIB_MATRIX__