#pragma once
#include "CComponent.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_Info;


public:

    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightColor(Vec3 _vColor) { m_Info.vColor = _vColor; }
    void SetSpecular(Vec3 _vSpec) { m_Info.vSpecular = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_Info.vAmbient = _vAmb; }

    Vec4 GetLightColor(Vec3 _vColor) { return m_Info.vColor; }
    Vec4 GetSpecular(Vec3 _vSpec) { return m_Info.vSpecular; }
    Vec4 GetAmbient(Vec3 _vAmb) { return m_Info.vAmbient; }

    void SetLightType(LIGHT_TYPE _type);
    void SetRadius(float _Radius) { m_Info.fRadius = _Radius; }
    void SetAngle(float _Angle) { m_Info.fAngle = _Angle; }

    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_Info.LightType; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }

public:
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CLight3D);
public:
    CLight3D();
    ~CLight3D();
};

