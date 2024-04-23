#ifndef _STD3D
#define _STD3D

#include "value.fx"


static float3 g_LightPos = float3(0.f, 0.f, 0.f);
static float3 g_LightDir = float3(1.f, -1.f, 1.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_LightAmbient = float3(0.1f, 0.1f, 0.1f);
static float3 g_SpecularRatio = float3(0.3f, 0.3f, 0.3f);


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
        
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};


VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
            
    // ���� �ȿ� ����ִ� Normal ������ ����� �̵���Ų��.
    // ���⺤�ʹ� ��������� 4��(�̵�����) �� ������� �ʾƾ� �ϱ� ������ 
    // ������ǥ�� 0 ���� �����ؼ� ����� �����Ų��.
    
    // ���� ������ PixelShader ����, ViewSpace �󿡼� ����
    // ���� ���̴������� ������ ViewSpace �󿡼��� Normal �� Position ���� PixelShader �� ����
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    
    
    return output;
}


float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
        
    // ��ü ����
    float4 ObjectColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    
    // ���� ������ ViewSpace ���� ����Ǳ�� �߱� ������,
    // ������ �����ϴ� ���⵵ View ���� �������� ������
    float3 vViewLightDir = normalize(mul(float4(g_LightDir, 0.f), g_matView).xyz);
   
    // ViewSpace ���� ������ �����, ��ü ǥ���� ������ �̿��ؼ� ������ ���� ����(Diffuse) �� ���Ѵ�.
    float LightPow = saturate(dot(_in.vViewNormal, -vViewLightDir));
            
    // ���� ǥ�鿡 �����ؼ� �ݻ�Ǵ� ������ ���Ѵ�.
    float3 vReflect = vViewLightDir + 2 * dot(-vViewLightDir, _in.vViewNormal) * _in.vViewNormal;
    vReflect = normalize(vReflect);
    
    // ī�޶� ��ü�� ���ϴ� ����
    float3 vEye = normalize(_in.vViewPos);
    
    // �ü����Ϳ� �ݻ纤�� ����, �ݻ籤�� ����
    float ReflectPow = saturate(dot(-vEye, vReflect));
    ReflectPow = pow(ReflectPow, 20.f);
    
    
    // ���� ���� == ��ü �� x ������ �� x ǥ���� ���� ���� 
    //           + ��ü �� x ȯ�汤 ����    
    //           + ���� �� x ���� �ݻ籤 ���Һ��� x �ݻ缼��(ī�޶�� �ݻ纤�Ͱ� ���� ���ֺ��� ����)
    vOutColor.xyz = ObjectColor.xyz * g_LightColor * LightPow
                    + ObjectColor.xyz * g_LightColor * g_LightAmbient
                    + g_LightColor * g_SpecularRatio * ReflectPow;
    
    return vOutColor;
}


// 3D �� �Ѿ���鼭 CULL_BACK ��带 ����Ѵ�.
// �ð�������� �ε��� ���ټ����� ������ ���� �����̶�� ����
// �ݴ� ���⿡�� ������ Culling �� �Ѵ�.







#endif