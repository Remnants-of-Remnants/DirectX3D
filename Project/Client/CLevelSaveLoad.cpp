#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAssetMgr.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>


void CLevelSaveLoad::SaveLevel(CLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);
	assert(LEVEL_STATE::STOP == _Level->GetState() || LEVEL_STATE::NONE == _Level->GetState());

	// Level �� ������ ���
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	ofstream fout(strLevelPath, ofstream::out | ofstream::trunc);
	if (!fout.is_open()) {
		MessageBox(nullptr, L"���� ���� ����!", L"���� ����", 0);
	}

	fout << "[LevelName]" << endl;
	fout << ToString(_Level->GetName()) << endl;

	// ������ ���̾� ����
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), fout);
	}

}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, FILE* _File)
{
	// Layer �� �̸� ����
	SaveWString(_Layer->GetName(), _File);

	// Layer �� �����ϰ� �ִ� GameObject ���� ����
	const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();

	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _File);
	}	
}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, ofstream& fout)
{
	// Layer �� �̸� ����
	fout << "[LayerName]" << endl;
	fout << ToString(_Layer->GetName()) << endl;

	// Layer �� �����ϰ� �ִ� GameObject ���� ����
	const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();

	size_t ObjCount = vecObject.size();
	fout << "[ObjCount]" << endl;
	fout << ObjCount << endl;

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], fout);
	}
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, FILE* _File)
{
	// GameObject �� �̸��� ����
	SaveWString(_Obj->GetName(), _File);
	
	// ������Ʈ ������ ����
	UINT i = 0;
	for ( ; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// ������Ʈ Ÿ�� ���� ����
		fwrite(&i, sizeof(UINT), 1, _File);

		// �ش� ������Ʈ�� ������ ������ ����
		pCom->SaveToFile(_File);
	}
	fwrite(&i, sizeof(UINT), 1, _File);

	// ��ũ��Ʈ ���� ����
	const vector<CScript*>& vecScripts = _Obj->GetScripts();
	size_t ScriptCount = vecScripts.size();

	// ��ũ��Ʈ ���� ����
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		SaveWString(CScriptMgr::GetScriptName(vecScripts[i]), _File);
		vecScripts[i]->SaveToFile(_File);
	}

	// �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	size_t childcount = vecChild.size();
	fwrite(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, ofstream& fout)
{
	// GameObject �� �̸��� ����
	fout << "[ObjectName]" << endl;
	fout << ToString(_Obj->GetName()) << endl;

	// ������Ʈ ������ ����
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// ������Ʈ Ÿ�� ���� ����
		fout << "[ComponentType]" << endl;
		auto type = magic_enum::enum_name((COMPONENT_TYPE)i);
		fout << ToString(type) << endl;

		// �ش� ������Ʈ�� ������ ������ ����
		pCom->SaveToFile(fout);
	}
	fout << "[Component_End]" << endl;

	// ��ũ��Ʈ ���� ����
	const vector<CScript*>& vecScripts = _Obj->GetScripts();
	size_t ScriptCount = vecScripts.size();

	// ��ũ��Ʈ ���� ����
	fout << "[ScriptCount]" << endl;
	fout << ScriptCount << endl;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		fout << "[ScriptName]" << endl;
		fout << ToString(wstring(CScriptMgr::GetScriptName(vecScripts[i]))) << endl;
		//vecScripts[i]->SaveToFile(_File);
	}

	// �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	size_t childcount = vecChild.size();

	string strChildCount;
	strChildCount = "[" + ToString(_Obj->GetName()) + "'s ChildCount]";
	fout << strChildCount << endl;
	fout << childcount << endl;

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], fout);
	}
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _strLevelPath)
{
	CLevel* pLevel = nullptr;

	// Level �� �ҷ��� ���
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	ifstream fin(strLevelPath);

	if (!fin.is_open())
	{
		MessageBox(nullptr, L"���� ��ΰ� �ùٸ��� �ʰų�, ������ ����", L"���� �ε� ����", MB_OK);
		return nullptr;
	}

	// ������ �̸��� �д´�.
	pLevel = new CLevel;
	string strLevelName;
	getline(fin, strLevelName); // [LevelName]
	getline(fin, strLevelName); 
	pLevel->SetName(strLevelName);

	// Layer �ε�
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		LoadLayer(pLevel->GetLayer(i), fin);
	}

	return pLevel;
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, FILE* _File)
{
	// Layer �� �̸� ����
	wstring strLayerName;
	LoadWString(strLayerName, _File);
	_Layer->SetName(strLayerName);


	// Layer �� �����ϴ� GameObject ���� �ҷ��´�.
	size_t ObjCount = 0;
	fread(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		CGameObject* pObject = LoadGameObject(_File);
		_Layer->AddObject(pObject, false);
	}
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, ifstream& fin)
{
	string tag;
	string str;
	getline(fin, tag); // [LayerName]
	getline(fin, str);
	_Layer->SetName(str);

	int objCnt;
	getline(fin, tag); // [ObjCount]
	getline(fin, str);
	objCnt = stoi(str);

	for (size_t i = 0; i < objCnt; i++)
	{
		CGameObject* pObject = LoadGameObject(fin);
		_Layer->AddObject(pObject, false);
	}
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;
	
	// GameObject �� �̸��� �ε�
	wstring strName;
	LoadWString(strName, _File);
	pObject->SetName(strName);

	// ������Ʈ ������ �ҷ�����
	COMPONENT_TYPE type = COMPONENT_TYPE::END;
	while(true)
	{
		fread(&type, sizeof(UINT), 1, _File);
		if (COMPONENT_TYPE::END == type)
			break;

		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;				
		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;		
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;		
		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case COMPONENT_TYPE::STATEMACHINE:
			pComponent = new CStateMachine;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;
		default:
			assert(nullptr);
			break;
		}

		// �ش� ������Ʈ�� ������ �����͸� �ε�
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// ��ũ��Ʈ ���� �б�
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWString(strScriptName, _File);		

		CScript* pScript = CScriptMgr::GetScript(strScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromFile(_File);
	}

	// �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����	
	size_t childcount = 0;
	fread(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		pObject->AddChild(LoadGameObject(_File));
	}

	return pObject;
}

CGameObject* CLevelSaveLoad::LoadGameObject(ifstream& fin)
{
	CGameObject* pObject = new CGameObject;

	string tag, str;
	getline(fin, tag); // [ObjectName]
	getline(fin, str);

	pObject->SetName(str);

	while (true) {
		getline(fin, tag); // [ComponentType] or [Component_End]
		if (tag == "[Component_End]") break;

		getline(fin, str);
		auto num = magic_enum::enum_cast<COMPONENT_TYPE>(str);
		if (num.has_value()) {
			auto type = num.value();
			CComponent* pComponent = nullptr;

			switch (type)
			{
			case COMPONENT_TYPE::TRANSFORM:
				pComponent = new CTransform;
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				pComponent = new CCollider2D;
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				MessageBox(nullptr, L"Collider3D", L"�̱����� ������Ʈ �ε�", 0);
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				pComponent = new CAnimator2D;
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				MessageBox(nullptr, L"Animator3D", L"�̱����� ������Ʈ �ε�", 0);
				break;
			case COMPONENT_TYPE::LIGHT2D:
				pComponent = new CLight2D;
				break;
			case COMPONENT_TYPE::LIGHT3D:
				pComponent = new CLight3D;
				break;
			case COMPONENT_TYPE::CAMERA:
				pComponent = new CCamera;
				break;
			case COMPONENT_TYPE::STATEMACHINE:
				pComponent = new CStateMachine;
				break;
			case COMPONENT_TYPE::MESHRENDER:
				pComponent = new CMeshRender;
				break;
			case COMPONENT_TYPE::TILEMAP:
				pComponent = new CTileMap;
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				pComponent = new CParticleSystem;
				break;
			default:
				assert(nullptr);
				break;
			}

			// �ش� ������Ʈ�� ������ �����͸� �ε�
			pObject->AddComponent(pComponent);
			pComponent->LoadFromFile(fin);
		}
		else {
			MessageBox(nullptr, L"������Ʈ Ÿ�� ����", L"���� ������Ʈ �ҷ�����", 0);
		}

	}

	size_t scriptCnt;
	getline(fin, tag); // [ScriptCount]
	getline(fin, str);
	scriptCnt = stoul(str);

	for (size_t i = 0; i < scriptCnt; i++)
	{
		getline(fin, tag); // [ScriptName]
		getline(fin, str);
		CScript* pScript = CScriptMgr::GetScript(ToWString(str));
		//pScript->LoadFromFile(fin);
		pObject->AddComponent(pScript);
	}

	size_t childCnt;
	getline(fin, tag); // [name's ChildCount]
	getline(fin, str);
	childCnt = stoul(str);

	for (size_t i = 0; i < childCnt; i++)
	{
		pObject->AddChild(LoadGameObject(fin));
	}

	return pObject;
}
