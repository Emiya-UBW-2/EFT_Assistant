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

SamplerState g_Diffuse2MapSampler : register(s1); // ディフューズマップサンプラ(ぼかし)
Texture2D g_Diffuse2MapTexture : register(t1); // ディフューズマップテクスチャ(ぼかし)

SamplerState g_DepthMapSampler : register(s2); // 深度マップサンプラ
Texture2D g_DepthMapTexture : register(t2); // 深度マップテクスチャ


PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;

	float per = 0.f;
	float Depth = g_DepthMapTexture.Sample(g_DepthMapSampler, PSInput.texCoords0).r;

	float near_min = caminfo.z;
	float near_max = caminfo.x;
	float far_min = caminfo.y;
	float far_max = caminfo.w;

	//無し
	if (Depth == 0.f) {
		per = 0.f;
	}
	//近
	else if (Depth < near_max) {
		per = 1.f - (Depth - near_min) / (near_max - near_min);
	}
	//中
	else if (near_max < Depth && Depth < far_min) {
		per = 0.f;
	}
	//遠
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