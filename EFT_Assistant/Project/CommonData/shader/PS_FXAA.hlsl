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

float4 GetTexColor(float2 texCoord, int2 offset = int2(0, 0))
{
    return g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, texCoord, offset);
}
float FxaaLuma(float4 rgba)
{
	//(ax * bx + ay * by + az * bz) NTSC�O���[�X�P�[����
    return dot(rgba.rgb, float3(0.298912f, 0.586611f, 0.114478f));
}

float4 FxaaPixelShader(
    float2 texCoord,
    // �A���S���Y����K�p���邽�߂ɕK�v�ȃ��[�J�� �R���g���X�g�̍ŏ��ʁB
    // 0.333 - ���������� (����)
    // 0.250 - ��i��
    // 0.166 - �f�t�H���g
    // 0.125 - ���i��
    // 0.063 - �I�[�o�[�L�� (�x��)
    float fxaaQualityEdgeThreshold,
    // �Õ��̏�������A���S���Y�����폜���܂��B
    // 0.0833 - ��� (�f�t�H���g�A�t�B���^����Ă��Ȃ��\�������G�b�W�̊J�n�_)
    // 0.0625 - ���i�� (����)
    // 0.0312 - �\������鐧�� (�x��)
    float fxaaQualityEdgeThresholdMin
)
{
	//���͂̍��W�擾�p
    float2 Pos_h = (0.5f / dispsize.x, 0.5f / dispsize.y);
    float2 Pos_t = (2.0f / dispsize.x, 2.0f / dispsize.y);

    float luma_LT = FxaaLuma(GetTexColor(texCoord + float2(-Pos_h.x, -Pos_h.y)));
    float luma_LB = FxaaLuma(GetTexColor(texCoord + float2(-Pos_h.x, Pos_h.y)));
    float luma_RT = FxaaLuma(GetTexColor(texCoord + float2(Pos_h.x, -Pos_h.y))) + 0.002604167;
    float luma_RB = FxaaLuma(GetTexColor(texCoord + float2(Pos_h.x, Pos_h.y)));
    float4 luma_C = GetTexColor(texCoord); //����

	//���͂̍ő�ŏ�
    float luma_Max = max(max(luma_RT, luma_RB), max(luma_LT, luma_LB));
    float luma_Min = min(min(luma_RT, luma_RB), min(luma_LT, luma_LB));

	//�ő�Ɠx��1/8
    float lumaMaxScaledClamped = max(fxaaQualityEdgeThresholdMin, luma_Max * fxaaQualityEdgeThreshold);

	//�O���[�X�P�[��
    float luma_M = FxaaLuma(luma_C);
	//�Ɠx��
    float lumaMaxSubMinM = max(luma_Max, luma_M) - min(luma_Min, luma_M);

	//�ω����r
    if (lumaMaxSubMinM < lumaMaxScaledClamped)
    {
		 //�ω������Ȃ��ꍇ�͌��̐F��Ԃ�
        return luma_C;
    }
    else
    {
		//�e�����̏Ɠx��
        float dirSwMinusNe = luma_LB - luma_RT;
        float dirSeMinusNw = luma_RB - luma_LT;

		//�Ɠx�x�N�g��
        float2 dir1 = normalize(float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw));
		//�Ɠx�x�N�g���̍�
        float2 dirAbsMinTimesC = dir1 / (8.0 * min(abs(dir1.x), abs(dir1.y)));
		//�͈͂Ɏ��߂�
        float2 dir2 = clamp(dirAbsMinTimesC, -2.0, 2.0) * Pos_t;

		//�ړ��ʎZ�o
        dir1 *= Pos_h;
		//�e�����擾
        float4 rgbyN1 = GetTexColor(texCoord - dir1); //���h�b�g�~�Ɠx�x�N�g�� ����
        float4 rgbyP1 = GetTexColor(texCoord + dir1); //���h�b�g�~�Ɠx�x�N�g�� �E��
        float4 rgbyN2 = GetTexColor(texCoord - dir2); //�Q�h�b�g�~�Ɠx�x�N�g�� ����
        float4 rgbyP2 = GetTexColor(texCoord + dir2); //�Q�h�b�g�~�Ɠx�x�N�g�� �E��
		//���Z�����l
        float4 rgbyA = rgbyN1 + rgbyP1; //���Z���Č��ʂ�ۑ�
		//1/4�����l
        float4 rgbyB = (rgbyN2 + rgbyP2 + rgbyA) * 0.25;
		
		//�O���[�X�P�[��
        float rgbyBM = FxaaLuma(rgbyB);
		//���l����
        if ((rgbyBM < luma_Min) || (rgbyBM > luma_Max))
        {
			//1/2��Ԃ�
            return rgbyA * 0.5;
        }
        else
        {
			//1/4��Ԃ�
            return rgbyB;
        }
    }
}

PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;

    PSOutput.color0 = FxaaPixelShader(
        PSInput.texCoords0,
        0.125,
        0.05
    );
    //0.125
    //0.05

    PSOutput.color0.a = 1.0;
#if 1
	return PSOutput;
#else
    float3 Color = GetTexColor(PSInput.texCoords0).xyz;
    if (
        PSOutput.color0.r == Color.r &&
        PSOutput.color0.g == Color.g &&
        PSOutput.color0.b == Color.b
    )
    {
        PSOutput.color0.r = 0.0;
		PSOutput.color0.g = 0.0;
		PSOutput.color0.b = 0.0;
	}
    return PSOutput;
#endif
}
