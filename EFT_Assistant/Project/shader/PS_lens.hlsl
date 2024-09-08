// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 dif         : COLOR0;		// ディフューズカラー
	float2 texCoords0  : TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color0           : SV_TARGET0;	// 色
};


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// アルファ値等

	float		MulAlphaColor;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef;			// アルファテストで使用する比較値
	float2		Padding1;

	int			AlphaTestCmpMode;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2;

	float4		IgnoreTextureColor;	// テクスチャカラー無視処理用カラー
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// プログラムとのやり取りのために使うレジスタ1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
	float2	dispsize;
}

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler            : register(s0);		// ディフューズマップサンプラ
Texture2D    g_DiffuseMapTexture            : register(t0);		// ディフューズマップテクスチャ

//ブラックホールによる屈折の計算を行う関数(ブラックホールの位置、計算対象のピクセル位置)
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

// main関数
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

	// テクスチャカラーの読み込み
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, CalcPos);

	// 出力カラー = テクスチャカラー * ディフューズカラー
	PSOutput.color0 = TextureDiffuseColor * PSInput.dif;

	// 出力パラメータを返す
	return PSOutput;
}

