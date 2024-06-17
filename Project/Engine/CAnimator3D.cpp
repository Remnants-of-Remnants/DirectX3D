﻿#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CAssetMgr.h"
#include "CAnimation3DShader.h"
#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_iStartFrame(0), m_iEndFrame(120)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iCurClip(_origin.m_iCurClip)
	, m_dCurTime(_origin.m_dCurTime)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_origin.m_iFrameIdx)
	, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
	, m_fRatio(_origin.m_fRatio)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_iStartFrame(40), m_iEndFrame(96)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}


void CAnimator3D::finaltick()
{
	m_dCurTime = 0.f;
	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_vecClipUpdateTime[m_iCurClip] += DTd_ENGINE;

	if (KEY_TAP(M))
	{
		m_iStartFrame = 0;
		m_iEndFrame = 120;
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}
	else if (KEY_TAP(N))
	{
		m_iStartFrame = 0;
		m_iEndFrame = 39;
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}
	else if (KEY_TAP(B))
	{
		m_iStartFrame = 40;
		m_iEndFrame = 96;
		m_vecClipUpdateTime[m_iCurClip] = (double)m_iStartFrame / (double)m_iFrameCount;
	}
	else if (KEY_TAP(V))
	{
		m_iStartFrame = 97;
		m_iEndFrame = 120;
		m_vecClipUpdateTime[m_iCurClip] = (double)m_iStartFrame / (double)m_iFrameCount;
	}

    double startTime = (double)m_iStartFrame / (double)m_iFrameCount;
	double endTime = (double)m_iEndFrame / (double)m_iFrameCount;
	double segmentTimeLength = endTime - startTime;

	if (m_vecClipUpdateTime[m_iCurClip] >= endTime)
	{
		m_vecClipUpdateTime[m_iCurClip] = startTime;
	}
	//if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	//{
	//	m_vecClipUpdateTime[m_iCurClip] = 0.f;
	//}
	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];
	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 시작 및 끝 프레임 범위 내에서 반복 재생
	if (m_iFrameIdx >= m_iEndFrame - 1)
	{
		if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iEndFrame - 1)
			m_iNextFrameIdx = m_iFrameIdx = m_iEndFrame - 2;
		else
			m_iNextFrameIdx = m_iFrameIdx = m_iEndFrame - 1;
	}
	//// 다음 프레임 인덱스
	//if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
	//	m_iNextFrameIdx = m_iFrameIdx = m_pVecClip->at(0).iFrameLength - 2; // 끝이면 현재 인덱스를 유지
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());

	// 테스트 코드
	/*static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;*/
}


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear(30);

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_READ_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveToFile(FILE* _pFile)
{
}

void CAnimator3D::SaveToFile(ofstream& fout)
{
}

void CAnimator3D::LoadFromFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromFile(ifstream& fin)
{
}
