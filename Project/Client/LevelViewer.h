#pragma once
#include "EditorUI.h"

class CLevel;

class LevelViewer :
    public EditorUI
{
private:
    CLevel*         m_CurLevel;

    wstring          m_LevelName;

    bool            m_LevelEditMode;

public:
    virtual void Update() override;

public:
    void Viewer();
    void ViewerButton();
    void FindLevelName();
    void RenewLevel();

public:
    LevelViewer();
    ~LevelViewer();
};

