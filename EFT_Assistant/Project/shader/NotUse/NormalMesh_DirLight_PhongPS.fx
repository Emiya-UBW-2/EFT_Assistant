// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0;	// テクスチャ座標
	float3 VPosition       : TEXCOORD1;    // 座標( ビュー空間 )
	float3 VTan            : TEXCOORD2;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD3;    // 従法線( ビュー空間 )
	float3 VNormal         : TEXCOORD4;    // 法線( ビュー空間 )
	float  Fog			   : TEXCOORD5;		// フォグパラメータ
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0;	// 色
};

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
// 描画カラーに乗算する値
cbuffer cbMULTIPLYCOLOR_CBUFFER : register(b3)
{
	float4	g_MultiplyColor;
}

// C++ 側で設定するテクスチャや定数の定義
SamplerState g_DiffuseMapSampler            : register(s0);		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register(t0);		// ディフューズマップテクスチャ
SamplerState g_NormalMapSampler             : register(s1);		// 法線マップテクスチャ
Texture2D    g_NormalMapTexture             : register(t1);		// 法線マップテクスチャ

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;
	float3 SpecularColor;
	float3 Normal;
	float DiffuseAngleGen;
	float3 TotalDiffuse;
	float3 TotalSpecular;
	float3 TempF3;
	float Temp;
	float3 V_to_Eye;
	float3 lLightDir;
	float3 VNrm;
	float3 VTan;
	float3 VBin;

	float	g_time = g_MultiplyColor.x;
	// 接線・従法線・法線を正規化
	VNrm = normalize(PSInput.VNormal);
	VTan = normalize(PSInput.VTan);
	VBin = normalize(PSInput.VBin);

	// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
	TempF3.x = dot(VTan, -PSInput.VPosition.xyz);
	TempF3.y = dot(VBin, -PSInput.VPosition.xyz);
	TempF3.z = dot(VNrm, -PSInput.VPosition.xyz);
	V_to_Eye = normalize(TempF3);

	// 法線の 0～1 の値を -1.0～1.0 に変換する
	// 法線の準備
	//*
	{
		// テクスチャ座標
		float2 TexCoords0 = PSInput.TexCoords0;
		//ゆらゆら
		float WaveM;
		float2 WaveUV, WaveMove, DirU, DirV, WaveDir = { 1.0, 0.0 };

		WaveUV.x = dot(WaveDir, TexCoords0);
		WaveUV.y = dot(-WaveDir.yx, TexCoords0);
		WaveM = fmod((g_time + WaveUV.x * 5) * 6.28, 6.28);
		WaveMove.x = sin(WaveM) * 0.01 / 2;
		WaveM = fmod((g_time + WaveUV.y * 5) * 6.28, 6.28);
		WaveMove.y = sin(WaveM) * 0.01 / 2;

		TexCoords0 += WaveMove;
		//ノーマルマップ二重化計画
		TexCoords0.x += 0.5;
		TexCoords0.y += (sin(g_time * 3 + 8) / 512) + (g_time / 32);
		float h = g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).a;
		TexCoords0 = TexCoords0 + 0.016 * h * V_to_Eye.xy;
		Normal = 2.0f * g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).xyz - 1.0f;

		TexCoords0.x -= 0.5;
		TexCoords0.y -= ((sin(g_time * 3 + 8) / 512) + (g_time / 32))*2.0f;
		h = g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).a;
		TexCoords0 = TexCoords0 + 0.016 * h * V_to_Eye.xy;
		float3 Normal2 = 2.0f * g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords0).xyz - 1.0f;

		Normal = (Normal + Normal2) / 2;
	}
	//*/
	//Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0.xy).rgb - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// ディフューズカラーとスペキュラカラーの蓄積値を初期化
	TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	TotalSpecular = float3(0.0f, 0.0f, 0.0f);
	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ライト方向ベクトルのセット
	// ライト方向ベクトルの計算
	TempF3 = g_Common.Light[0].Direction;
	// ライトのベクトルを接地空間に変換
	lLightDir.x = dot(VTan, TempF3);
	lLightDir.y = dot(VBin, TempF3);
	lLightDir.z = dot(VNrm, TempF3);

	// ディフューズ色計算
	// DiffuseAngleGen = ディフューズ角度減衰率計算
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	// ディフューズカラー蓄積値 += ライトのディフューズカラー * マテリアルのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
	TotalDiffuse += g_Common.Light[0].Diffuse * g_Common.Material.Diffuse.xyz * DiffuseAngleGen + g_Common.Light[0].Ambient.xyz;

	// スペキュラカラー計算
	// ハーフベクトルの計算
	TempF3 = normalize(V_to_Eye - lLightDir);
	// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.Material.Power);
	// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
	TotalSpecular += Temp * g_Common.Light[0].Specular;

	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )
	

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
	TotalDiffuse += g_Common.Material.Ambient_Emissive.xyz;

	// SpecularColor = ライトのスペキュラカラー蓄積値 * マテリアルのスペキュラカラー
	SpecularColor = TotalSpecular * g_Common.Material.Specular.xyz;

	// 出力カラー = TotalDiffuse * テクスチャカラー + SpecularColor
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
	PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + SpecularColor;

	PSOutput.Color0.rgb = lerp(g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate(PSInput.Fog));//fog

	// アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	PSOutput.Color0.a = TextureDiffuseColor.a * g_Common.Material.Diffuse.a;// *g_Base.FactorColor.a;

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 出力パラメータを返す
	return PSOutput;
}