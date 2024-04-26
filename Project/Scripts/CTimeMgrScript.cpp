#include "pch.h"
#include "CTimeMgrScript.h"

#include <Engine/CTimeMgr.h>



#define TIMEMGR CTimeMgr::GetInst()

CTimeMgrScript::CTimeMgrScript()
	: CScript((UINT)SCRIPT_TYPE::TIMEMGRSCRIPT)
	, m_DTLock(false)
	, m_DTScale(1.f)
{

}

CTimeMgrScript::~CTimeMgrScript()
{
}

void CTimeMgrScript::begin()
{
	AddScriptParam(SCRIPT_PARAM::INT, "[bool] DeltaTime Lock", &m_DTLock);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "[float] DeltaTime Scale", &m_DTScale);
}

void CTimeMgrScript::tick()
{
	// ���� ScriptParam�� �Լ��� ȣ���ϴ� ��������Ʈ ������ ��ư�� �ִٸ� �� ƽ���� �Լ��� ȣ������ �ʾƵ� �� �� ����.
	LockDeltaTime(m_DTLock);
	SetDTScale(m_DTScale);
}

void CTimeMgrScript::SetDTScale(float _DTScale)
{
	m_DTScale = _DTScale;
	TIMEMGR->SetDTScale(m_DTScale);
}

void CTimeMgrScript::LockDeltaTime(bool _DTLock)
{
	m_DTLock = _DTLock;
	TIMEMGR->LockDeltaTime(m_DTLock);
}

void CTimeMgrScript::SaveToFile(FILE* _File)
{
}

void CTimeMgrScript::LoadFromFile(FILE* _File)
{
}

#undef TIMEMGR