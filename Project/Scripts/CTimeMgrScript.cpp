#include "pch.h"
#include "CTimeMgrScript.h"

#include <Engine/CTimeMgr.h>



#define TIMEMGR CTimeMgr::GetInst()

CTimeMgrScript::CTimeMgrScript()
	: CScript(TIMEMGRSCRIPT)
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
	TIMEMGR->LockDeltaTime(m_DTLock);
	TIMEMGR->SetDTScale(m_DTScale);
}

void CTimeMgrScript::SaveToFile(FILE* _File)
{
}

void CTimeMgrScript::LoadFromFile(FILE* _File)
{
}

#undef TIMEMGR