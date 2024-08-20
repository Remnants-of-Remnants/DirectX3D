#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#include "func.fx"

struct VS_SKYBOX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_SKYBOX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vUV_Dir : POSITION;
};

VS_SKYBOX_OUT VS_SkyBox(VS_SKYBOX_IN _in)
{
    VS_SKYBOX_OUT output = (VS_SKYBOX_OUT) 0.f;
        
    // 로컬 스페이스의 메쉬가 이미 카메라(View) 스페이스의 원점이 있는것으로 가정
    float3 vLocalPos = _in.vPos * 2.f;
    
    // 회전만 적용
    vLocalPos = mul(float4(vLocalPos, 1.f), g_matWorld).xyz;
    
    float3 vViewPos = mul(float4(vLocalPos, 0.f), g_matView);
    float4 vPosition = mul(float4(vViewPos, 1.f), g_matProj);
    vPosition.z = vPosition.w;
    
    // Skybox 가 Cube 타입인 경우
    if (1 == g_int_0)
    {
        output.vUV_Dir = _in.vPos;
        vPosition.z -= 0.01f; // 깊이 값에 작은 오프셋 추가
    }
    else
    {
        vPosition.z -= 0.02f; // 깊이 값에 작은 오프셋 추가
    }
    
    output.vPosition = vPosition;
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vEmissive : SV_Target3;
    float4 vRelativeLuminance : SV_Target4;
};

PS_OUT PS_SkyBox(VS_SKYBOX_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    //float4 vOutColor = float4(0.2f, 0.2f, 1.f, 1.f);
    
    if (0 == g_int_0)
    {
        if (g_btex_0)
        {
            output.vEmissive = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else if (1 == g_int_0)
    {
        if (g_btexcube_0)
        {
            float3 vUV = normalize(_in.vUV_Dir);
            output.vEmissive = g_texcube_0.Sample(g_sam_0, vUV);
        }
    }
    
    if (g_iBloomUse)
    {
        const static float3 vRLWeight = float3(0.2126f, 0.7152f, 0.0722f);
        float4 vBloomColor = g_BloomInfo[0].vBloomColor;
        float fThreshold = g_BloomInfo[0].fThreshold;
        //const static float4 vBloomColor = float4(1.f, 1.f, 1.f, 1.f);
        //float fThreshold = 0.8f;

        float brightness = dot(output.vEmissive.rgb, vRLWeight);
        if (brightness > fThreshold)
        {
            if (0 != g_BloomInfo[0].bUseOriginalColor)
            {
                output.vRelativeLuminance.rgb = output.vEmissive.rgb;
                output.vRelativeLuminance.a = 1.f;
            }
            else
            {
                output.vRelativeLuminance = vBloomColor;
            }
        }
        else
        {
            output.vRelativeLuminance = float4(0.f, 0.f, 0.f, 1.f);
        }
    }
    else
    {
        output.vRelativeLuminance = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    output.vEmissive.a = g_float_0;
    
    return output;
}

#endif