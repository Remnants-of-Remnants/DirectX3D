#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"



CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(true)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{	
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	
	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
		m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
	}

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bAbsolute)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();

			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_matWorld = m_matWorld * matParentScaleInv* matParentWorld;
		}
		else
		{
			m_matWorld *= matParentWorld;
		}		

		for (int i = 0; i < 3; ++i)
		{
			// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
			m_arrWorldDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
			m_arrWorldDir[i].Normalize();
		}
	}

	// 월드 뷰 행렬이 계산된 카메라 시점에서의 pos 값 세팅
	m_vWorldViewPos = TransformByWorldView(GetWorldPos());
}

void CTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;
		
	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->UpdateData();
}

Vec3 CTransform::GetWorldScale()
{
	if (m_bAbsolute)
	{
		return m_vRelativeScale;
	}

	CGameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = m_vRelativeScale;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRealtiveRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);	
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRealtiveRotation, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
}

Vec3 CTransform::TransformByWorldView(const Vec3& _vWorldpos)
{
	// 인자로 전달된 월드 위치 사용
	XMFLOAT3 worldPos = _vWorldpos;

	// 월드 뷰 행렬 가져오기
	Matrix matWorldView = GetWorldViewMatrix();

	// 타입 변환 (Vec -> Float)
	XMVECTOR worldPosVec = XMLoadFloat3(&worldPos);
	XMMATRIX worldViewMat = XMLoadFloat4x4(&matWorldView);

	XMVECTOR transformedPosVec = XMVector3TransformCoord(worldPosVec, worldViewMat);

	// 결과를 Vec3로 변환
	Vec3 transformedPos;
	XMStoreFloat3(&transformedPos, transformedPosVec);

	// 변환된 위치 반환
	return transformedPos;
}
