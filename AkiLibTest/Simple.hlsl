//==============================================================================
//�O���[�o��
//==============================================================================
cbuffer global : register( b0 )
{
	matrix g_mWVP;
	matrix g_mWorld;
	float3	g_LightDir = float3( 0, 0, 1 );
};

Texture2D		g_Tex		: register( t0 );
SamplerState	g_Sampler	: register( s0 );


Texture2D		g_MeshTex		: register( t1 );
SamplerState	g_MeshSampler	: register( s1 );

SamplerState g_test
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 8;
	AddressU = Wrap;
	AddressV = Wrap;
};

//==============================================================================
// �\���̐錾
//==============================================================================
struct VSOUT
{
	float4	Pos		: SV_POSITION;
	float4	Color	: COLOR;
	float2	UV		: TEXCOORD0;
	float3	Normal	: TEXCOORD1;
};

struct HSCONSTANT
{
	float Outside[4]	: SV_TessFactor;       // �O���̃e�b�Z���[�V�����W��
	float Inside[2]		: SV_InsideTessFactor; // �����̃e�b�Z���[�V�����W��
};

struct HSOUT
{
	float4	Pos		: SV_POSITION;
	float2	UV		: TEXCOORD0;
};

//==============================================================================
// ���_�V�F�[�_
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

// ���b�V���p���_�V�F�[�_
VSOUT VS_Mesh(	float4 pos		: POSITION,
			  float3 normal	: NORMAL,
			  float2 uv		: TEXCOORD0)
{
	VSOUT Out = (VSOUT)0;

	Out.Pos		= mul( pos, g_mWVP );
	Out.UV		= uv;
	Out.Normal	= mul( normal, (float3x3)g_mWorld );

	return Out;  
}

//==============================================================================
//�s�N�Z���V�F�[�_�[
//==============================================================================
float4 PS( VSOUT vsin ) : SV_Target
{
	return vsin.Color;
}

// ���b�V���p�s�N�Z���V�F�[�_
float4 PS_Mesh( VSOUT vsin ) : SV_TARGET0
{
	//return float4( vsin.Normal, 1 );
	float4 temp = g_MeshTex.Sample( g_MeshSampler, vsin.UV ) * saturate( dot( vsin.Normal, -normalize( g_LightDir ) ) );

		temp.a = 1;
	return temp;
}

// �e�N�X�`���\��t��
float4 PSTex( VSOUT vsin ) : SV_Target
{
	return g_Tex.Sample( g_Sampler, vsin.UV );
}

struct STRUCTUREBUFFER
{
	int temp;
};

//==============================================================================
// �R���s���[�g�V�F�[�_
//==============================================================================
StructuredBuffer<STRUCTUREBUFFER>	InParam : register( t0 );
RWStructuredBuffer<STRUCTUREBUFFER> Result : register( u0 );

[numthreads(1,1,1)]
void CS_Calc( int id : SV_GroupIndex )
{
	Result[0].temp += InParam[id].temp;
}

//==============================================================================
// �e�b�Z���[�V�����p
//==============================================================================
// �p�b�`�萔�n���V�F�[�_
HSCONSTANT HS_Constant(InputPatch<VSOUT, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	HSCONSTANT outHSConstant;

	// �Ƃ肠�����K���ɕ���
	outHSConstant.Inside[0] = outHSConstant.Inside[1] = 7;
	outHSConstant.Outside[0] = outHSConstant.Outside[1] = outHSConstant.Outside[2] = outHSConstant.Outside[3] = 7;

	return outHSConstant;
}

// �R���g���[���|�C���g�n���V�F�[�_
[domain("quad")]					// �e�b�Z���[�V�������郁�b�V���̌`��
[partitioning("integer")]			// �p�b�`�̕����Ɏg�p����A���S���Y��
[outputtopology("triangle_cw")]		// ���b�V���̏o�͕��@���w�肷��
[outputcontrolpoints(4)]			// �n���V�F�[�_�[�̃R���g���[���|�C���g�t�F�[�Y���R�[��������
[patchconstantfunc("HS_Constant")]	// �Ή�����p�b�`�萔�n���V�F�[�_�֐�
HSOUT HS( InputPatch<VSOUT, 4> In, uint index : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
	HSOUT Out;

	// ���̂܂ܓn��
	Out.Pos		= In[index].Pos;
	Out.UV		= In[index].UV;

	return Out;
}

// �h���C���V�F�[�_
[domain("quad")]
VSOUT DS( HSCONSTANT In, float2 uv : SV_DomainLocation, const OutputPatch<HSOUT, 4> patch )
{
	// ���ꂼ��̓K�؂Ȉʒu���v�Z����
	VSOUT Out;

	// ���_���W
	float3 p1 = lerp( patch[0].Pos.xyz, patch[1].Pos.xyz, uv.x );
	float3 p2 = lerp( patch[3].Pos.xyz, patch[2].Pos.xyz, uv.x );

	Out.Pos =  mul( float4( lerp( p1, p2, uv.y ), 1.0f ), g_mWVP );

	// �e�N�Z��
	float2 t1 = lerp( patch[0].UV, patch[1].UV, uv.x );
	float2 t2 = lerp( patch[3].UV, patch[2].UV, uv.x );
	Out.UV = lerp( t1, t2, uv.y );

	return Out;
}

// �h���C���V�F�[�_���ʂ𔽉f������s�N�Z���V�F�[�_
float4 PS_DomainOut() : SV_Target0
{
	return float4( 1, 0, 1, 1 );
}