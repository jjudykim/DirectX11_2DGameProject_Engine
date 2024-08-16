#pragma once
#include "CAsset.h"

#include "CSprite.h"

class CFlipBook :
    public CAsset
{
private:
    vector<Ptr<CSprite>>      m_vecSprite;
    float                     m_FPS;              // 현재 재생중인 FlipBook의 초당 프레임 수


public:
    void SetFPS(float _FPS) { m_FPS = _FPS; }
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    vector<Ptr<CSprite>>& GetVecSprite() { return m_vecSprite; }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }
    float GetFPS() { return m_FPS; }
    int GetMaxFrameCount() { return (int)m_vecSprite.size(); }

public:
    void FinalTick();

    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

public:
    CFlipBook();
    ~CFlipBook();
};

