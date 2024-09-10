#pragma once
#include "singleton.h"
class CTimeMgr :
    public CSingleton<CTimeMgr>
{
    SINGLE(CTimeMgr)

private:
    // LARGE_INTEGER - 8바이트 정수 대응
    LARGE_INTEGER    m_llCurCount;
    LARGE_INTEGER    m_llPrevCount;
    LARGE_INTEGER    m_llFrequency;

    UINT             m_FPS;

    float            m_DeltaTime;
    float            m_Time;

    float            m_E_DeltaTime;
    float            m_E_Time;

    vector<tTimer>   m_VecTimers;

public:
    void Init();
    void Tick();
    void TimerUpdate();

public:
    float GetDeltaTime() { return m_DeltaTime; }
    float GetTime() { return m_Time; }

    float GetEngineDeltaTime() { return m_E_DeltaTime; }
    float GetEngineTime() { return m_E_Time; }

    UINT GetFPS() { return m_FPS; }

public:
    void AddTimer(float _Duration, std::function<void()> _Callback, bool _Repeat = false)
    {
        m_VecTimers.push_back({ _Duration, 0.0f, _Callback, _Repeat });
    }

};

