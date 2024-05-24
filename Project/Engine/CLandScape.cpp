﻿#include "pch.h"
#include "CLandScape.h"

#include "CKeyMgr.h"
#include "CTransform.h"
#include "CRenderMgr.h"

#include "CCamera.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(64)
	, m_FaceZ(64)
	, m_BrushScale(Vec2(0.5f, 0.5f))
	, m_CrossBuffer(nullptr)
{
	Init();
}

CLandScape::~CLandScape()
{
}

void CLandScape::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		// 교점 위치정보를 토대로 높이를 수정 함
		m_CSHeightMap->SetInputBuffer(m_CrossBuffer);	// 픽킹 정보를 HeightMapShader 에 세팅

		m_CSHeightMap->SetBrushTex(m_BrushTex);			// 사용할 브러쉬 텍스쳐 세팅
		m_CSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
		m_CSHeightMap->SetBrushScale(m_BrushScale);		// 브러쉬 크기
		m_CSHeightMap->SetHeightMap(m_HeightMapTex);
		m_CSHeightMap->Execute();
	}
}

void CLandScape::render()
{
	UpdateData();

	GetMesh()->render();
}

void CLandScape::UpdateData()
{
	Transform()->UpdateData();

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceZ);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HeightMapTex);

	GetMaterial()->UpdateData();
}

void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_CrossBuffer->SetData(&out, 1);

	m_CSRaycast->SetHeightMap(m_HeightMapTex);
	m_CSRaycast->SetFaceCount(m_FaceX, m_FaceZ);
	m_CSRaycast->SetCameraRay(CamRay);
	m_CSRaycast->SetOuputBuffer(m_CrossBuffer);

	m_CSRaycast->Execute();

	m_CrossBuffer->GetData(&out);
}


#define TagFaceX "[FaceX]"
#define TagFaceZ "[FaceZ]"

void CLandScape::SaveToFile(ofstream& fout)
{
	fout << TagFaceX << endl;
	fout << m_FaceX << endl;

	fout << TagFaceZ << endl;
	fout << m_FaceZ << endl;
}

void CLandScape::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFaceX);
	fin >> m_FaceX;

	Utils::GetLineUntilString(fin, TagFaceZ);
	fin >> m_FaceZ;
}
