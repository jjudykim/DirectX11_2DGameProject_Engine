#include "pch.h"
#include "CBossPuzzleScript.h"

CBossPuzzleScript::CBossPuzzleScript()
	: CScript(UINT(SCRIPT_TYPE::BOSSPUZZLESCRIPT))
{
}

CBossPuzzleScript::~CBossPuzzleScript()
{
}

void CBossPuzzleScript::Begin()
{
}

void CBossPuzzleScript::Tick()
{
}

void CBossPuzzleScript::SaveToFile(FILE* _File)
{
}

void CBossPuzzleScript::LoadFromFile(FILE* _File)
{
}

void CBossPuzzleScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYERWEAPON)
	{
		GetOwner()->Collider2D()->AddOverlapCount();
	}
}

void CBossPuzzleScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossPuzzleScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


