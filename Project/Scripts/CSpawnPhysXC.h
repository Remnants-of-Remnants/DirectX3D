#pragma once
#include "Engine\CScript.h"
class CSpawnPhysXC : public CScript
{
private:
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSpawnPhysXC);
	CSpawnPhysXC();
	~CSpawnPhysXC();
};