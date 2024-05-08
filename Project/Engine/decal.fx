#ifndef _DECAL
#define _DECAL

#include "value.fx"
#include "func.fx"

// ==========================
// Decal Shader
// Domain   : DOMAIN_DECAL
// MRT      : Decal
// RS_TYPE  : CULL_FRONT
// DS_TYPE  : NoTest NoWrite
// BS_TYPE  : AlphaBlend

// Parameter
// g_mat_0  : ViewInv * WorldInv
// g_tex_0  : Output Texture
// g_tex_1  : PositionTargetTex
// ===========================
struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_Decal(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target;
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // ȣ��� �ȼ��� ��ġ�� UV ������ ȯ��
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // PositionTarget ���� ���� ȣ��� �ȼ����̴��� ������ ������ �����ؼ� ��ǥ���� Ȯ��
    float4 vViewPos = g_tex_1.Sample(g_sam_0, vScreenUV);
    
    // Deferred �ܰ迡�� �׷����� ���ٸ� ���� �� �� ����.
    if (-1.f == vViewPos.w)
    {
        output.vColor = float4(1.f, 0.f, 0.f, 1.f);
        return output;
        //discard;
    }
    
    // Cube Volume �޽��� ���� �������� ��������.
    float3 vLocal = mul(float4(vViewPos.xyz, 1.f), g_mat_0).xyz;
    vLocal += 0.5f;
    if (vLocal.x < 0.f || 1.f < vLocal.x ||
        vLocal.y < 0.f || 1.f < vLocal.y ||
        vLocal.z < 0.f || 1.f < vLocal.z)
    {
        output.vColor = float4(0.f, 0.f, 1.f, 1.f);
        return output;
        //discard;
    }
        
    output.vColor = float4(0.f, 1.f, 0.f, 1.f);
    return output;
}





#endif