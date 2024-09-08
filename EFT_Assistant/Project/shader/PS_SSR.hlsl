// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT {
	float4 dif         : COLOR0;		// �f�B�t���[�Y�J���[
	float2 texCoords0  : TEXCOORD0;	// �e�N�X�`�����W
	float4 pos         : SV_POSITION;   // ���W( �v���W�F�N�V������� )
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT {
	float4 color0           : SV_TARGET0;	// �F
};

//��ʃT�C�Y
static int2 dispsize = {0, 0};

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE {
	float4		FactorColor;			// �A���t�@�l��

	float		MulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1;

	int		    AlphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2;

	float4		IgnoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1) {
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3) {
	float4 caminfo;
}

cbuffer cbLIGHTCAMERA_MATRIX : register(b4) {
	matrix		g_LightViewMatrix;			// ���C�g�̃��[���h�@���@�r���[�s��
	matrix		g_LightProjectionMatrix;	// ���C�g�̃r���[�@�@���@�ˉe�s��
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER3 : register(b5) {
	float4	g_param;
}


//�e�N�X�`��
SamplerState g_Register0MapSampler : register(s0); // �f�B�t���[�Y�}�b�v�T���v��
Texture2D g_Register0MapTexture : register(t0); // �f�B�t���[�Y�}�b�v�e�N�X�`��

SamplerState g_Register1MapSampler : register(s1); // �@���}�b�v�T���v��
Texture2D g_Register1MapTexture : register(t1); // �@���}�b�v�e�N�X�`��

SamplerState g_Register2MapSampler : register(s2); // �[�x�}�b�v�T���v��
Texture2D g_Register2MapTexture : register(t2); // �[�x�}�b�v�e�N�X�`��

SamplerState dynamicCubeMapSampler     : register(s3);
TextureCube  dynamicCubeMapTexture     : register(t3);

SamplerState FilterSampler		: register(s4);
Texture2D  FilterTexture		: register(t4);

//�֐�
float4 GetTexColor0(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register0MapTexture.Sample(g_Register0MapSampler, texCoord, offset);
}
float4 GetTexColor1(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register1MapTexture.Sample(g_Register1MapSampler, texCoord, offset);
}
float4 GetTexColor2(float2 texCoord, int2 offset = int2(0, 0)) {
	return g_Register2MapTexture.Sample(g_Register2MapSampler, texCoord, offset);
}

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

float2 ProjtoDisp(float3 position) {
	position = position / position.z;

	float2 screenUV;
	screenUV.x = position.x / caminfo.z * dispsize.y / dispsize.x;
	screenUV.y = position.y / caminfo.z;

	screenUV.x = screenUV.x + 1.f;
	screenUV.y = 1.f - screenUV.y;
	screenUV *= 0.5f;
	return screenUV;
}

bool Hitcheck(float3 position) {
	float2 screenUV = ProjtoDisp(position);
	if (
		(abs(screenUV.x) <= 1.f) &&
		(abs(screenUV.y) <= 1.f)
		) {
		float depth = GetTexColor2(screenUV).r;
		float z = depth / (caminfo.y * 0.005f);
		return (position.z < z && z < position.z + caminfo.w);
	}
	else {
		return false;
	}
}

static float maxLength = 5000.f; // ���ˍő勗��
static int BinarySearchIterations = 8; //2���T���ő吔

float4 applySSR(float3 normal, float2 screenUV,bool Skip) {
	float pixelStride;
	float3 delta;
	float3 position;

	float depth = GetTexColor2(screenUV).r;

	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	bool isend = (depth <= 0.f || Skip);
	if (!isend) {
		float3 NormPos = DisptoProjNorm(screenUV);
		pixelStride = maxLength / caminfo.x;
		delta = reflect(NormPos, normal); // ���˃x�N�g��*�P��Ői�ދ���
		position = NormPos * (depth / (caminfo.y * 0.005f)); //����
	}
	[fastopt]
	for (int i = 0; i < caminfo.x; i++) {
		if (!isend) {
			position += delta * pixelStride;

				if (Hitcheck(position)) { //���������̂œ񕪒T��
					position -= delta * pixelStride; //���ɖ߂�
					delta /= BinarySearchIterations; //�i�ޗʂ�������
					[unroll] // attribute
					for (int j = 0; j < BinarySearchIterations; j++) {
						if (!isend) {
							pixelStride *= 0.5f;
							position += delta * pixelStride;
							if (Hitcheck(position)) {
								pixelStride = -pixelStride;
							}
							if (length(pixelStride) < 1.f) {
								isend = true;
							}
						}
					}
					color = GetTexColor0(ProjtoDisp(position));// ���������̂ŐF���u�����h����
					isend = true;
				}
		}
	}
	return color;
}

PS_OUTPUT main(PS_INPUT PSInput) {
	//�߂�l
	PS_OUTPUT PSOutput;
	//��ʃT�C�Y���擾���Ă���
	g_Register0MapTexture.GetDimensions(dispsize.x, dispsize.y);
	//���˂��ǂꂾ�������邩
	float Per = GetTexColor2(PSInput.texCoords0).g * FilterTexture.Sample(FilterSampler, PSInput.texCoords0).r;
	//����
	float4 lWorldPosition;

	//�m�[�}�����W�擾
	float3 normal = GetTexColor1(PSInput.texCoords0).xyz * 2.f - 1.f;

	//�L���[�u�}�b�v����̔���
	lWorldPosition.xyz = DisptoProjNorm(PSInput.texCoords0);
	lWorldPosition.x *= -1.f;
	lWorldPosition.w = 0.f;
	
	// ���[���h���W���ˉe���W�ɕϊ�
	float4 LPPosition1 = mul(g_LightViewMatrix, lWorldPosition);
	LPPosition1.z *= -1.f;

	lWorldPosition.xyz = normal;
	lWorldPosition.w = 0.f;

	// ���[���h���W���ˉe���W�ɕϊ�
	float4 LPPosition2 = mul(g_LightViewMatrix, lWorldPosition);

	float3 LPPosition3 = reflect(LPPosition1.xyz, LPPosition2.xyz);
	LPPosition3.xz *= -1.f;
	PSOutput.color0 = dynamicCubeMapTexture.Sample(dynamicCubeMapSampler, LPPosition3);

	if (g_param.x >= 2) {
		float4 color = applySSR(normal, PSInput.texCoords0, (Per <= 0.f));
		if (color.a > 0.f) {
			PSOutput.color0 = color;
		}
	}
	PSOutput.color0 = lerp(float4(0.f, 0.f, 0.f, 0.f), PSOutput.color0, Per);

	//�߂�l
	//return PSOutput;
	//�����������o�͂���ꍇ�͂�����
	float3 Color = GetTexColor0(PSInput.texCoords0).xyz;
	if (
		PSOutput.color0.r == Color.r &&
		PSOutput.color0.g == Color.g &&
		PSOutput.color0.b == Color.b
		) {
		PSOutput.color0.a = 0.0;
	}
	return PSOutput;
}
