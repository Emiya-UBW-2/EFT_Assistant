/*============================================================================
                    NVIDIA FXAA 3.11 by TIMOTHY LOTTES
------------------------------------------------------------------------------
COPYRIGHT (C) 2010, 2011 NVIDIA CORPORATION. ALL RIGHTS RESERVED.
------------------------------------------------------------------------------
------------------------------------------------------------------------------
                           INTEGRATION CHECKLIST
------------------------------------------------------------------------------
(2.)
次に、このファイルをインクルードします。

  #include "Fxaa3_11.h"

(4.)
FXAA 出力 RGBL の前にパスを保証します (次のセクションを参照)。
または、使用して、

  #define FXAA_GREEN_AS_LUMA 1

(6.)
FXAA 頂点シェーダーをフルスクリーンの三角形として実行します。
「pos」と「fxaaConsolePosPos」を出力します
ピクセル シェーダの入力が提供するように、

  // {xy} = ピクセルの中心
  FxaaFloat2 位置、

  // {xy__} = ピクセルの左上
  // {__zw} = ピクセルの右下
  FxaaFloat4 fxaaConsolePosPos、

（7。）
FXAA で使用されるテクスチャ サンプラーがバイリニア フィルタリングに設定されていることを確認します。

------------------------------------------------------------------------------
                    INTEGRATION - RGBL AND COLORSPACE
------------------------------------------------------------------------------
以下が設定されていない限り、FXAA3 は入力として RGBL を必要とします。

  #define FXAA_GREEN_AS_LUMA 1

この場合、エンジンは輝度の代わりに緑色を使用します。
非線形色空間の RGB 入力が必要です。

RGB は LDR (ロー ダイナミック レンジ) である必要があります。
具体的には、トーンマッピング後に FXAA を実行します。

テクスチャのフェッチによって返される RGB データは非線形である場合があります。
FXAA_GREEN_AS_LUMA が設定されていない場合は線形になります。
「sRGB 形式」テクスチャは線形としてカウントされることに注意してください。
テクスチャフェッチの結果は線形データであるためです。
sRGB カラースペースの通常の「RGBA8」テクスチャは非線形です。

FXAA_GREEN_AS_LUMA が設定されていない場合、
luma は、FXAA を実行する前にアルファ チャネルに保存する必要があります。
この輝度は知覚空間内にある必要があります (ガンマ 2.0 の可能性があります)。
出力がガンマ 2.0 でエンコードされている FXAA の前の例。

  color.rgb = トーンマップ(color.rgb); // リニアカラー出力
  color.rgb = sqrt(color.rgb); // ガンマ 2.0 カラー出力
  色を返す;

FXAAを使用するには、

  color.rgb = トーンマップ(color.rgb); // リニアカラー出力
  color.rgb = sqrt(color.rgb); // ガンマ 2.0 カラー出力
  color.a = dot(color.rgb, float3(0.299, 0.587, 0.114)); // ルミナンスを計算する
  色を返す;

出力が線形エンコードされる別の例:
たとえば、sRGB 形式のレンダー ターゲットに書き込む場合、
ここで、レンダー ターゲットはブレンド後に sRGB への変換を戻します。

  color.rgb = トーンマップ(color.rgb); // リニアカラー出力
  色を返す;

FXAAを使用するには、

  color.rgb = トーンマップ(color.rgb); // リニアカラー出力
  color.a = sqrt(dot(color.rgb, float3(0.299, 0.587, 0.114))); // ルミナンスを計算する
  色を返す;

アルゴリズムが正しく動作するには、輝度を正しく取得する必要があります。
------------------------------------------------------------------------------
                          BEING LINEARLY CORRECT?
------------------------------------------------------------------------------
FXAA をリニア RGB カラーのフレームバッファに適用すると、見た目が悪くなります。
これは非常に直観に反しますが、この場合はたまたま当てはまります。
その理由は、ディザリングアーティファクトがより目立つためです。
線形色空間で。

------------------------------------------------------------------------------
                             COMPLEX INTEGRATION
------------------------------------------------------------------------------
Q. FXAA を実行する前にエンジンが RGB にブレンドされている場合はどうなりますか?

A. FXAA 前の最後の不透明なパスでは、
   パスでルマをアルファに書き出すようにします。
   次にRGBのみにブレンドします。
   FXAA は正常に実行できるはずです
   ブレンディングパスでエイリアシングが追加されなかったと仮定します。
   これは、パーティクルと一般的なブレンド パスの一般的なケースです。

A. または、FXAA_GREEN_AS_LUMA を使用します。

============================================================================*/

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 dif : COLOR0; // ディフューズカラー
    float2 texCoords0 : TEXCOORD0; // テクスチャ座標
    float4 pos : SV_POSITION; // 座標( プロジェクション空間 )
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 color0 : SV_TARGET0; // 色
};


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
    float4 FactorColor; // アルファ値等

    float MulAlphaColor; // カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
    float AlphaTestRef; // アルファテストで使用する比較値
    float2 Padding1;

    int AlphaTestCmpMode; // アルファテスト比較モード( DX_CMP_NEVER など )
    int3 Padding2;

    float4 IgnoreTextureColor; // テクスチャカラー無視処理用カラー
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    DX_D3D11_PS_CONST_BUFFER_BASE g_Base;
};

// プログラムとのやり取りのために使うレジスタ1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
    float2 dispsize;
}

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
    float4 caminfo;
}

SamplerState g_DiffuseMapSampler : register(s0); // ディフューズマップサンプラ
Texture2D g_DiffuseMapTexture : register(t0); // ディフューズマップテクスチャ


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
    float2 pos, // ここでは透視補間を使用しません (透視補間をオフにします)。
    float2 fxaaQualityRcpFrame, // FXAA 品質でのみ使用されます。定数/ユニフォームからのものでなければなりません。 (1.0/screenWidthInPixels, 1.0/screenHeightInPixels)
    float fxaaQualitySubpix, // サブピクセルのエイリアシング除去の量を選択します。これは鮮明さに影響を与える可能性があります。 0.00 ~ 1.00
    float fxaaQualityEdgeThreshold, // アルゴリズムを適用するために必要なローカル コントラストの最小量。
    float fxaaQualityEdgeThresholdMin // 暗部の処理からアルゴリズムを削除します。
)
{
/*--------------------------------------------------------------------------*/
    float4 luma_C = GetTexColor(pos);
    float lumaS = FxaaLuma(GetTexColor(pos, FxaaInt2(0, 1)));
    float lumaE = FxaaLuma(GetTexColor(pos, FxaaInt2(1, 0)));
    float lumaN = FxaaLuma(GetTexColor(pos, FxaaInt2(0, -1)));
    float lumaW = FxaaLuma(GetTexColor(pos, FxaaInt2(-1, 0)));
/*--------------------------------------------------------------------------*/
	//グレースケール
    float lumaM = FxaaLuma(luma_C);
	//周囲の最大最小
    float luma_Max = max(max(lumaN, lumaW), max(lumaE, max(lumaS, lumaM)));
    float luma_Min = min(min(lumaN, lumaW), min(lumaE, min(lumaS, lumaM)));
    
   	//最大照度の1/8
    float lumaMaxScaledClamped = max(fxaaQualityEdgeThresholdMin, luma_Max * fxaaQualityEdgeThreshold);

   	//照度差
    float lumaMaxSubMinM = luma_Max - luma_Min;

	//変化を比較
    if (lumaMaxSubMinM < lumaMaxScaledClamped)
    {
        //変化が少ない場合は元の色を返す
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
