#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
{
}

CLight3D::~CLight3D()
{
}


void CLight3D::finaltick()
{
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_Info.vWorldPos = Transform()->GetWorldPos();

	CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_Info.LightType = (int)_type;
}


void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}