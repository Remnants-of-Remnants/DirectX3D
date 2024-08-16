#include "pch.h"
#include "CSpawnPhysXC.h"

#include "Engine\CKeyMgr.h"
#include "CSpawnSpotScript.h"
void CSpawnPhysXC::begin()
{
}

void CSpawnPhysXC::tick()
{
	auto			  vecscript = GetOwner()->GetScripts();
	CSpawnSpotScript* SSS		= nullptr;

	for (auto& e : vecscript)
	{
		if (e->GetScriptType() == (UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
		{
			SSS = static_cast<CSpawnSpotScript*>(e);
		}
	}

	if (KEY_TAP(KEY::N) && nullptr != SSS)
	{
		SSS->RegisterObject();
		SSS->SpawnObject();
	}
}

CSpawnPhysXC::CSpawnPhysXC()
	: CScript((UINT)SCRIPT_TYPE::SPAWNPHYSXC)
{
}

CSpawnPhysXC::~CSpawnPhysXC()
{
}
