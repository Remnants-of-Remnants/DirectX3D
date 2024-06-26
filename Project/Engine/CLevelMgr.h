﻿#pragma once
#include "singleton.h"

class CLevel;

enum class Client_Function_Type
{
	CIMGUIMGR_ENTER,
};

class CLevelMgr : public CManager<CLevelMgr>
{
	SINGLE(CLevelMgr);

private:
	CLevel* m_CurLevel;

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override;
	virtual void exit() override;

public:
	void ChangeLevelState(LEVEL_STATE _State);

private:
	void ChangeLevel_Task(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	void	RegisterClientFunction(Client_Function_Type _Type, std::function<void()> _MemberFunction);

private:
	unordered_map<Client_Function_Type, std::function<void()>> m_MapClientFunc;

	friend class CTaskMgr;
};
