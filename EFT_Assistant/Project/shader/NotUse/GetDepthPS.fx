// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0;
	float4 Specular        : COLOR1;
	float2 TexCoords0      : TEXCOORD0;
	float4 VPosition	   : TEXCOORD1;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 Position        : SV_POSITION;	// ���W( �v���W�F�N�V������� )
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0;	// �F
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

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;

	// �o�̓J���[ = �f�B�t���[�Y�J���[
	PSOutput.Color0.r = PSInput.VPosition.z;

	// �o�̓A���t�@ = 1.f
	PSOutput.Color0.a = 1.f;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}

/*
// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 ViewPosition   : TEXCOORD0;
};
// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0         : COLOR0;
};

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;

	// ���s���l����������
	PSOutput.Color0.r = PSInput.ViewPosition.z;
	PSOutput.Color0.g = 0.0f;
	PSOutput.Color0.b = 0.0f;
	PSOutput.Color0.a = 1.0;

	return PSOutput;
}
//*/