#pragma once
#include "EditorUI.h"

class Content :
    public EditorUI
{
private:
    class TreeUI*     m_Tree;
    vector<wstring>   m_vecAssetPath;     // Content 폴더에 있는 에셋 경로들

private:
    void RenewContent();
    void AssetClicked(DWORD_PTR _Param);
    void PopupMenu(DWORD_PTR _Param);

    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);

public:
    void Reload();              // Content Folder의 Resource들을 확인 및 로딩

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    Content();
    ~Content();
};

