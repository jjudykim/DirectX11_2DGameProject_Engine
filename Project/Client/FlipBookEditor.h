#pragma once
#include "EditorUI.h"

class FE_FBViewer;
class FE_FBDetail;
class FE_SpriteList;

class FlipBookEditor :
    public EditorUI
{
private:
    FE_FBViewer*         m_Viewer;
    FE_FBDetail*         m_Detail;
    FE_SpriteList*       m_SpriteList;

public:
    FE_FBViewer* GetFBViewer() { return m_Viewer; }
    FE_FBDetail* GetDetail() { return m_Detail; }
    FE_SpriteList* GetSpriteList() { return m_SpriteList; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    FlipBookEditor();
    ~FlipBookEditor();
};

