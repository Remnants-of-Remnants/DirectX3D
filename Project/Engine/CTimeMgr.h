#pragma once

class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	// Larget Integer �� �׳� 8����Ʈ long long Ÿ�� ������ ���
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	double			m_DeltaTime;
	double			m_EngineDeltaTime;

	float			m_DTScale; // ��Ÿ Ÿ�� ����

	bool			m_bLock; // DT ���
	
	UINT			m_iCall;
	double			m_Time;

	wchar_t			m_szText[256];

public:
	float GetDeltaTime() const { return (float)m_DeltaTime; }
	double GetDeltaTime_d() const { return m_DeltaTime; }

	float GetEngineDeltaTime() const { return (float)m_EngineDeltaTime; }
	double GetEngineDeltaTime_d() const { return m_EngineDeltaTime; }

	float GetDTScale() const { return m_DTScale; }
	void SetDTScale(float _DTScale) { m_DTScale = _DTScale; }

	void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

public:
	void init();
	void tick();
	void render();
};

