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

SamplerState g_DiffuseMapSampler            : register(s0);		// �f�B�t���[�Y�}�b�v�T���v��
Texture2D    g_DiffuseMapTexture            : register(t0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��

//�u���b�N�z�[���ɂ����܂̌v�Z���s���֐�(�u���b�N�z�[���̈ʒu�A�v�Z�Ώۂ̃s�N�Z���ʒu)
float2 ZoomCalc(in float2 pixel_pos) {
	float2 pos;
	pos.x = lenspos.x;
	pos.y = lenspos.y;

	float2 out_pos;
	float2 pos_pix_vec = pixel_pos - pos;
	float distance = sqrt(pow(pos_pix_vec.x, 2) + pow(pos_pix_vec.y, 2));//0~0.5
	if (distance >= lenspos.z) {
		out_pos.x = 0;
		out_pos.y = 0;
	}
	else if (distance >= lenspos.z-30.0) 
	{
		float at = (1.f - (distance - (lenspos.z - 30.0))*0.5f / 30.0);
		pos_pix_vec /= lenspos.w*at;
		float2 cpos = pos_pix_vec + pos;
		out_pos.x = (cpos - pixel_pos).x;
		out_pos.y = (cpos - pixel_pos).y;
	}
	else {
		pos_pix_vec /= lenspos.w;
		float2 cpos = pos_pix_vec + pos;
		out_pos.x = (cpos - pixel_pos).x;
		out_pos.y = (cpos - pixel_pos).y;
	}
	return out_pos;
}

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;
	float2 pixel_pos;
	pixel_pos.x = PSInput.texCoords0.x * dispsize.x;
	pixel_pos.y = PSInput.texCoords0.y * dispsize.y;
	float2 CalcPos = pixel_pos;
	float2 calc_pos = { 0,0 };

	calc_pos += ZoomCalc(pixel_pos);

	CalcPos.x += calc_pos.x;
	CalcPos.y += calc_pos.y;

	CalcPos.x = CalcPos.x / dispsize.x;
	CalcPos.y = CalcPos.y / dispsize.y;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, CalcPos);

	// �o�̓J���[ = �e�N�X�`���J���[ * �f�B�t���[�Y�J���[
	PSOutput.color0 = TextureDiffuseColor * PSInput.dif;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}

