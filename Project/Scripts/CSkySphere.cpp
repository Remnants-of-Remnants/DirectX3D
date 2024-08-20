#include "pch.h"
#include "CSkySphere.h"

#include "Engine\CLevelMgr.h"
#include "Engine\CLevel.h"

#include "Engine\CSkyBox.h"
#include "Engine\CLight3D.h"
void CSkySphere::SetMember()
{
	auto pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;

	if (nullptr == m_Skybox_Day)
	{
		auto pObjDS = pLevel->FindObjectByName(L"SkyBox_Day");
		if (nullptr != pObjDS)
		{
			m_Skybox_Day = pObjDS->SkyBox();
			AppendScriptParam("Day::Alpha", SCRIPT_PARAM::FLOAT, &(m_Skybox_Day->m_fAlpha), 0.f, 1.f, true);
		}
	}
	if (nullptr == m_Skybox_Night)
	{
		auto pObjNS = pLevel->FindObjectByName(L"SkyBox_Night");
		if (nullptr != pObjNS)
		{
			m_Skybox_Night = pObjNS->SkyBox();
			AppendScriptParam("Night::Alpha", SCRIPT_PARAM::FLOAT, &(m_Skybox_Night->m_fAlpha), 0.f, 1.f, true);
		}
	}
	if (nullptr == m_DirectionalLight)
	{
		auto pObjDL = pLevel->FindObjectByName(L"Directional Light");
		if (nullptr != pObjDL)
		{
			m_DirectionalLight = pObjDL->Light3D();
		}
	}
}

void CSkySphere::begin()
{
}

static float RatioDown = .14f;
static float RatioUp   = .60f;
static float DTMul	   = 5.f;

#include "Engine\CRenderMgr.h"
void CSkySphere::tick()
{
	if ((nullptr == m_Skybox_Day || nullptr == m_Skybox_Night || nullptr == m_DirectionalLight))
	{
		SetMember();
	}
	if ((nullptr == m_Skybox_Day || nullptr == m_Skybox_Night || nullptr == m_DirectionalLight))
	{
		return;
	}

	m_Skybox_Day->m_fAlpha;

	// m_CurTime에 따라 m_Alpha 계산
	m_Skybox_Day->m_fAlpha	 = 1.0f - abs(2.0f * m_CurTime - 1.0f);
	m_Skybox_Night->m_fAlpha = 2 * abs(m_CurTime - .5f);

	// DayRot Set
	float fRot	  = -XM_PI + (XM_PI * 2 * m_CurTime); // -π에서 π까지 선형 보간
	auto  vDayRot = m_Skybox_Day->Transform()->GetRelativeRotation();
	vDayRot.x	  = fRot;
	m_Skybox_Day->Transform()->SetRelativeRotation(vDayRot);

	// NightRot Set
	fRot		   = -(XM_PI * 2 * m_CurTime); // π에서 -π까지 선형 보간
	auto vNightRot = m_Skybox_Night->Transform()->GetRelativeRotation();
	vNightRot.x	   = fRot;
	m_Skybox_Night->Transform()->SetRelativeRotation(vNightRot);

	// DirLightRot Set
	fRot		= XM_PI / 8.f - (XM_PI * 4 * m_CurTime);
	auto vDLRot = m_DirectionalLight->Transform()->GetRelativeRotation();
	vDLRot.y	= fRot;
	m_DirectionalLight->Transform()->SetRelativeRotation(vDLRot);

	// AmbientLight Set
	float fAmbient = NightAmbient + (DayAmbient - NightAmbient) * (1 - abs(m_CurTime - 0.5f) * 2);
	fAmbient /= 255.f;
	m_DirectionalLight->SetAmbient(Vec3(fAmbient, fAmbient, fAmbient));

	// MergeRatio 수정
	// auto& Refatio = CRenderMgr::GetInst()->m_GlobalBloomInfo.Ratio;
	// m_MergeRatioIncline ? Refatio = RoRMath::Lerp(Refatio, RatioUp, DT * DTMul)
	//					: Refatio = RoRMath::Lerp(Refatio, RatioDown, DT * DTMul);
	// if (Refatio > RatioUp)
	//	m_MergeRatioIncline = false;
	// if (Refatio < RatioDown)
	//	m_MergeRatioIncline = true;
}

CSkySphere::CSkySphere()
	: CScript((UINT)SCRIPT_TYPE::SKYSPHERE)
{
	AppendScriptParam("CurTime", SCRIPT_PARAM::FLOAT, &(m_CurTime), 0.f, 1.f, false, "", true);
	AppendScriptParam("DTMul", SCRIPT_PARAM::FLOAT, &(DTMul), 0.f, 1.f, false, "", true);
	AppendScriptParam("RatioDown", SCRIPT_PARAM::FLOAT, &(RatioDown), 0.f, 1.f, false, "", true);
	AppendScriptParam("RatioUp", SCRIPT_PARAM::FLOAT, &(RatioUp), 0.f, 1.f, false, "", true);
}

CSkySphere::~CSkySphere()
{
}
