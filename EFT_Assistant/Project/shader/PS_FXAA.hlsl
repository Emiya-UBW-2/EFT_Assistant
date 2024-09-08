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

float4 GetTexColor(float2 texCoord, int2 offset = int2(0, 0))
{
    return g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, texCoord, offset);
}
float FxaaLuma(float4 rgba)
{
	//(ax * bx + ay * by + az * bz) NTSCグレースケール化
    return dot(rgba.rgb, float3(0.298912f, 0.586611f, 0.114478f));
}

float4 FxaaPixelShader(
    float2 texCoord,
    // アルゴリズムを適用するために必要なローカル コントラストの最小量。
    // 0.333 - 小さすぎる (高速)
    // 0.250 - 低品質
    // 0.166 - デフォルト
    // 0.125 - 高品質
    // 0.063 - オーバーキル (遅い)
    float fxaaQualityEdgeThreshold,
    // 暗部の処理からアルゴリズムを削除します。
    // 0.0833 - 上限 (デフォルト、フィルタされていない表示されるエッジの開始点)
    // 0.0625 - 高品質 (高速)
    // 0.0312 - 表示される制限 (遅い)
    float fxaaQualityEdgeThresholdMin
)
{
	//周囲の座標取得用
    float2 Pos_h = (0.5f / dispsize.x, 0.5f / dispsize.y);
    float2 Pos_t = (2.0f / dispsize.x, 2.0f / dispsize.y);

    float luma_LT = FxaaLuma(GetTexColor(texCoord + float2(-Pos_h.x, -Pos_h.y)));
    float luma_LB = FxaaLuma(GetTexColor(texCoord + float2(-Pos_h.x, Pos_h.y)));
    float luma_RT = FxaaLuma(GetTexColor(texCoord + float2(Pos_h.x, -Pos_h.y))) + 0.002604167;
    float luma_RB = FxaaLuma(GetTexColor(texCoord + float2(Pos_h.x, Pos_h.y)));
    float4 luma_C = GetTexColor(texCoord); //中央

	//周囲の最大最小
    float luma_Max = max(max(luma_RT, luma_RB), max(luma_LT, luma_LB));
    float luma_Min = min(min(luma_RT, luma_RB), min(luma_LT, luma_LB));

	//最大照度の1/8
    float lumaMaxScaledClamped = max(fxaaQualityEdgeThresholdMin, luma_Max * fxaaQualityEdgeThreshold);

	//グレースケール
    float luma_M = FxaaLuma(luma_C);
	//照度差
    float lumaMaxSubMinM = max(luma_Max, luma_M) - min(luma_Min, luma_M);

	//変化を比較
    if (lumaMaxSubMinM < lumaMaxScaledClamped)
    {
		 //変化が少ない場合は元の色を返す
        return luma_C;
    }
    else
    {
		//各方向の照度差
        float dirSwMinusNe = luma_LB - luma_RT;
        float dirSeMinusNw = luma_RB - luma_LT;

		//照度ベクトル
        float2 dir1 = normalize(float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw));
		//照度ベクトルの差
        float2 dirAbsMinTimesC = dir1 / (8.0 * min(abs(dir1.x), abs(dir1.y)));
		//範囲に収める
        float2 dir2 = clamp(dirAbsMinTimesC, -2.0, 2.0) * Pos_t;

		//移動量算出
        dir1 *= Pos_h;
		//各方向取得
        float4 rgbyN1 = GetTexColor(texCoord - dir1); //半ドット×照度ベクトル 左上
        float4 rgbyP1 = GetTexColor(texCoord + dir1); //半ドット×照度ベクトル 右下
        float4 rgbyN2 = GetTexColor(texCoord - dir2); //２ドット×照度ベクトル 左上
        float4 rgbyP2 = GetTexColor(texCoord + dir2); //２ドット×照度ベクトル 右下
		//加算した値
        float4 rgbyA = rgbyN1 + rgbyP1; //加算して結果を保存
		//1/4した値
        float4 rgbyB = (rgbyN2 + rgbyP2 + rgbyA) * 0.25;
		
		//グレースケール
        float rgbyBM = FxaaLuma(rgbyB);
		//半値分岐
        if ((rgbyBM < luma_Min) || (rgbyBM > luma_Max))
        {
			//1/2を返す
            return rgbyA * 0.5;
        }
        else
        {
			//1/4を返す
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
