// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 dif : COLOR0; // �f�B�t���[�Y�J���[
	float2 texCoords0 : TEXCOORD0; // �e�N�X�`�����W
	float4 pos : SV_POSITION; // ���W( �v���W�F�N�V������� )
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color0 : SV_TARGET0; // �F
};


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4 FactorColor; // �A���t�@�l��

	float MulAlphaColor; // �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float AlphaTestRef; // �A���t�@�e�X�g�Ŏg�p�����r�l
	float2 Padding1;

	int AlphaTestCmpMode; // �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3 Padding2;

	float4 IgnoreTextureColor; // �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE g_Base;
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
	float2 dispsize;
}

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4 caminfo;
}

SamplerState g_DiffuseMapSampler : register(s0); // �f�B�t���[�Y�}�b�v�T���v��
Texture2D g_DiffuseMapTexture : register(t0); // �f�B�t���[�Y�}�b�v�e�N�X�`��

SamplerState g_Diffuse2MapSampler : register(s1); // �f�B�t���[�Y�}�b�v�T���v��(�ڂ���)
Texture2D g_Diffuse2MapTexture : register(t1); // �f�B�t���[�Y�}�b�v�e�N�X�`��(�ڂ���)

SamplerState g_DepthMapSampler : register(s2); // �[�x�}�b�v�T���v��
Texture2D g_DepthMapTexture : register(t2); // �[�x�}�b�v�e�N�X�`��


PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;

	float per = 0.f;
	float Depth = g_DepthMapTexture.Sample(g_DepthMapSampler, PSInput.texCoords0).r;

	float near_min = caminfo.z;
	float near_max = caminfo.x;
	float far_min = caminfo.y;
	float far_max = caminfo.w;

	//����
	if (Depth == 0.f) {
		per = 0.f;
	}
	//��
	else if (Depth < near_max) {
		per = 1.f - (Depth - near_min) / (near_max - near_min);
	}
	//��
	else if (near_max < Depth && Depth < far_min) {
		per = 0.f;
	}
	//��
	else if (far_min < Depth) {
		per = (Depth - far_min) / (far_max - far_min);
	}
	else {
		per = 1.f;
	}
	if (per <= 0.f) {
		per = 0.f;
	}
	else if(per >= 1.f){
		per = 1.f;
	}

	PSOutput.color0 = lerp(
		g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.texCoords0),
		g_Diffuse2MapTexture.Sample(g_Diffuse2MapSampler, PSInput.texCoords0),
		per);
	return PSOutput;
}