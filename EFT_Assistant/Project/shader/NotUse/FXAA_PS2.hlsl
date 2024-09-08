/*============================================================================
                    NVIDIA FXAA 3.11 by TIMOTHY LOTTES
------------------------------------------------------------------------------
COPYRIGHT (C) 2010, 2011 NVIDIA CORPORATION. ALL RIGHTS RESERVED.
------------------------------------------------------------------------------
------------------------------------------------------------------------------
                           INTEGRATION CHECKLIST
------------------------------------------------------------------------------
(2.)
���ɁA���̃t�@�C�����C���N���[�h���܂��B

  #include "Fxaa3_11.h"

(4.)
FXAA �o�� RGBL �̑O�Ƀp�X��ۏ؂��܂� (���̃Z�N�V�������Q��)�B
�܂��́A�g�p���āA

  #define FXAA_GREEN_AS_LUMA 1

(6.)
FXAA ���_�V�F�[�_�[���t���X�N���[���̎O�p�`�Ƃ��Ď��s���܂��B
�upos�v�ƁufxaaConsolePosPos�v���o�͂��܂�
�s�N�Z�� �V�F�[�_�̓��͂��񋟂���悤�ɁA

  // {xy} = �s�N�Z���̒��S
  FxaaFloat2 �ʒu�A

  // {xy__} = �s�N�Z���̍���
  // {__zw} = �s�N�Z���̉E��
  FxaaFloat4 fxaaConsolePosPos�A

�i7�B�j
FXAA �Ŏg�p�����e�N�X�`�� �T���v���[���o�C���j�A �t�B���^�����O�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B

------------------------------------------------------------------------------
                    INTEGRATION - RGBL AND COLORSPACE
------------------------------------------------------------------------------
�ȉ����ݒ肳��Ă��Ȃ�����AFXAA3 �͓��͂Ƃ��� RGBL ��K�v�Ƃ��܂��B

  #define FXAA_GREEN_AS_LUMA 1

���̏ꍇ�A�G���W���͋P�x�̑���ɗΐF���g�p���܂��B
����`�F��Ԃ� RGB ���͂��K�v�ł��B

RGB �� LDR (���[ �_�C�i�~�b�N �����W) �ł���K�v������܂��B
��̓I�ɂ́A�g�[���}�b�s���O��� FXAA �����s���܂��B

�e�N�X�`���̃t�F�b�`�ɂ���ĕԂ���� RGB �f�[�^�͔���`�ł���ꍇ������܂��B
FXAA_GREEN_AS_LUMA ���ݒ肳��Ă��Ȃ��ꍇ�͐��`�ɂȂ�܂��B
�usRGB �`���v�e�N�X�`���͐��`�Ƃ��ăJ�E���g����邱�Ƃɒ��ӂ��Ă��������B
�e�N�X�`���t�F�b�`�̌��ʂ͐��`�f�[�^�ł��邽�߂ł��B
sRGB �J���[�X�y�[�X�̒ʏ�́uRGBA8�v�e�N�X�`���͔���`�ł��B

FXAA_GREEN_AS_LUMA ���ݒ肳��Ă��Ȃ��ꍇ�A
luma �́AFXAA �����s����O�ɃA���t�@ �`���l���ɕۑ�����K�v������܂��B
���̋P�x�͒m�o��ԓ��ɂ���K�v������܂� (�K���} 2.0 �̉\��������܂�)�B
�o�͂��K���} 2.0 �ŃG���R�[�h����Ă��� FXAA �̑O�̗�B

  color.rgb = �g�[���}�b�v(color.rgb); // ���j�A�J���[�o��
  color.rgb = sqrt(color.rgb); // �K���} 2.0 �J���[�o��
  �F��Ԃ�;

FXAA���g�p����ɂ́A

  color.rgb = �g�[���}�b�v(color.rgb); // ���j�A�J���[�o��
  color.rgb = sqrt(color.rgb); // �K���} 2.0 �J���[�o��
  color.a = dot(color.rgb, float3(0.299, 0.587, 0.114)); // ���~�i���X���v�Z����
  �F��Ԃ�;

�o�͂����`�G���R�[�h�����ʂ̗�:
���Ƃ��΁AsRGB �`���̃����_�[ �^�[�Q�b�g�ɏ������ޏꍇ�A
�����ŁA�����_�[ �^�[�Q�b�g�̓u�����h��� sRGB �ւ̕ϊ���߂��܂��B

  color.rgb = �g�[���}�b�v(color.rgb); // ���j�A�J���[�o��
  �F��Ԃ�;

FXAA���g�p����ɂ́A

  color.rgb = �g�[���}�b�v(color.rgb); // ���j�A�J���[�o��
  color.a = sqrt(dot(color.rgb, float3(0.299, 0.587, 0.114))); // ���~�i���X���v�Z����
  �F��Ԃ�;

�A���S���Y�������������삷��ɂ́A�P�x�𐳂����擾����K�v������܂��B
------------------------------------------------------------------------------
                          BEING LINEARLY CORRECT?
------------------------------------------------------------------------------
FXAA �����j�A RGB �J���[�̃t���[���o�b�t�@�ɓK�p����ƁA�����ڂ������Ȃ�܂��B
����͔��ɒ��ςɔ����܂����A���̏ꍇ�͂��܂��ܓ��Ă͂܂�܂��B
���̗��R�́A�f�B�U�����O�A�[�e�B�t�@�N�g�����ڗ����߂ł��B
���`�F��ԂŁB

------------------------------------------------------------------------------
                             COMPLEX INTEGRATION
------------------------------------------------------------------------------
Q. FXAA �����s����O�ɃG���W���� RGB �Ƀu�����h����Ă���ꍇ�͂ǂ��Ȃ�܂���?

A. FXAA �O�̍Ō�̕s�����ȃp�X�ł́A
   �p�X�Ń��}���A���t�@�ɏ����o���悤�ɂ��܂��B
   ����RGB�݂̂Ƀu�����h���܂��B
   FXAA �͐���Ɏ��s�ł���͂��ł�
   �u�����f�B���O�p�X�ŃG�C���A�V���O���ǉ�����Ȃ������Ɖ��肵�܂��B
   ����́A�p�[�e�B�N���ƈ�ʓI�ȃu�����h �p�X�̈�ʓI�ȃP�[�X�ł��B

A. �܂��́AFXAA_GREEN_AS_LUMA ���g�p���܂��B

============================================================================*/

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


#define HLSL_3 0
//                           FXAA QUALITY - PRESETS
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 4.0
#define FXAA_QUALITY__P5 12.0

#if (HLSL_3 == 1)
#define FxaaInt2 float2
#else
#define FxaaInt2 int2
#endif

float4 GetTexColor(float2 texCoord, FxaaInt2 offset = FxaaInt2(0, 0))
{
#if (HLSL_3 == 1)
    sampler2D t;
    t.smpl = g_DiffuseMapSampler;
    t.tex = g_DiffuseMapTexture;
    return tex2Dlod(t, float4(texCoord + (offset * fxaaQualityRcpFrame.xy), 0, 0));
#else
    return g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, texCoord, offset);
#endif
}

float FxaaLuma(float4 rgba)
{
    return dot(rgba.rgb, float3(0.298912f, 0.586611f, 0.114478f));
}


/*--------------------------------------------------------------------------*/
float4 FxaaPixelShader(
    float2 pos, // �����ł͓�����Ԃ��g�p���܂��� (������Ԃ��I�t�ɂ��܂�)�B
    float2 fxaaQualityRcpFrame, // FXAA �i���ł̂ݎg�p����܂��B�萔/���j�t�H�[������̂��̂łȂ���΂Ȃ�܂���B (1.0/screenWidthInPixels, 1.0/screenHeightInPixels)
    float fxaaQualitySubpix, // �T�u�s�N�Z���̃G�C���A�V���O�����̗ʂ�I�����܂��B����͑N�����ɉe����^����\��������܂��B 0.00 ~ 1.00
    float fxaaQualityEdgeThreshold, // �A���S���Y����K�p���邽�߂ɕK�v�ȃ��[�J�� �R���g���X�g�̍ŏ��ʁB
    float fxaaQualityEdgeThresholdMin // �Õ��̏�������A���S���Y�����폜���܂��B
)
{
/*--------------------------------------------------------------------------*/
    float4 luma_C = GetTexColor(pos);
    float lumaS = FxaaLuma(GetTexColor(pos, FxaaInt2(0, 1)));
    float lumaE = FxaaLuma(GetTexColor(pos, FxaaInt2(1, 0)));
    float lumaN = FxaaLuma(GetTexColor(pos, FxaaInt2(0, -1)));
    float lumaW = FxaaLuma(GetTexColor(pos, FxaaInt2(-1, 0)));
/*--------------------------------------------------------------------------*/
	//�O���[�X�P�[��
    float lumaM = FxaaLuma(luma_C);
	//���͂̍ő�ŏ�
    float luma_Max = max(max(lumaN, lumaW), max(lumaE, max(lumaS, lumaM)));
    float luma_Min = min(min(lumaN, lumaW), min(lumaE, min(lumaS, lumaM)));
    
   	//�ő�Ɠx��1/8
    float lumaMaxScaledClamped = max(fxaaQualityEdgeThresholdMin, luma_Max * fxaaQualityEdgeThreshold);

   	//�Ɠx��
    float lumaMaxSubMinM = luma_Max - luma_Min;

	//�ω����r
    if (lumaMaxSubMinM < lumaMaxScaledClamped)
    {
        //�ω������Ȃ��ꍇ�͌��̐F��Ԃ�
        return luma_C;
    }
/*--------------------------------------------------------------------------*/
    float lumaNW = FxaaLuma(GetTexColor(pos, FxaaInt2(-1, -1)));
    float lumaSE = FxaaLuma(GetTexColor(pos, FxaaInt2(1, 1)));
    float lumaNE = FxaaLuma(GetTexColor(pos, FxaaInt2(1, -1)));
    float lumaSW = FxaaLuma(GetTexColor(pos, FxaaInt2(-1, 1)));
/*--------------------------------------------------------------------------*/
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float subpixNSWE = lumaNS + lumaWE;
    float subpixNWSWNESE = lumaNWSW + lumaNESE;
/*--------------------------------------------------------------------------*/
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;
/*--------------------------------------------------------------------------*/
    float lengthSign = 0.0;
    bool horzSpan = edgeHorz >= edgeVert;
/*--------------------------------------------------------------------------*/
    if (!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
        lengthSign = fxaaQualityRcpFrame.x;
    }
    else
    {
        lengthSign = fxaaQualityRcpFrame.y;
    }
    float subpixB = ((subpixNSWE * 2.0 + subpixNWSWNESE) * (1.0 / 12.0)) - lumaM;
/*--------------------------------------------------------------------------*/
    float gradientN = lumaN - lumaM;
    float gradientS = lumaS - lumaM;
    float lumaNN = lumaN + lumaM;
    float lumaSS = lumaS + lumaM;
    bool pairN = abs(gradientN) >= abs(gradientS);
    float gradient = max(abs(gradientN), abs(gradientS));
    if (pairN)
    {
        lengthSign = -lengthSign;
    }
    float subpixC = saturate(abs(subpixB) * (1.0 / lumaMaxSubMinM));
/*--------------------------------------------------------------------------*/
    float2 posB;
    posB.x = pos.x;
    posB.y = pos.y;
    float2 offNP;
    if (!horzSpan)
    {
        offNP.x = 0;
        offNP.y = fxaaQualityRcpFrame.y;
        posB.x += lengthSign * 0.5;
    }
    else
    {
        offNP.x = fxaaQualityRcpFrame.x;
        offNP.y = 0;
        posB.y += lengthSign * 0.5;
    }
/*--------------------------------------------------------------------------*/
    float2 posN;
    posN.x = posB.x - offNP.x * FXAA_QUALITY__P0;
    posN.y = posB.y - offNP.y * FXAA_QUALITY__P0;
    float2 posP;
    posP.x = posB.x + offNP.x * FXAA_QUALITY__P0;
    posP.y = posB.y + offNP.y * FXAA_QUALITY__P0;
    float subpixD = ((-2.0) * subpixC) + 3.0;
    float lumaEndN = FxaaLuma(GetTexColor(posN));
    float subpixE = subpixC * subpixC;
    float lumaEndP = FxaaLuma(GetTexColor(posP));
/*--------------------------------------------------------------------------*/
    if (!pairN)
        lumaNN = lumaSS;
    float gradientScaled = gradient * 1.0 / 4.0;
    float lumaMM = lumaM - lumaNN * 0.5;
    float subpixF = subpixD * subpixE;
    bool lumaMLTZero = lumaMM < 0.0;
/*--------------------------------------------------------------------------*/
    lumaEndN -= lumaNN * 0.5;
    lumaEndP -= lumaNN * 0.5;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if (!doneN)
        posN.x -= offNP.x * FXAA_QUALITY__P1;
    if (!doneN)
        posN.y -= offNP.y * FXAA_QUALITY__P1;
    bool doneNP = (!doneN) || (!doneP);
    if (!doneP)
        posP.x += offNP.x * FXAA_QUALITY__P1;
    if (!doneP)
        posP.y += offNP.y * FXAA_QUALITY__P1;
    if (doneNP)
    {
        if (!doneN)
            lumaEndN = FxaaLuma(GetTexColor(posN.xy));
        if (!doneP)
            lumaEndP = FxaaLuma(GetTexColor(posP.xy));
        if (!doneN)
            lumaEndN = lumaEndN - lumaNN * 0.5;
        if (!doneP)
            lumaEndP = lumaEndP - lumaNN * 0.5;
        doneN = abs(lumaEndN) >= gradientScaled;
        doneP = abs(lumaEndP) >= gradientScaled;
        if (!doneN)
            posN.x -= offNP.x * FXAA_QUALITY__P2;
        if (!doneN)
            posN.y -= offNP.y * FXAA_QUALITY__P2;
        doneNP = (!doneN) || (!doneP);
        if (!doneP)
            posP.x += offNP.x * FXAA_QUALITY__P2;
        if (!doneP)
            posP.y += offNP.y * FXAA_QUALITY__P2;
        if (doneNP)
        {
            if (!doneN)
                lumaEndN = FxaaLuma(GetTexColor(posN.xy));
            if (!doneP)
                lumaEndP = FxaaLuma(GetTexColor(posP.xy));
            if (!doneN)
                lumaEndN = lumaEndN - lumaNN * 0.5;
            if (!doneP)
                lumaEndP = lumaEndP - lumaNN * 0.5;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if (!doneN)
                posN.x -= offNP.x * FXAA_QUALITY__P3;
            if (!doneN)
                posN.y -= offNP.y * FXAA_QUALITY__P3;
            doneNP = (!doneN) || (!doneP);
            if (!doneP)
                posP.x += offNP.x * FXAA_QUALITY__P3;
            if (!doneP)
                posP.y += offNP.y * FXAA_QUALITY__P3;
            if (doneNP)
            {
                if (!doneN)
                    lumaEndN = FxaaLuma(GetTexColor(posN.xy));
                if (!doneP)
                    lumaEndP = FxaaLuma(GetTexColor(posP.xy));
                if (!doneN)
                    lumaEndN = lumaEndN - lumaNN * 0.5;
                if (!doneP)
                    lumaEndP = lumaEndP - lumaNN * 0.5;
                doneN = abs(lumaEndN) >= gradientScaled;
                doneP = abs(lumaEndP) >= gradientScaled;
                if (!doneN)
                    posN.x -= offNP.x * FXAA_QUALITY__P4;
                if (!doneN)
                    posN.y -= offNP.y * FXAA_QUALITY__P4;
                doneNP = (!doneN) || (!doneP);
                if (!doneP)
                    posP.x += offNP.x * FXAA_QUALITY__P4;
                if (!doneP)
                    posP.y += offNP.y * FXAA_QUALITY__P4;
                if (doneNP)
                {
                    if (!doneN)
                        lumaEndN = FxaaLuma(GetTexColor(posN.xy));
                    if (!doneP)
                        lumaEndP = FxaaLuma(GetTexColor(posP.xy));
                    if (!doneN)
                        lumaEndN = lumaEndN - lumaNN * 0.5;
                    if (!doneP)
                        lumaEndP = lumaEndP - lumaNN * 0.5;
                    doneN = abs(lumaEndN) >= gradientScaled;
                    doneP = abs(lumaEndP) >= gradientScaled;
                    if (!doneN)
                        posN.x -= offNP.x * FXAA_QUALITY__P5;
                    if (!doneN)
                        posN.y -= offNP.y * FXAA_QUALITY__P5;
                    doneNP = (!doneN) || (!doneP);
                    if (!doneP)
                        posP.x += offNP.x * FXAA_QUALITY__P5;
                    if (!doneP)
                        posP.y += offNP.y * FXAA_QUALITY__P5;
                }
            }
        }
    }
/*--------------------------------------------------------------------------*/
    float dstN = pos.x - posN.x;
    float dstP = posP.x - pos.x;
    if (!horzSpan)
    {
        dstN = pos.y - posN.y;
        dstP = posP.y - pos.y;
    }
    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
    bool goodSpan = (dstN < dstP) ? goodSpanN : goodSpanP;
    float pixelOffset = (min(dstN, dstP) * (-1.0 / (dstP + dstN))) + 0.5;
/*--------------------------------------------------------------------------*/
    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
    float pixelOffsetSubpix = max(pixelOffsetGood, (subpixF * subpixF) * fxaaQualitySubpix);
    float2 posM;
    posM.x = pos.x;
    posM.y = pos.y;
    if (!horzSpan)
    {
        posM.x += pixelOffsetSubpix * lengthSign;
    }
    else
    {
        posM.y += pixelOffsetSubpix * lengthSign;
    }
    return float4(GetTexColor(posM).xyz, lumaM);
}
/*==========================================================================*/


PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    uint dx, dy;
    g_DiffuseMapTexture.GetDimensions(dx, dy);
    float2 rcpro = rcp(float2(dx, dy));

    PSOutput.color0 = FxaaPixelShader(
        PSInput.texCoords0,
        rcpro,
        1.0,
        0.166,
        0.0312
        );
    PSOutput.color0.a = 1.0;

    
    float3 Color = GetTexColor(PSInput.texCoords0).xyz;
    if (
        PSOutput.color0.r == Color.r &&
        PSOutput.color0.g == Color.g &&
        PSOutput.color0.b == Color.b
    )
    {
        //PSOutput.color0.a = 0.0;
    }
    return PSOutput;
}
