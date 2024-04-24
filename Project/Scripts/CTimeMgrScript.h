#pragma once
#include <Engine/CScript.h>

class CTimeMgrScript :
	public CScript
{
private:
	float	m_DTScale;
	int		m_DTLock; // ������� bool ������ ����ؾ� ������, ���� ��ũ��Ʈ �Ķ��� bool�� ���⿡ int�� �����.

public:
	void SetDTScale(float _DTScale) { m_DTScale = _DTScale; }
	void LockDeltaTime(bool _DTLock) { m_DTLock = _DTLock; }

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE_DISABLE(CTimeMgrScript);
	CTimeMgrScript();
	~CTimeMgrScript();
};

