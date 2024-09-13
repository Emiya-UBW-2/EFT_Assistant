set NowDir=%CD%
cd %~dp0

ShaderCompiler.exe /Tvs_4_0 VS_SoftShadow.hlsl
ShaderCompiler.exe /Tps_4_0 PS_SoftShadow.hlsl

ShaderCompiler.exe /Tvs_4_0 VS_PBR3D.hlsl
ShaderCompiler.exe /Tgs_4_0 GS_PBR3D.hlsl
ShaderCompiler.exe /Tps_4_0 PS_PBR3D.hlsl

pause

cd %NowDir%