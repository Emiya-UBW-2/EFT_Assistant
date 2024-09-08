// defines
#define PI 3.14159265359
#define EPSILON 1e-6

#define DX_LIGHTTYPE_POINT							(1)				// D_D3DLIGHT_POINT
#define DX_LIGHTTYPE_SPOT							(2)				// D_D3DLIGHT_SPOT
#define DX_LIGHTTYPE_DIRECTIONAL					(3)				// D_D3DLIGHT_DIRECTIONAL

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float3 VPosition : TEXCOORD1; // 座標( ビュー空間 )
    float3 VTan : TEXCOORD2; // 接線( ビュー空間 )
    float3 VBin : TEXCOORD3; // 従法線( ビュー空間 )
    float3 VNormal : TEXCOORD4; // 法線( ビュー空間 )
    float4 Specular : COLOR1; // スペキュラカラー
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// 色
	float4 Normal          : SV_TARGET1;	// 法線( ビュー空間 )
	float4 Depth           : SV_TARGET2;	// 深度
} ;


// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// ディフューズカラー
	float4		Specular ;				// スペキュラカラー
	float4		Ambient_Emissive ;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power ;					// スペキュラの強さ
	float		TypeParam0 ;			// マテリアルタイプパラメータ0
	float		TypeParam1 ;			// マテリアルタイプパラメータ1
	float		TypeParam2 ;			// マテリアルタイプパラメータ2
} ;

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv ;				// フォグ用パラメータ -1  / ( end - start )
	float		Density ;				// フォグ用パラメータ density
	float		E ;						// フォグ用パラメータ 自然対数の低

	float4		Color ;					// カラー
} ;

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		Padding1 ;				// パディング１

	float3		Position ;				// 座標( ビュー空間 )
	float		RangePow2 ;				// 有効距離の２乗

	float3		Direction ;				// 方向( ビュー空間 )
	float		FallOff ;				// スポットライト用FallOff

	float3		Diffuse ;				// ディフューズカラー
	float		SpotParam0 ;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		Specular ;				// スペキュラカラー
	float		SpotParam1 ;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		Attenuation0 ;			// 距離による減衰処理用パラメータ０
	float		Attenuation1 ;			// 距離による減衰処理用パラメータ１
	float		Attenuation2 ;			// 距離による減衰処理用パラメータ２
	float		Padding2 ;				// パディング２
} ;

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material ;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog ;					// フォグパラメータ
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

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common ;
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;


// C++ 側で設定するテクスチャや定数の定義
SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// ディフューズマップテクスチャ

SamplerState g_NormalMapSampler             : register(s1);		// 法線マップテクスチャ
Texture2D    g_NormalMapTexture             : register(t1);		// 法線マップテクスチャ

SamplerState g_SpecularMapSampler           : register(s2);		// スペキュラマップテクスチャ
Texture2D    g_SpecularMapTexture           : register(t2);		// スペキュラマップテクスチャ


struct IncidentLight
{
    float3 color;
    float3 direction;
    bool visible;
};

struct ReflectedLight
{
    float3 directDiffuse;
    float3 directSpecular;
};

struct GeometricContext
{
    float3 position;
    float3 normal;
    float3 viewDir;
};

struct Material
{
    float3 diffuseColor;
    float specularRoughness;
    float3 specularColor;
};

// lights

float punctualLightIntensityToIrradianceFactorDX(const in float lightDistance, const in float Attenuation0, const in float Attenuation1, const in float Attenuation2)
{
   	// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
    return 1.0f / (Attenuation0 + Attenuation1 * lightDistance + Attenuation2 * (lightDistance * lightDistance));
}

float punctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent)
{
    if (decayExponent > 0.0)
    {
        return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
    }
  
    return 1.0;
}

void getDirectionalDirectLightIrradiance(const in DX_D3D11_CONST_LIGHT Light, const in GeometricContext geometry, out IncidentLight directLight)
{
	directLight.color = Light.Diffuse;// +Light.Ambient.xyz;
    directLight.direction = -Light.Direction;
    directLight.visible = true;
}

void getPointDirectLightIrradiance(const in DX_D3D11_CONST_LIGHT Light, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = Light.Position - geometry.position;
    directLight.direction = normalize(L);
  
    float lightDistance = length(L);
    if (((lightDistance * lightDistance) < Light.RangePow2))
    {
		directLight.color = Light.Diffuse;// + Light.Ambient.xyz;
        directLight.color *= punctualLightIntensityToIrradianceFactorDX(lightDistance, Light.Attenuation0, Light.Attenuation1, Light.Attenuation2);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0.0, 0.0, 0.0);
        directLight.visible = false;
    }
}

void getSpotDirectLightIrradiance(const in DX_D3D11_CONST_LIGHT Light, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = Light.Position - geometry.position;
    directLight.direction = normalize(L);

    // ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
    float lLightDirectionCosA = dot(directLight.direction, Light.Direction * -1.f);
	// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
    float spotEffect = saturate(pow(abs(max(lLightDirectionCosA - Light.SpotParam0, 0.0f) * Light.SpotParam1), Light.FallOff));
    
    float lightDistance = length(L);
    if (((lightDistance * lightDistance) < Light.RangePow2) && spotEffect > 0.f)
    {
		directLight.color = Light.Diffuse;// +Light.Ambient.xyz;
        directLight.color *= punctualLightIntensityToIrradianceFactorDX(lightDistance, Light.Attenuation0, Light.Attenuation1, Light.Attenuation2);
        directLight.color *= spotEffect;
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0.0, 0.0, 0.0);
        directLight.visible = false;
    }
}

// BRDFs

// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material)
{
  
    float3 N = geometry.normal;
    float3 V = geometry.viewDir;
    float3 L = directLight.direction;
  
    float dotNL = saturate(dot(N, L));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V);
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = material.specularRoughness * material.specularRoughness;

    float D = D_GGX(a, dotNH);
    float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
    float3 F = F_Schlick(material.specularColor, V, H);
    return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight)
{
	float p = dot(geometry.normal, directLight.direction);
	p = p * 0.5f + 0.5;
	p = p * p;
	float dotNL = saturate(p);
	
	float3 irradiance = dotNL * directLight.color;
  
  // punctual light
	irradiance *= PI;

    reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
    reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material);
}

float4x4 InvTangentMatrix(
	float3 tangent,
	float3 binormal,
	float3 normal) {
	float4x4 mat = {float4(tangent , 0.0f),
					 float4(binormal, 0.0f),
					 float4(normal  , 0.0f),
					{0,0,0,1}
	};
	return mat;   // 転置
}

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

int      g_nMinSamples=1;             // 高さフィールドプロファイルをサンプリングするための最小サンプル数
int      g_nMaxSamples=1;             // 高さフィールドプロファイルをサンプリングするためのサンプルの最大数

// Parallax occlusion mapping pixel shader
float2 GetParallaxOcclusionMapping(float2 TexCoords, float3 Normal, float3 V_to_Eye) {
	float2 vParallaxOffsetTS = V_to_Eye.xy*(0.015);

	const int nNumSteps = 150;
	float fStepSize = 1.0 / (float)nNumSteps;
	float fPrevHeight = 1.0;

	

	float  fCurrentBound = 1.0;

	float2 dx, dy;
	dx = ddx(TexCoords);
	dy = ddy(TexCoords);

	bool IsEnd = false;
	[unroll]
	for (int nStepIndex = 0;nStepIndex < nNumSteps;nStepIndex++) {
		if (!IsEnd) {
			fCurrentBound -= fStepSize;
			// サンプルの高さマップ。この場合、法線マップのアルファ チャネルに保存されます。
			float fCurrHeight = g_NormalMapTexture.SampleGrad(g_NormalMapSampler, TexCoords - fCurrentBound * vParallaxOffsetTS, dx, dy).a;
			//float fCurrHeight = g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords - fCurrentBound * vParallaxOffsetTS).a;
			if (fCurrHeight < 0.1f) { fCurrHeight = 0.f; }

			//潜ったので
			if (fCurrHeight > fCurrentBound) {
				/*
				float fDenominator = fStepSize + fCurrHeight - fPrevHeight;
				if (fDenominator == 0.0f) {
					fCurrentBound = 0.f;
				}
				else {
					fCurrentBound = 1.f - (fCurrentBound * (fCurrHeight - fPrevHeight) + fStepSize * fCurrHeight) / fDenominator;
				}
				//*/
				IsEnd = true;
			}
			fPrevHeight = fCurrHeight;
		}
	}

	// 擬似押し出しサーフェス上の変位点の計算されたテクスチャ オフセット :
	return TexCoords - fCurrentBound * vParallaxOffsetTS;
}

PS_OUTPUT main(PS_INPUT PSInput)
{
    float3x4 mat =
    {
        normalize(PSInput.VTan),
		normalize(PSInput.VBin),
		normalize(PSInput.VNormal),
        { 0, 0, 0 }
    };
    float3 V_to_Eye;
	// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
    V_to_Eye.x = dot(normalize(PSInput.VTan), -PSInput.VPosition.xyz);
    V_to_Eye.y = dot(normalize(PSInput.VBin), -PSInput.VPosition.xyz);
    V_to_Eye.z = dot(normalize(PSInput.VNormal), -PSInput.VPosition.xyz);
    V_to_Eye = normalize(V_to_Eye);

	float3 Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0).rgb - 0.5f) * 2.0f;
	//接空間行列でローカル法線に変換
    Normal = mul(Normal, mat);

	//視差遮蔽マッピング
#if false
	float2 TexCoords = GetParallaxOcclusionMapping(PSInput.TexCoords0, Normal, V_to_Eye);
#else
	//視差マッピング
	float P = g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoords0).a;
	if (P < 0.1f) { P = 0.f; }
    float2 TexCoords = PSInput.TexCoords0 + 0.015 * P * V_to_Eye.xy;
	Normal = (g_NormalMapTexture.Sample(g_NormalMapSampler, TexCoords).rgb - 0.5f) * 2.0f;
	//接空間行列でローカル法線に変換
    Normal = mul(Normal, mat);
#endif

	float metallic = 0.f;
    float roughness = 0.f; //仮
    float4 albedo = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, TexCoords);

    PS_OUTPUT PSOutput;

    GeometricContext geometry;
    geometry.position = -PSInput.VPosition;
    geometry.normal = Normal;
    geometry.viewDir = normalize(PSInput.VPosition);
  
    Material material;
    material.diffuseColor = lerp(albedo.rgb, float3(0.0, 0.0, 0.0), metallic) * g_Common.Material.Diffuse.xyz;
    material.specularColor = lerp(float3(0.04, 0.04, 0.04), albedo.rgb, metallic) * g_Common.Material.Specular.xyz;
    material.specularRoughness = roughness;
  
    // Lighting
  
    ReflectedLight reflectedLight;
    reflectedLight.directDiffuse = float3(0.0, 0.0, 0.0);
    reflectedLight.directSpecular = float3(0.0, 0.0, 0.0);

    float3 emissive = g_Common.Material.Ambient_Emissive.xyz;
    float opacity = albedo.a;
  
    IncidentLight directLight;

    for (int i = 0; i < 6; ++i)
    {
        if (g_Common.Light[i].Type == DX_LIGHTTYPE_DIRECTIONAL)
        {
			getDirectionalDirectLightIrradiance(g_Common.Light[i], geometry, directLight);
            RE_Direct(directLight, geometry, material, reflectedLight);
        }
        else if (g_Common.Light[i].Type == DX_LIGHTTYPE_SPOT)
        {
            getSpotDirectLightIrradiance(g_Common.Light[i], geometry, directLight);
            if (directLight.visible)
            {
                RE_Direct(directLight, geometry, material, reflectedLight);
            }
        }
        else if (g_Common.Light[i].Type == DX_LIGHTTYPE_POINT)
        {
            getPointDirectLightIrradiance(g_Common.Light[i], geometry, directLight);
            if (directLight.visible)
            {
                RE_Direct(directLight, geometry, material, reflectedLight);
            }
        }
    }
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular;
	outgoingLight.r = saturate(outgoingLight.r);
	outgoingLight.g = saturate(outgoingLight.g);
	outgoingLight.b = saturate(outgoingLight.b);

    PSOutput.Color0 = float4(outgoingLight, opacity);













	PSOutput.Normal.x = (Normal.x + 1.0) / 2.0;
	PSOutput.Normal.y = (Normal.y + 1.0) / 2.0;
	PSOutput.Normal.z = (Normal.z + 1.0) / 2.0;
	PSOutput.Normal.w = 1.0;

	float4		TextureSpecularColor;
	TextureSpecularColor.xyzw = 0.0;

	TextureSpecularColor.xyz = g_SpecularMapTexture.Sample(g_SpecularMapSampler, TexCoords).xyz * PSInput.Specular.xyz;

	PSOutput.Depth.x = PSInput.VPosition.z;
	PSOutput.Depth.y = TextureSpecularColor.r * 0.299 + TextureSpecularColor.g * 0.587 + TextureSpecularColor.b * 0.114;
	PSOutput.Depth.z = 0.0;
	PSOutput.Depth.w = 1.0;

	// 出力パラメータを返す
    return PSOutput;
}

