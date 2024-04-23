#ifndef _STD3D
#define _STD3D

#include "value.fx"


static float3 g_LightPos = float3(0.f, 0.f, 0.f);
static float3 g_LightDir = float3(1.f, 0.f, 0.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_LightAmbient = float3(0.1f, 0.1f, 0.1f);


struct VTX_IN
{
    float3 vPos : POSITION;
    float3 vUV  : TEXCOORD;    
    float3 vNormal : NORMAL;    
};


struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float LightPow : FOG;
};


VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
            
    // ���� �ȿ� ����ִ� Normal ������ ����� �̵���Ų��.
    // ���⺤�ʹ� ��������� 4��(�̵�����) �� ������� �ʾƾ� �ϱ� ������ 
    // ������ǥ�� 0 ���� �����ؼ� ����� �����Ų��.
    float3 vWorldNormal = mul(float4(_in.vNormal, 0.f), g_matWorld);    
    float3 vLightDir = normalize(g_LightDir);
    
    // ������ �����, ǥ�鿡���� ������ Normal ������ �������Ѽ�
    // ������ ���Ⱚ�� ���Ѵ�.
    output.LightPow = dot(vWorldNormal, -vLightDir);
    
    return output;
}


float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
         
    // ��ü ����
    float4 ObjectColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // ���� ���� == ��ü �� x ������ �� x ǥ���� ���� ���� 
    //           + ��ü �� x ȯ�汤 ����
    vOutColor.xyz = ObjectColor.xyz * g_LightColor * _in.LightPow
                  + ObjectColor.xyz * g_LightAmbient;
    
    return vOutColor;
}


// 3D �� �Ѿ���鼭 CULL_BACK ��带 ����Ѵ�.
// �ð�������� �ε��� ���ټ����� ������ ���� �����̶�� ����
// �ݴ� ���⿡�� ������ Culling �� �Ѵ�.







#endif