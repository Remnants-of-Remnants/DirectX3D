﻿#include "pch.h"
#include "CLandScape.h"

#include "CDevice.h"
#include "CAssetMgr.h"

void CLandScape::Init()
{
	CreateMesh();

	// 지형 전용 재질 적용
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));

	// 지형 전용 컴퓨트 쉐이더 생성
	CreateComputeShader();

	// 지형 전용 텍스쳐 생성
	CreateTexture();

	// 레이캐스팅 결과 받는 버퍼
	m_CrossBuffer = CDevice::GetInst()->GetCrossBuffer();
}

void CLandScape::CreateMesh()
{
	// 지형 전용 메쉬 생성 및 적용
	vector<Vtx> vecVtx;
	Vtx v;

	// 정점 배치
	for (UINT i = 0; i < m_FaceZ + 1; ++i)
	{
		for (UINT j = 0; j < m_FaceX + 1; ++j)
		{
			v.vPos = Vec3(j, 0.f, i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);

			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vUV = Vec2(j, m_FaceZ - i);

			vecVtx.push_back(v);
		}
	}

	// 인덱스 지정
	vector<UINT> vecIdx;

	for (UINT i = 0; i < m_FaceZ; ++i)
	{
		for (UINT j = 0; j < m_FaceX; ++j)
		{
			// 0 
			// | \
			// 2--1
			vecIdx.push_back((m_FaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_FaceX + 1) * i + (j + 1));
			vecIdx.push_back((m_FaceX + 1) * i + j);

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((m_FaceX + 1) * i + (j + 1));
			vecIdx.push_back((m_FaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_FaceX + 1) * (i + 1) + j + 1);
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	SetMesh(pMesh);

	// 지형 전용 재질 적용
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
}

void CLandScape::CreateComputeShader()
{
	// ======================
	// 높이 수정 컴퓨트 쉐이더
	// ======================
	m_CSHeightMap = (CHeightMapShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HeightMapShader").Get();
	if (nullptr == m_HeightMapTex)
	{
		m_CSHeightMap = new CHeightMapShader;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"HeightMapShader", m_CSHeightMap.Get());
	}

	// =====================
	// 지형 피킹 컴퓨트 쉐이더
	// =====================
	m_CSRaycast = (CRaycastShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"RaycastShader").Get();
	if (nullptr == m_CSRaycast)
	{
		m_CSRaycast = new CRaycastShader;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"RaycastShader", m_CSRaycast.Get());
	}
}

void CLandScape::CreateTexture()
{
	// 높이맵 텍스쳐		
	m_HeightMapTex = CAssetMgr::GetInst()->CreateTexture(L"HeightMapTex"
														, 2048, 2048
														, DXGI_FORMAT_R32_FLOAT
														, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
														, D3D11_USAGE_DEFAULT);

	m_BrushTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\brush\\Brush_02.png");
}
