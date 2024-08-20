#pragma once
#include "Engine\CScript.h"

class CSkyBox;
class CLight3D;
class CSkySphere : public CScript
{
private:
	CSkyBox*	m_Skybox_Day		= nullptr;
	CSkyBox*	m_Skybox_Night		= nullptr;
	CLight3D*	m_DirectionalLight	= nullptr;
	float		m_CurTime			= .5f;
	float		m_TargetTime		= 0.f;
	bool		m_MergeRatioIncline = true;
	void		SetMember();
	const float DayAmbient	 = 150.f;
	const float NightAmbient = 80.f;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSkySphere);
	CSkySphere();
	~CSkySphere();
};
