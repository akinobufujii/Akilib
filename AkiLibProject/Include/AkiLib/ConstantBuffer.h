//==============================================================================
/// @file	ConstantBuffer.h
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_CONStANTBUFFER_H__
#define __AKILIB_CONStANTBUFFER_H__

namespace AkiLib
{
	//==============================================================================
	/// @brief	コンスタントバッファクラス(テンプレートクラス)
	/// @brief	※必ず16バイトアラインされた構造体をテンプレートタイプににしてください
	//==============================================================================
	template<typename TYPE>
	class ConstantBuffer : public TYPE
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		inline ConstantBuffer():
			m_lpConstantBuffer(nullptr),
			BUFFER_SIZE(sizeof(TYPE))
		{
		}

		/// @brief	デストラクタ
		inline ~ConstantBuffer()
		{
			Release();
		}

		/// @brief	初期化
		/// @param[in]	lpdevice	デバイス
		/// @retval true	成功
		/// @retval false	失敗
		inline bool Init(ID3D11Device1* lpdevice)
		{
			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth		= BUFFER_SIZE;
			bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags		= 0;

			if( FAILED( lpdevice->CreateBuffer( &bufferDesc, nullptr, &m_lpConstantBuffer ) ) )
			{
				ErrorMessageBox( "コンスタントバッファ作成失敗" );
				return false;
			}

			return true;
		}

		/// @brief	解放
		inline void Release()
		{
			AkiLib::SafeRelease(m_lpConstantBuffer);
		}

		/// @brief	バッファポインター獲得
		/// @return	バッファのポインターを返す
		inline ID3D11Buffer* GetBufferPointer() const
		{
			return m_lpConstantBuffer;
		}

		/// @brief	テンプレートタイプの内容獲得
		/// @return	テンプレートタイプの内容ポインターで返す
		const TYPE* GetStructPointer() const
		{
			return static_cast<const TYPE*>(this);
		}

	private:
		//==============================================================================
		// フィールド
		//==============================================================================
		const size_t	BUFFER_SIZE;		// コンスタントバッファのサイズ
		ID3D11Buffer*	m_lpConstantBuffer;	// コンスタントバッファ
	};
}

#endif	// __AKILIB_CONStANTBUFFER_H__