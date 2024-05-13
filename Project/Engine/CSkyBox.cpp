﻿#include "pch.h"
#include "CSkyBox.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"

#define TagType "[SkyBoxType]"
#define TagSphere "[SkyBoxSphereTex]"
#define TagCube "[SkyBoxCubeTex]"


CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_SkyBoxType(SKYBOX_TYPE::SPHERE)
{
	SetSkyBoxType(m_SkyBoxType);
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
	DrawShadow(false);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_SkyBoxType = _Type;

	if (SKYBOX_TYPE::SPHERE == m_SkyBoxType)
	{
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	}
	else if (SKYBOX_TYPE::CUBE == m_SkyBoxType)
	{
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	}
}


void CSkyBox::SaveToFile(ofstream& fout)
{
	fout << TagType << endl;

	string strSkyBoxType;
	strSkyBoxType = ToString(magic_enum::enum_name(m_SkyBoxType));

	fout << strSkyBoxType << endl;

	fout << TagSphere << endl;
	SaveAssetRef(m_SphereTex, fout);

	fout << TagCube << endl;
	SaveAssetRef(m_CubeTex, fout);

}

void CSkyBox::LoadFromFile(ifstream& fin)
{
	string tag;

	Utils::GetLineUntilString(fin, TagType);
	
	string strSkyBoxType;
	fin >> strSkyBoxType;

	auto SkyboxType = magic_enum::enum_cast<SKYBOX_TYPE>(strSkyBoxType);

	m_SkyBoxType = SkyboxType.value();

	Utils::GetLineUntilString(fin, TagSphere);
	LoadAssetRef(m_SphereTex, fin);

	Utils::GetLineUntilString(fin, TagCube);
	LoadAssetRef(m_CubeTex, fin);

}

void CSkyBox::finaltick()
{
}

void CSkyBox::UpdateData()
{
	Transform()->UpdateData();

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, (int)m_SkyBoxType);

	if (SKYBOX_TYPE::SPHERE == m_SkyBoxType)
	{
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_SphereTex);
	}
	else if (SKYBOX_TYPE::CUBE == m_SkyBoxType)
	{
		GetMaterial()->SetTexParam(TEX_PARAM::TEXCUBE_0, m_CubeTex);
	}

	GetMaterial()->UpdateData();
}

void CSkyBox::render()
{
	UpdateData();

	GetMesh()->render();
}
