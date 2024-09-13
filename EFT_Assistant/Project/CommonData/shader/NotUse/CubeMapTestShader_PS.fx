// ピクセルシェーダーの入力
struct PS_INPUT
{
	float3 normalW         : TEXCOORD0 ;
	float3 viewVecW        : TEXCOORD1 ;
	float2 TexCoords0      : TEXCOORD2;    // テクスチャ座標
	float3 VPosition       : TEXCOORD3;    // 座標( ビュー空間 )
	float3 VNormal         : TEXCOORD4;    // 法線( ビュー空間 )
	float3 VTan            : TEXCOORD5;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD6;    // 従法線( ビュー空間 )
	float  Fog			   : TEXCOORD7;		// フォグパラメータ
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// 色
} ;


// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse;				// ディフューズカラー
	float4		Specular;				// スペキュラカラー
	float4		Ambient_Emissive;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power;					// スペキュラの強さ
	float		TypeParam0;			// マテリアルタイプパラメータ0
	float		TypeParam1;			// マテリアルタイプパラメータ1
	float		TypeParam2;			// マテリアルタイプパラメータ2
};

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv;				// フォグ用パラメータ -1  / ( end - start )
	float		Density;				// フォグ用パラメータ density
	float		E;						// フォグ用パラメータ 自然対数の低

	float4		Color;					// カラー
};

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
	int			Type;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		Padding1;				// パディング１

	float3		Position;				// 座標( ビュー空間 )
	float		RangePow2;				// 有効距離の２乗

	float3		Direction;				// 方向( ビュー空間 )
	float		FallOff;				// スポットライト用FallOff

	float3		Diffuse;				// ディフューズカラー
	float		SpotParam0;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		Specular;				// スペキュラカラー
	float		SpotParam1;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		Attenuation0;			// 距離による減衰処理用パラメータ０
	float		Attenuation1;			// 距離による減衰処理用パラメータ１
	float		Attenuation2;			// 距離による減衰処理用パラメータ２
	float		Padding2;				// パディング２
};

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[6];			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog;					// フォグパラメータ
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

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common;
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

SamplerState g_MetallicMapSampler           : register(s5);		// ディフューズマップテクスチャ
Texture2D    g_MetallicMapTexture           : register(t5);		// ディフューズマップテクスチャ

SamplerState g_NormalMapSampler             : register(s4);		// ディフューズマップテクスチャ
Texture2D    g_NormalMapTexture             : register(t4);		// ディフューズマップテクスチャ

// C++ 側で設定するテクスチャや定数の定義
SamplerState g_ToonDiffuseGradSampler       : register(s3);		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
Texture2D    g_ToonDiffuseGradTexture       : register(t3);		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ

SamplerState cubeMapSampler					: register( s2 ) ;
TextureCube  cubeMapTexture					: register( t2 ) ;

SamplerState dynamicCubeMapSampler			: register( s1 ) ;
TextureCube  dynamicCubeMapTexture			: register( t1 ) ;

SamplerState g_DiffuseMapSampler            : register(s0);		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register(t0);		// ディフューズマップテクスチャ



// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float3 vReflect ;
	float4 color ;

	float4 lTextureDiffuseColor;

	float4 lNormalDiffuseColor;
	float4 lMetallicDiffuseColor;


	float3 lSpecularColor;
	float3 lDiffuseColor;
	float3 Normal;
	float lDiffuseAngleGen;
	float3 lTotalDiffuse;
	float3 lTotalSpecular;
	float3 lTotalAmbient;
	float4 lToonColor;
	float lTotalLightGen;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float3 lLightDir;



	// 法線の 0～1 の値を -1.0～1.0 に変換する
	Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0.xy).rgb - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// 法線の準備
	//Normal = normalize(PSInput.VNormal);

	// 頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize(-PSInput.VPosition);

	// ディフューズカラーとスペキュラカラーとアンビエントカラーの合計値を初期化
	lTotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	lTotalSpecular = float3(0.0f, 0.0f, 0.0f);
	lTotalAmbient = float3(0.0f, 0.0f, 0.0f);

	// ライトの減衰率合計値の初期化
	lTotalLightGen = 0.0f;



	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ライト方向ベクトルのセット
	lLightDir = g_Common.Light[0].Direction;


	// ディフューズ角度減衰率計算
	lDiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	// ディフューズ減衰率を合計値に加算
	lTotalLightGen += lDiffuseAngleGen;


	// スペキュラカラー計算

	// ハーフベクトルの計算
	TempF3 = normalize(V_to_Eye - lLightDir);

	// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.Material.Power);

	// スペキュラライト合計値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
	lTotalSpecular += Temp * g_Common.Light[0].Specular;


	// ライトのディフューズカラーを合計値に加算
	lTotalDiffuse += g_Common.Light[0].Diffuse;

	// ライトのアンビエントカラーを合計値に加算
	lTotalAmbient += g_Common.Light[0].Ambient.xyz;

	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// アンビエントカラーの蓄積値 += マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
	lTotalAmbient += g_Common.Material.Ambient_Emissive.xyz;








	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャカラーの取得
	lTextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
	lNormalDiffuseColor = g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0);
	lMetallicDiffuseColor = g_MetallicMapTexture.Sample(g_MetallicMapSampler, PSInput.TexCoords0);

	// トゥーンテクスチャカラーをライトのディフューズ減衰率から取得
	lToonColor = g_ToonDiffuseGradTexture.Sample(g_ToonDiffuseGradSampler, lTotalLightGen);

	// ディフューズカラー = ライトのディフューズカラー蓄積値 * マテリアルのディフューズカラー
	lDiffuseColor = lTotalDiffuse * g_Common.Material.Diffuse.xyz;

	// スペキュラカラー = ライトのスペキュラカラー蓄積値 * マテリアルのスペキュラカラー
	lSpecularColor = lTotalSpecular * g_Common.Material.Specular.xyz * 10.f;

	// 出力 = saturate( saturate( ディフューズカラー * アンビエントカラーの蓄積値 ) * トゥーンテクスチャカラー + スペキュラカラー ) * テクスチャカラー
	PSOutput.Color0.rgb = saturate(saturate(lDiffuseColor + lTotalAmbient) * lToonColor.rgb + lSpecularColor) * lTextureDiffuseColor.rgb;

	// アルファ値 = ディフューズアルファ * マテリアルのディフューズアルファ * 不透明度
	PSOutput.Color0.a = lTextureDiffuseColor.a * g_Common.Material.Diffuse.a * g_Base.FactorColor.a;

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )






	vReflect = reflect( PSInput.viewVecW, PSInput.normalW ) ;
	color = 1.f * cubeMapTexture.Sample( cubeMapSampler, vReflect ) +
	        0.5f * dynamicCubeMapTexture.Sample( dynamicCubeMapSampler, vReflect ) ;

	color.a = 1.0f;
//	color = cubeMapTexture.Sample( cubeMapSampler, PSInput.normalW ) ;

	PSOutput.Color0 = (0.1f * lMetallicDiffuseColor.r * color) + 1.f * PSOutput.Color0;

	// フォグ効果
	PSOutput.Color0.rgb = lerp(g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate(PSInput.Fog));

	return PSOutput ;
}


