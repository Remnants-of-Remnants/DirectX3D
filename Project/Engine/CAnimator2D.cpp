#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
{

}

CAnimator2D::CAnimator2D(const CAnimator2D& _OriginAnimator)
	: CComponent(_OriginAnimator)
	, m_CurAnim(nullptr)
	, m_bRepeat(_OriginAnimator.m_bRepeat)
{
	map<wstring, CAnim*>::const_iterator iter = _OriginAnimator.m_mapAnim.begin();
	for (; iter != _OriginAnimator.m_mapAnim.end(); ++iter)
	{
		CAnim* pCloneAnim = iter->second->Clone();

		pCloneAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
	}

	if (nullptr != _OriginAnimator.m_CurAnim)
	{
		m_CurAnim = FindAnim(_OriginAnimator.m_CurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Delete_Map(m_mapAnim);	
}

void CAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_CurAnim->IsFinish() && m_bRepeat)
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void CAnimator2D::UpdateData()
{
	if (nullptr == m_CurAnim)
		return;

	m_CurAnim->UpdateData();
}

void CAnimator2D::Clear()
{	
	CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop
	, Vec2 _vSliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);

	pAnim = new CAnim;	
	pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetSize, _Background, _FrmCount, _FPS);
	auto name = std::filesystem::path(_strKey).filename();
	pAnim->SetName(name);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strKey)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
	CAnim* pAnim = FindAnim(_strAnimName);
	if (nullptr == pAnim)
		return;

	m_bRepeat = _bRepeat;

	m_CurAnim = pAnim;
	m_CurAnim->Reset();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
	// �ִϸ��̼� ���� ����
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToFile(_File);
	}
	
	// �÷��� ���̴� �ִϸ��̼��� Ű�� �����Ѵ�.
	wstring PlayAnimName;

	if (nullptr != m_CurAnim)
	{
		PlayAnimName = m_CurAnim->GetName();
	}

	SaveWString(PlayAnimName, _File);
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

void CAnimator2D::SaveToFile(ofstream& fout)
{
	// �ִϸ��̼� ���� ����
	size_t AnimCount = m_mapAnim.size();
	fout << "[AnimCount]" << endl;
	fout << AnimCount << endl;
	if (AnimCount == 0) return;

	fout << "[AnimNames]" << endl;
	for (const auto& pair : m_mapAnim)
	{
		fout << ToString(pair.first) << endl;
	}

	// �÷��� ���̴� �ִϸ��̼��� Ű�� �����Ѵ�.
	wstring PlayAnimName;
	fout << "[PlayingAnimName]" << endl;

	if (nullptr != m_CurAnim)
	{
		PlayAnimName = m_CurAnim->GetName();
	}

	fout << ToString(PlayAnimName) << endl;

	fout << "[IsRepeat]" << endl;
	fout << m_bRepeat << endl;
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	// �ִϸ��̼� ���� �ε�
	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);
		
	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim* pAnim = new CAnim;
		pAnim->LoadFromFile(_File);

		pAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));		
	}

	// �÷��� ���̴� �ִϸ��̼��� Ű�� �ҷ��´�
	wstring PlayAnimName;
	LoadWString(PlayAnimName, _File);

	if (!PlayAnimName.empty())
	{
		m_CurAnim = FindAnim(PlayAnimName);
	}

	fread(&m_bRepeat, sizeof(bool), 1, _File);
}

void CAnimator2D::LoadFromFile(ifstream& fin)
{
	string tag, str;

	size_t animcount;
	getline(fin, tag); // [AnimCount]
	fin >> animcount;
	getline(fin, str); // ���� ó��

	if (animcount == 0) return;

	getline(fin, tag); // [AnimNames]

	wstring path = CPathMgr::GetContentPath();
	path += L"anim\\";

	vector<string> files;
	Utils::LoadAllFilePaths(path, files);

	for (size_t i = 0; i < animcount; i++)
	{
		string animName;
		getline(fin, animName);

		bool isFind = false;
		for (int i = 0; i < files.size(); i++) 
		{
			if (files[i].find(animName) != string::npos) 
			{
				isFind = true;

				ifstream finAnim(files[i]);

				CAnim* pAnim = new CAnim;
				pAnim->LoadFromFile(finAnim);

				pAnim->m_Animator = this;
				m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
				break;
			}
		}

		if (!isFind) 
		{
			MessageBox(nullptr, L"���� �ִϸ��̼��Դϴ�.", L"�ִϸ��̼� �ε� ����", 0);
		}
	}

	getline(fin, tag); // [PlayingAnimName]
	getline(fin, str);

	bool repeat;
	getline(fin, tag); // [IsRepeat]
	fin >> repeat;
	Play(ToWString(str), repeat);
	getline(fin, str); // ���� ó��
}

void CAnimator2D::SaveAllAnim(const wstring& path)
{
	for (auto iter : m_mapAnim) {
		string animPath = ToString(path + iter.second->GetName());
		animPath += ".anim";
		ofstream fout(animPath);
		iter.second->SaveToFile(fout);
	}
}
