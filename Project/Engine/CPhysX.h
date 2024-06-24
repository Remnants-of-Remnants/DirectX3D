﻿#pragma once
#include "CComponent.h"

class CPhysX :
    public CComponent
{
private:
    PxRigidActor* m_Actor = nullptr;
    int m_CollisionCount = 0;

    void updateFromPhysics();
    void updateToPhysics();

public:
    bool m_bStaticActor = false;
    virtual void begin() override;
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) {};
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) {};

    void setTransform(const PxTransform& transform);
    PxTransform getTransform() const;

    PxRigidActor* getActor() const {
        return m_Actor;
    }

    void BeginOverlap(CGameObject* other);
    void Overlap(CGameObject* other);
    void EndOverlap(CGameObject* other);

public:
    CLONE(CPhysX);
    CPhysX();
    //CPhysX(PxRigidActor* actor) : mActor(actor) {}
    ~CPhysX();

    friend class CPhysXMgr;
    friend class CScript;
};
