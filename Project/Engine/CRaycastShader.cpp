﻿#include "pch.h"
#include "CRaycastShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CTexture.h"

CRaycastShader::CRaycastShader()
	: CComputeShader(32, 32, 1)
	, m_iXFace(0)
	, m_iZFace(0)
	, m_ray{}
	, m_pOutput(nullptr)
{
	Create(L"shader\\raycast.fx", "CS_Raycast");
}

CRaycastShader::~CRaycastShader()
{
}

int CRaycastShader::UpdateData()
{
	if (nullptr == m_pOutput)
		return E_FAIL;

	m_Const.iArr[0] = m_iXFace;
	m_Const.iArr[1] = m_iZFace;

	m_Const.v4Arr[0] = m_ray.vStart;
	m_Const.v4Arr[1] = m_ray.vDir;

	// 높이맵 (SRV)
	m_pHeightMap->UpdateData_CS_SRV(0);

	// 출력 버퍼	(UAV)
	m_pOutput->UpdateData_CS_UAV(0);

	return S_OK;
}

void CRaycastShader::UpdateGroupCount()
{
	// 그룹 수 계산
	UINT GroupX = ((m_iXFace * 2) / m_ThreadX) + 1;
	UINT GroupY = (m_iZFace / m_ThreadY) + 1;
	UINT GroupZ = 1;

	SetGroup(GroupX, GroupY, GroupZ);
}

void CRaycastShader::Clear()
{
	m_pHeightMap->Clear_CS_SRV();
	m_pOutput->Clear_CS_UAV();
}