#include "pch.h"
#include "CBossChainState.h"

CBossChainState::CBossChainState()
{
}

CBossChainState::~CBossChainState()
{
}

void CBossChainState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Chain.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Chain.flip");
}

void CBossChainState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(586.f, 640.f, 0.f));
}

void CBossChainState::FinalTick()
{
}

void CBossChainState::Exit()
{
}

