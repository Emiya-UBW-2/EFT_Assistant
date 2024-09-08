// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 LPPosition : TEXCOORD1; // ライトからみた座標( xとyはライトの射影座標、zはビュー座標 )
	float4 LPPosition2 : TEXCOORD2; // ライトからみた座標( xとyはライトの射影座標、zはビュー座標 )
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
};

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3) {
	float4	g_param;
}

SamplerState g_DepthMapSampler : register(s1); // 深度バッファテクスチャ
Texture2D g_DepthMapTexture : register(t1); // 深度バッファテクスチャ

SamplerState g_DepthMapSampler2 : register(s2); // 深度バッファテクスチャ
Texture2D g_DepthMapTexture2 : register(t2); // 深度バッファテクスチャ

float GetAlpha(float LightDepth, float TextureDepth2) {
	// テクスチャに記録されている深度よりＺ値が大きかったら奥にある
    return (LightDepth > (TextureDepth2 + 1.f) && TextureDepth2 > 0.f) ? 1.f : 0.f;
}
// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;

	float2 DepthTexCoord;
	float comp;
	int total;

	PSOutput.Color0.rgb = 0.f;
	PSOutput.Color0.a = 1.f;


	// 深度テクスチャの座標を算出
    DepthTexCoord.x = (PSInput.LPPosition.x + 1.0f) / 2.0f; // PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
    DepthTexCoord.y = 1.0f - (PSInput.LPPosition.y + 1.0f) / 2.0f; // yは更に上下反転

	// 深度バッファテクスチャから深度を取得( +補正値 )
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

	// 深度テクスチャの座標を算出
	DepthTexCoord.x = (PSInput.LPPosition2.x + 1.0f) / 2.0f; // PSInput.LPPosition2.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
	DepthTexCoord.y = 1.0f - (PSInput.LPPosition2.y + 1.0f) / 2.0f; // yは更に上下反転

	// 深度バッファテクスチャから深度を取得( +補正値 )
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

	// 出力パラメータを返す
    return PSOutput;
}
