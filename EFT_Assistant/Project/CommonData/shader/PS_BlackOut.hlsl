// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 dif         : COLOR0;		// �f�B�t���[�Y�J���[
	float2 texCoords0  : TEXCOORD0;	// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color0           : SV_TARGET0;	// �F
};


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// �A���t�@�l��

	float		MulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1;

	int			AlphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2;

	float4		IgnoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
	float2	dispsize;
}

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler : register(s0);		// �f�B�t���[�Y�}�b�v�T���v��
Texture2D    g_DiffuseMapTexture            : register(t0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;
	float2 pixel_pos;
	pixel_pos.x = PSInput.texCoords0.x * dispsize.x;
	pixel_pos.y = PSInput.texCoords0.y * dispsize.y;

	float2 CalcPos = pixel_pos;

	CalcPos.x = CalcPos.x / dispsize.x;
	CalcPos.y = CalcPos.y / dispsize.y;

	float2 CalcPos2 = PSInput.texCoords0;

	CalcPos2.x = (CalcPos2.x - 0.5f) *lenspos.x;
	CalcPos2.y = (CalcPos2.y - 0.5f) *lenspos.x;


	float distance = 1.f - sqrt(pow(CalcPos2.x, 2) + pow(CalcPos2.y, 2));//0~0.5

	//distance = 0.5f;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, CalcPos);

	// �o�̓J���[ = �e�N�X�`���J���[ * �f�B�t���[�Y�J���[
	PSOutput.color0 = TextureDiffuseColor * PSInput.dif;

	PSOutput.color0.r *= distance;
	PSOutput.color0.g *= distance;
	PSOutput.color0.b *= distance;
	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}

