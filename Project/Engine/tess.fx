#ifndef _TESS
#define _TESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Tess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    return output;
}


// Hull Shader
struct PatchLevel
{
    float arrEdge[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

PatchLevel PatchConstFunc(InputPatch<VS_OUT, 3> _in, uint patchID : SV_PrimitiveID)
{
    PatchLevel output = (PatchLevel) 0.f;

    output.arrEdge[0] = g_vec4_0.x;
    output.arrEdge[1] = g_vec4_0.y;
    output.arrEdge[2] = g_vec4_0.z;
    output.Inside = g_vec4_0.w;
    
    return output;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

[patchconstantfunc("PatchConstFunc")]
[outputtopology("triangle_cw")]
[domain("tri")]
[maxtessfactor(64)]
//[partitioning("integer")]
[partitioning("fractional_odd")]
[outputcontrolpoints(3)]
HS_OUT HS_Tess(InputPatch<VS_OUT, 3> _in, uint _idx : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _in[_idx].vPos;
    output.vUV = _in[_idx].vUV;
    
    return output;
}

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


[domain("tri")]
DS_OUT DS_Tess(PatchLevel _pathlevel // �� ������ �� ���� ����
               , const OutputPatch<HS_OUT, 3> _Origin // ��ġ ���� ����
               , float3 _Weight : SV_DomainLocation) // �� ���� ������ ���� ����ġ
{
    DS_OUT output = (DS_OUT) 0.f;
    
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Origin[i].vPos * _Weight[i];
        vUV += _Origin[i].vUV * _Weight[i];
    
    }
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    return output;
}

float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    return vColor;
}



#endif