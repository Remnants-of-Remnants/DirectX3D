﻿#include "pch.h"
#include "CPhysXMgrScript.h"

#include <Engine\CPhysXMgr.h>


CPhysXMgrScript::CPhysXMgrScript()
	: CScript((UINT)SCRIPT_TYPE::PHYSXMGRSCRIPT)
{
}

CPhysXMgrScript::~CPhysXMgrScript()
{
}

//RayInput
static Vec3 RayOrigin = Vec3();
static Vec3 RayDir = Vec3();

//RayContactInfo
static tRoRHitInfo hitinfo= {};
static bool iscontact = false;
static string strobj = "";
static Vec3 contactpos = Vec3();


void CPhysXMgrScript::begin()
{
	AppendScriptParam("pPhysXMgr", SCRIPT_PARAM::MGR_PHYSX, (void*)CPhysXMgr::GetInst());

	AppendScriptParam("[Mgr]Debug Line&Sphere", SCRIPT_PARAM::BOOL, (void*)&(CPhysXMgr::GetInst()->m_RayDebug));

	AppendScriptParam("[Script]RayOrigin", SCRIPT_PARAM::VEC3, (void*)&RayOrigin);
	AppendScriptParam("[Script]RayDir", SCRIPT_PARAM::VEC3, (void*)&RayDir);

	AppendScriptParam("[Script]raycol_result", SCRIPT_PARAM::BOOL, (void*)&iscontact,0.f,0.f,true);
	AppendScriptParam("[Script]raycol_obj", SCRIPT_PARAM::STRING, (void*)&strobj,0.f,0.f,true);
	AppendScriptParam("[Script]raycol_pos", SCRIPT_PARAM::VEC3, (void*)&contactpos, 0.f, 0.f, true);
}


void CPhysXMgrScript::tick()
{
	iscontact = CPhysXMgr::GetInst()->PerfomRaycast(
		PxVec3(RayOrigin.x, RayOrigin.y, RayOrigin.z),
		PxVec3(RayDir.x, RayDir.y, RayDir.z),
		hitinfo);

	if (true == iscontact)
	{
		strobj = ToString(hitinfo.pOtherObj->GetName());
		contactpos = hitinfo.vHitPos;
	}
	else
	{
		strobj = "";
	}
}
