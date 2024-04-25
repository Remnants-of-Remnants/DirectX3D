#include "pch.h"
#include "PathMgr.h"

#include "function.h"
vector<wstring> g_vecName;

int main()
{
	CPathMgr::init();

	// 스크립트 매니저 생성
	ScriptNameInput();
	MakeScriptMgrHeader();
	MakeScriptMgrCPP();

	GetAllContents();

	MakeStrHeader("Scripts\\strLevel.h", "LEVEL", g_vecLevelNames);
	MakeStrHeader("Scripts\\strFx.h", "FX", g_vecFxNames);
	MakeStrHeader("Scripts\\strSnd.h", "SND", g_vecSndNames);
	MakeStrHeader("Scripts\\strTex.h", "TEX", g_vecTexNames);
	MakeStrHeader("Scripts\\strMtrl.h", "MTRL", g_vecMtrlNames);
	MakeStrHeader("Scripts\\strPref.h", "PREF", g_vecPrefNames);
	MakeStrHeader("Scripts\\strAnim.h", "ANIM", g_vecAnimNames);

	return 0;
}