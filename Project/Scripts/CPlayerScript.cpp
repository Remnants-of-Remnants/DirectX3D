﻿#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine\CLogMgr.h>
#include "CRoRStateMachine.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Speed(500.f)
{
	AppendScriptParam("Player Speed", SCRIPT_PARAM::FLOAT, &m_Speed);

	// 스테이트 초기화
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	m_FSM->SetCallbacks((UINT)PLAYER_STATE::NORMAL, ToString(magic_enum::enum_name(PLAYER_STATE::NORMAL)),
						&CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::ATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::ATTACK)),
						&CPlayerScript::AttackUpdate, &CPlayerScript::AttackBegin, &CPlayerScript::AttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVER, ToString(magic_enum::enum_name(PLAYER_STATE::COVER)),
						&CPlayerScript::CoverUpdate, &CPlayerScript::CoverBegin, &CPlayerScript::CoverEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::COVERATTACK)),
						&CPlayerScript::CoverAttackUpdate, &CPlayerScript::CoverAttackBegin,
						&CPlayerScript::CoverAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERKNEEATTACK,
						ToString(magic_enum::enum_name(PLAYER_STATE::COVERKNEEATTACK)),
						&CPlayerScript::CoverKneeAttackUpdate, &CPlayerScript::CoverKneeAttackBegin,
						&CPlayerScript::CoverKneeAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::DEATH, ToString(magic_enum::enum_name(PLAYER_STATE::DEATH)),
						&CPlayerScript::DeathUpdate, &CPlayerScript::DeathBegin, &CPlayerScript::DeathEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::CEREMONY, ToString(magic_enum::enum_name(PLAYER_STATE::CEREMONY)),
						&CPlayerScript::CeremonyUpdate, &CPlayerScript::CeremonyBegin, &CPlayerScript::CeremonyEnd,
						nullptr);
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

static string state = "";
void		  CPlayerScript::begin()
{
	m_FSM->Begin();
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void*)&state);
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial(0);
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
