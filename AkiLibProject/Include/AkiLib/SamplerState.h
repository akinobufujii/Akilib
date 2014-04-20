//==============================================================================
/// @file	SamplerState.h
/// @brief	サンプラステート
/// @author	藤井章暢
//==============================================================================
#ifndef __AKILIB_SAMPLERSTATE_H__
#define __AKILIB_SAMPLERSTATE_H__

namespace AkiLib
{
	class CSamplerState : public D3D11_SAMPLER_DESC
	{
	public:
		//==============================================================================
		// メソッド
		//==============================================================================
		/// @brief	コンストラクタ
		CSamplerState():
			m_lpSamlerState(nullptr)
		{
			Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			AddressU = AddressV = AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			MipLODBias = 0.f;
			MaxAnisotropy = 0;
			ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0.f;
			MinLOD = MaxLOD = 0.f;
		}

		/// @brief	デストラクタ
		~CSamplerState()
		{
			SafeRelease(m_lpSamlerState);
		}

		/// @brief	サンプラステート作成
		/// @param[in]	device	デバイス
		/// @retval true  成功
		/// @retval false 失敗
		bool CreateSamplerState(ID3D11Device1* device)
		{
			SafeRelease(m_lpSamlerState);

			if(FAILED(device->CreateSamplerState(this, &m_lpSamlerState)))
			{
				return false;
			}

			return true;
		}

		/// @brief	サンプラステート設定
		/// @param[in]	context	コンテキスト
		/// @param[in]	slot	設定スロット	
		void SetSamplerState(ID3D11DeviceContext1* context, UINT slot)
		{
			context->VSSetSamplers(slot, 1, &m_lpSamlerState);
			context->PSSetSamplers(slot, 1, &m_lpSamlerState);
			context->GSSetSamplers(slot, 1, &m_lpSamlerState);
			context->CSSetSamplers(slot, 1, &m_lpSamlerState);
			context->HSSetSamplers(slot, 1, &m_lpSamlerState);
			context->DSSetSamplers(slot, 1, &m_lpSamlerState);
		}

	private:
		ID3D11SamplerState* m_lpSamlerState;	//! サンプラステート
	};
}

#endif // __AKILIB_SAMPLERSTATE_H__
