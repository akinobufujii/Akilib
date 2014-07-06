//==============================================================================
//グローバル
//==============================================================================
cbuffer global : register( b0 )
{
	matrix g_mWVP;
};

Texture2D		g_Tex		: register( t0 );
SamplerState	g_Sampler	: register( s0 );

SamplerState g_test
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 8;
	AddressU = Wrap;
	AddressV = Wrap;
};

//==============================================================================
// 構造体宣言
//==============================================================================
struct VSOUT
{
	float4	Pos		: SV_POSITION;
	float4	Color	: COLOR;
	float2	UV		: TEXCOORD0;
};

//==============================================================================
// 頂点シェーダ
//==============================================================================
VSOUT VS(float4 pos	: POSITION,
		 float4 color	: COLOR0,
		 float2 uv		: TEXCOORD0)
{ 
	VSOUT Out = (VSOUT)0;

	Out.Pos = mul( pos, g_mWVP );
	Out.Color = color;
	Out.UV = uv;

	return Out;  
}

//==============================================================================
//ピクセルシェーダー
//==============================================================================
float4 PS( VSOUT vsin ) : SV_Target
{
	return vsin.Color;
}

// テクスチャ貼り付け
float4 PSTex( VSOUT vsin ) : SV_Target
{
	return g_Tex.Sample( g_Sampler, vsin.UV ) * vsin.Color;
}
