﻿#pragma once
#include "UI.h"

class MessageUI :
    public UI
{
private:

    vector<tLog>    m_vectLog;
    ImVec4          m_LogColor[(UINT)Log_Level::END];
    
    ImVec2          m_TableSize;
    ImVec2          m_MaxTableSize;

    int             m_iLoglvMask;
    string         m_charLogMsgFilter;


    int             m_iLogCount;
    bool            m_bResize;
    float           m_fLastScrollY;

public:
    ImVec4 SetLogColor(tLog Log);

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    MessageUI();
    ~MessageUI();
};

