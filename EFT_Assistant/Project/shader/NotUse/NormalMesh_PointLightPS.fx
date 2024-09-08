// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float4 PositionSub	   : TEXCOORD1;	// 座標( 射影空間 )ピクセルシェーダーで参照する為の物
	float4 VPosition	   : TEXCOORD2;	// 座標( 射影空間 )ピクセルシェーダーで参照する為の物
	float4 Position        : SV_POSITION;	// 座標( プロジェクション空間 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// 色
} ;


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// アルファ値等

	float		MulAlphaColor ;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef ;			// アルファテストで使用する比較値
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// テクスチャカラー無視処理用カラー
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler            : register(s0) ;		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register(t0) ;		// ディフューズマップテクスチャ
SamplerState g_DepthMapSampler			    : register(s2) ;		// 深度
Texture2D    g_DepthMapTexture	            : register(t2) ;		// 深度



// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 Depth;
	float MaxOpacityDistance = lenspos.x;
	float2 TexCoords;
	//*
	// テクスチャカラーの読み込み
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0 ) ;

	// 深度テクスチャ座標の算出
	TexCoords.x = (PSInput.PositionSub.x / PSInput.PositionSub.w + 1.0f) / 2.0f;
	TexCoords.y = (-PSInput.PositionSub.y / PSInput.PositionSub.w + 1.0f) / 2.0f;
	Depth = g_DepthMapTexture.Sample(g_DepthMapSampler, TexCoords);

	// 出力カラー = ディフューズカラー * テクスチャカラー + スペキュラカラー
	PSOutput.Color0 = TextureDiffuseColor;
	//*/

	PSOutput.Color0.a = 1.f - saturate((Depth.r - PSInput.VPosition.z) / MaxOpacityDistance);
	if (PSOutput.Color0.a == 1.f) {
		PSOutput.Color0.a = 0.f;
	}

	PSOutput.Color0.a *= PSInput.Diffuse.a;

	// 出力パラメータを返す
	return PSOutput ;
}
