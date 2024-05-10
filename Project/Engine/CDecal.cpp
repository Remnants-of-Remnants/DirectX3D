﻿#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bAsEmissive(false)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"));
}

CDecal::~CDecal()
{
}

#define TagAsEmissive "[AsEmissive]"

void CDecal::SaveToFile(ofstream& fout)
{
	fout << TagAsEmissive << endl;
	fout << m_bAsEmissive << endl;
}

void CDecal::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagAsEmissive);
	fin >> m_bAsEmissive;
}

void CDecal::finaltick()
{
	GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 0.f), true);
}

void CDecal::UpdateData()
{
	Transform()->UpdateData();

	// ViewInv * WorldInv
	Matrix mat = g_Transform.matViewInv * Transform()->GetWorldInvMat();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::MAT_0, mat);

	// AsEmissive
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_bAsEmissive);

	GetMaterial()->UpdateData();
}

void CDecal::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	UpdateData();

	GetMesh()->render();
}