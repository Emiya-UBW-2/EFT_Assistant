// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
	float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
	float4 pos         : SV_POSITION;   // ���W( �v���W�F�N�V������� )
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color0 : SV_TARGET0; // �F
};

//��ʃT�C�Y
static int2 dispsize = {0, 0};


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

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4 caminfo;
}


cbuffer cbLIGHTCAMERA_MATRIX						: register(b4) {
	matrix		g_CamViewToWorldMatrix;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix;	// ���C�g�̃r���[�@�@���@�ˉe�s��
};

cbuffer cbLIGHTCAMERA_MATRIX2						: register(b5) {
	matrix		g_LightViewMatrix1;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix1;	// ���C�g�̃r���[�@�@���@�ˉe�s��
};

cbuffer cbLIGHTCAMERA_MATRIX3						: register(b6) {
	matrix		g_LightViewMatrix2;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix2;	// ���C�g�̃r���[�@�@���@�ˉe�s��
};

SamplerState g_DepthMapSampler : register(s0); // �[�x�}�b�v�T���v��
Texture2D g_DepthMapTexture : register(t0); // �[�x�}�b�v�e�N�X�`��

SamplerState g_ShadowMapSampler : register(s1); // �[�x�o�b�t�@�e�N�X�`��
Texture2D g_ShadowMapTexture : register(t1); // �[�x�o�b�t�@�e�N�X�`��

SamplerState g_ShadowFarMapSampler : register(s2); // �[�x�o�b�t�@�e�N�X�`��
Texture2D g_ShadowFarMapTexture : register(t2); // �[�x�o�b�t�@�e�N�X�`��

float3 DisptoProjNorm(float2 screenUV) {
	float2 pos = screenUV;

	pos /= 0.5f;
	pos.x = pos.x - 1.f;
	pos.y = 1.f - pos.y;

	float3 position;
	position.x = pos.x * caminfo.z * dispsize.x / dispsize.y;
	position.y = pos.y * caminfo.z;
	position.z = 1.f;

	return position;
}


PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	//��ʃT�C�Y���擾���Ă���
	g_DepthMapTexture.GetDimensions(dispsize.x, dispsize.y);

	float Depth = g_DepthMapTexture.Sample(g_DepthMapSampler, PSInput.TexCoords0).r;
	if (Depth <= 0.f) {
		Depth = 100000.f;
	}

	float3 ViewPositionOne = DisptoProjNorm(PSInput.TexCoords0);

	float4 lWorldPosition;
	float4 LPPosition; // ���C�g����݂����W( x��y�̓��C�g�̎ˉe���W�Az�̓r���[���W )
	float4 lLViewPosition;
	float DepthS;

	float Total1 = 0.f;
	float Total2 = 0.f;

	for (int i = 1;i <= 50;i++) {
		if (i <= caminfo.x) {
			lWorldPosition.xyz = ViewPositionOne * Depth * i / caminfo.x;
			lWorldPosition.w = 1.f;
			lWorldPosition = mul(g_CamViewToWorldMatrix, lWorldPosition);

			// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
			// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�
			lLViewPosition = mul(g_LightViewMatrix1, lWorldPosition);
			LPPosition = mul(g_LightProjectionMatrix1, lLViewPosition);
			//LPPosition.x = LPPosition.x*dispsize.x / dispsize.y;
			// �[�x�e�N�X�`���̍��W���Z�o
			LPPosition.x = (LPPosition.x + 1.0f) / 2.0f; // LPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
			LPPosition.y = 1.0f - (LPPosition.y + 1.0f) / 2.0f; // y�͍X�ɏ㉺���]
			DepthS = g_ShadowMapTexture.Sample(g_ShadowMapSampler, LPPosition.xy).r;

			if (((lLViewPosition.z - DepthS) > 0.f) && (DepthS > 0)) {
				Total1++;
			}
			//------------------------------------------------------------------------------------------
			// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�
			lLViewPosition = mul(g_LightViewMatrix2, lWorldPosition);
			LPPosition = mul(g_LightProjectionMatrix2, lLViewPosition);
			//LPPosition.x = LPPosition.x*dispsize.x / dispsize.y;
			// �[�x�e�N�X�`���̍��W���Z�o
			LPPosition.x = (LPPosition.x + 1.0f) / 2.0f; // LPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
			LPPosition.y = 1.0f - (LPPosition.y + 1.0f) / 2.0f; // y�͍X�ɏ㉺���]
			DepthS = g_ShadowFarMapTexture.Sample(g_ShadowFarMapSampler, LPPosition.xy).r;

			if (((lLViewPosition.z - DepthS) > 0.f) && (DepthS > 0)) {
				Total2++;
			}
			// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )
		}
	}
	Total1 /= caminfo.x;
	Total2 /= caminfo.x;

	PSOutput.color0.rgb = min(1.f - Total1, 1.f - Total2);
	PSOutput.color0.a = 1.f;

	return PSOutput;
}