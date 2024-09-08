// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 LPPosition : TEXCOORD1; // ���C�g����݂����W( x��y�̓��C�g�̎ˉe���W�Az�̓r���[���W )
	float4 LPPosition2 : TEXCOORD2; // ���C�g����݂����W( x��y�̓��C�g�̎ˉe���W�Az�̓r���[���W )
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // �F
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3) {
	float4	g_param;
}

SamplerState g_DepthMapSampler : register(s1); // �[�x�o�b�t�@�e�N�X�`��
Texture2D g_DepthMapTexture : register(t1); // �[�x�o�b�t�@�e�N�X�`��

SamplerState g_DepthMapSampler2 : register(s2); // �[�x�o�b�t�@�e�N�X�`��
Texture2D g_DepthMapTexture2 : register(t2); // �[�x�o�b�t�@�e�N�X�`��

float GetAlpha(float LightDepth, float TextureDepth2) {
	// �e�N�X�`���ɋL�^����Ă���[�x���y�l���傫�������牜�ɂ���
    return (LightDepth > (TextureDepth2 + 1.f) && TextureDepth2 > 0.f) ? 1.f : 0.f;
}
// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;

	float2 DepthTexCoord;
	float comp;
	int total;

	PSOutput.Color0.rgb = 0.f;
	PSOutput.Color0.a = 1.f;


	// �[�x�e�N�X�`���̍��W���Z�o
    DepthTexCoord.x = (PSInput.LPPosition.x + 1.0f) / 2.0f; // PSInput.LPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
    DepthTexCoord.y = 1.0f - (PSInput.LPPosition.y + 1.0f) / 2.0f; // y�͍X�ɏ㉺���]

	// �[�x�o�b�t�@�e�N�X�`������[�x���擾( +�␳�l )
	int xs = 3;
	{
		comp = 0;
		total = 0;
		[fastopt]
		for (int x = -xs;x <= xs;x++) {
			[fastopt]
			for (int y = -xs;y <= xs;y++) {
				if (abs(x) < (int)g_param.x && abs(y) < (int)g_param.x) {
					comp += GetAlpha(PSInput.LPPosition.z, g_DepthMapTexture.Sample(g_DepthMapSampler, DepthTexCoord, int2(x, y)).r);
					total++;
				}
			}
		}
        if (total > 0)
        {
            PSOutput.Color0.r = max(PSOutput.Color0.r, comp / total);
        }
    }

	// �[�x�e�N�X�`���̍��W���Z�o
	DepthTexCoord.x = (PSInput.LPPosition2.x + 1.0f) / 2.0f; // PSInput.LPPosition2.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
	DepthTexCoord.y = 1.0f - (PSInput.LPPosition2.y + 1.0f) / 2.0f; // y�͍X�ɏ㉺���]

	// �[�x�o�b�t�@�e�N�X�`������[�x���擾( +�␳�l )
	{
		comp = 0;
		total = 0;
		[fastopt]
		for (int x = -xs;x <= xs;x++) {
			[fastopt]
			for (int y = -xs;y <= xs;y++) {
				if (abs(x) < (int)g_param.x && abs(y) < (int)g_param.x) {
					comp += GetAlpha(PSInput.LPPosition2.z, g_DepthMapTexture2.Sample(g_DepthMapSampler2, DepthTexCoord, int2(x, y)).r);
					total++;
				}
			}
        }
        if (total > 0)
        {
            PSOutput.Color0.r = max(PSOutput.Color0.r, comp / total);
        }
    }
	
    if ((int)g_param.x>2 && PSOutput.Color0.r < 128.f / 255.f)
    {
        PSOutput.Color0.r = 0.f;

    }

	// �o�̓p�����[�^��Ԃ�
    return PSOutput;
}
