// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float4 PositionSub	   : TEXCOORD1;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 VPosition	   : TEXCOORD2;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 Position        : SV_POSITION;	// ���W( �v���W�F�N�V������� )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// �F
} ;


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// �A���t�@�l��

	float		MulAlphaColor ;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef ;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// �e�N�X�`���J���[���������p�J���[
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler            : register(s0) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register(t0) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
SamplerState g_DepthMapSampler			    : register(s2) ;		// �[�x
Texture2D    g_DepthMapTexture	            : register(t2) ;		// �[�x



// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 Depth;
	float MaxOpacityDistance = lenspos.x;
	float2 TexCoords;
	//*
	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0 ) ;

	// �[�x�e�N�X�`�����W�̎Z�o
	TexCoords.x = (PSInput.PositionSub.x / PSInput.PositionSub.w + 1.0f) / 2.0f;
	TexCoords.y = (-PSInput.PositionSub.y / PSInput.PositionSub.w + 1.0f) / 2.0f;
	Depth = g_DepthMapTexture.Sample(g_DepthMapSampler, TexCoords);

	// �o�̓J���[ = �f�B�t���[�Y�J���[ * �e�N�X�`���J���[ + �X�y�L�����J���[
	PSOutput.Color0 = TextureDiffuseColor;
	//*/

	PSOutput.Color0.a = 1.f - saturate((Depth.r - PSInput.VPosition.z) / MaxOpacityDistance);
	if (PSOutput.Color0.a == 1.f) {
		PSOutput.Color0.a = 0.f;
	}

	PSOutput.Color0.a *= PSInput.Diffuse.a;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}
