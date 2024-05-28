﻿#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM       m_SelectTexParam;

public:
    virtual void render_update() override;


public:
    void SelectTexture(DWORD_PTR _dwData);

    virtual void CreateAssetInstance(Ptr<CAsset> _Asset) override;

public:
    MaterialUI();
    ~MaterialUI();
};

