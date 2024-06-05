#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"

// =======================================
// LandScape Shader
// MRT      : Deferred
// RS_TYPE  : CULL_BACK
// DS_TYPE  : LESS
// BS_TYPE  : DEFAULT

// Parameter
// g_int_0  : Face X
// g_int_1  : Face Z
// g_tex_0  : HeightMap Texture


#define                     TileTexArr                      g_texarr_0  // Tile �迭 �ý���
#define                     TileTexArrSize                  g_float_0   // �迭 ����
#define                     WeightMapResolution             g_vec2_0    // ����ġ ���� �ػ�
#define                     CamWorldPos                     g_vec4_0.xyz // ī�޶� ���� ��ǥ

#define                     EdgePowCOunt                    g_vec4_1    // �� edge�� �����ϴ� Ƚ��(x,y,z) ,w = inside 
#define                     MatEdgeFactor                   g_mat_0     // �� tessfactor�� ���� ���
StructuredBuffer<float4> WEIGHT_MAP : register(t17); // ����ġ ����
// =======================================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    
    output.vTangent = _in.vTangent;
    output.vBinormal = _in.vBinormal;
    output.vNormal = _in.vNormal;
    
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
        
    float3 vUpDown = (_in[1].vWorldPos + _in[2].vWorldPos) / 2.f;
    float3 vLeftRight = (_in[0].vWorldPos + _in[2].vWorldPos) / 2.f;
    float3 vSlide = (_in[0].vWorldPos + _in[1].vWorldPos) / 2.f;
    float3 vMid = (_in[0].vWorldPos + _in[1].vWorldPos + _in[2].vWorldPos) / 3.f;

    float3 vCamWorldPos = CamWorldPos;
    vCamWorldPos.y = 0.f;
    
    output.arrEdge[0] = pow(EdgePowCOunt.x, (int) GetTessFactor(distance(vCamWorldPos, vUpDown), (int) MatEdgeFactor._11, (int) MatEdgeFactor._12, MatEdgeFactor._13, MatEdgeFactor._14));
    output.arrEdge[1] = pow(EdgePowCOunt.y, (int) GetTessFactor(distance(vCamWorldPos, vLeftRight), (int) MatEdgeFactor._21, (int) MatEdgeFactor._22, MatEdgeFactor._23, MatEdgeFactor._24));
    output.arrEdge[2] = pow(EdgePowCOunt.z, (int) GetTessFactor(distance(vCamWorldPos, vSlide), (int) MatEdgeFactor._31, (int) MatEdgeFactor._32, MatEdgeFactor._33, MatEdgeFactor._34));
    output.Inside = pow(EdgePowCOunt.w, (int) GetTessFactor(distance(vCamWorldPos, vMid), (int) MatEdgeFactor._41, (int) MatEdgeFactor._42, MatEdgeFactor._43, MatEdgeFactor._44));
    
    return output;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

[patchconstantfunc("PatchConstFunc")]
[outputtopology("triangle_cw")]
[domain("tri")]
[maxtessfactor(64)]
[partitioning("integer")] //[partitioning("fractional_odd")]
[outputcontrolpoints(3)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _in, uint _idx : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _in[_idx].vPos;
    output.vUV = _in[_idx].vUV;
    
    output.vTangent = _in[_idx].vTangent;
    output.vBinormal = _in[_idx].vBinormal;
    output.vNormal = _in[_idx].vNormal;
    
    return output;
}

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float2 vFullUV : TEXCOORD1;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

[domain("tri")]
DS_OUT DS_LandScape(PatchLevel _pathlevel // �� ������ �� ���� ����
             , const OutputPatch<HS_OUT, 3> _Origin // ��ġ ���� ����
             , float3 _Weight : SV_DomainLocation)   // �� ���� ������ ���� ����ġ)
{
    DS_OUT output = (DS_OUT) 0.f;
        
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    float3 vTangent = (float3) 0.f;
    float3 vBinormal = (float3) 0.f;
    float3 vNormal = (float3) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Origin[i].vPos * _Weight[i];
        vUV += _Origin[i].vUV * _Weight[i];
        
        vTangent += _Origin[i].vTangent * _Weight[i];
        vBinormal += _Origin[i].vBinormal * _Weight[i];
        vNormal += _Origin[i].vNormal * _Weight[i];
    }
    
    // ���� ��ü ���� UV
    output.vFullUV = vUV / float2(g_int_0, g_int_1);
    
    // ���̸� �ؽ��İ� ���� ��
    if (g_btex_0)
    {
        float2 FullUV = output.vFullUV;
        vLocalPos.y = g_tex_0.SampleLevel(g_sam_0, FullUV, 0).x;
        
        // �ֺ� ����(��, �Ʒ�, ��, ��) �� �����Ҷ��� ���ý����̽��󿡼��� ����
        float LocalStep = 1.f / _pathlevel.Inside;
        
        // �ֺ� ����(��, �Ʒ�, ��, ��) �� ���̸� ���̸ʿ��� �����ö� �߽�UV ���� �ֺ�UV �� �����Ҷ��� UV ��ȭ��
        float2 vUVStep = LocalStep / float2(g_int_0, g_int_1);
        
        // ��
        float3 vUp = float3(vLocalPos.x
                            , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x, FullUV.y - vUVStep.y), 0).x
                            , vLocalPos.z + LocalStep);
        
        // �Ʒ�
        float3 vDown = float3(vLocalPos.x
                             , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x, FullUV.y + vUVStep.y), 0).x
                             , vLocalPos.z - LocalStep);
        
        // ��
        float3 vLeft = float3(vLocalPos.x - LocalStep
                             , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x - vUVStep.x, FullUV.y), 0).x
                             , vLocalPos.z);
        
        // ��
        float3 vRight = float3(vLocalPos.x + LocalStep
                            , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x + vUVStep.x, FullUV.y), 0).x
                            , vLocalPos.z);
        
        
        vTangent = mul(float4(vRight, 1.f), g_matWorld).xyz - mul(float4(vLeft, 1.f), g_matWorld).xyz;
        vBinormal = mul(float4(vDown, 1.f), g_matWorld).xyz - mul(float4(vUp, 1.f), g_matWorld).xyz;
        vNormal = normalize(cross(vTangent, vBinormal));
        

    }

    output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matWV).xyz);
    output.vViewTangent = normalize(mul(float4(vTangent, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(vBinormal, 0.f), g_matWV).xyz);
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vEmissive : SV_Target3;
};

PS_OUT PS_LandScape(DS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
        
    float3 vViewNormal = _in.vViewNormal;
    
    // Ÿ�� �迭 �ؽ��İ� ������
    output.vColor = float4(0.8f, 0.8f, 0.8f, 1.f);
    
    if (g_btexarr_0)
    {
        float2 derivX = ddx(_in.vUV) * 0.5f; // �����ȼ��� x�� ��̺а��� ���Ѵ�
        float2 derivY = ddy(_in.vUV) * 0.5f; // �����ȼ��� y�� ��̺а��� ���Ѵ�
        
         // Ÿ�� ����
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int) WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;

        int iMaxWeightIdx = -1;
        float fMaxWeight = 0.f;

        for (int i = 0; i < TileTexArrSize; ++i)
        {
            // �迭 �ؽ��� ���ø��Ҷ� UV 3��°���� �迭 �ε���
            vColor += TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            //vColor += TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, i), 0) * vWeight[i];

            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
        
        output.vColor = float4(vColor.rgb, 1.f);
        
         // Ÿ�� �븻
        if (-1 != iMaxWeightIdx)
        {
            float3 vTangentSpaceNormal = TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileTexArrSize), derivX, derivY).xyz;
            //float3 vTangentSpaceNormal = TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), 0).xyz;
            vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;

            float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
            vViewNormal = normalize(mul(vTangentSpaceNormal, matTBN));
        }
    }
    
    output.vNormal = float4(vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vEmissive;
    
    return output;
}

#endif