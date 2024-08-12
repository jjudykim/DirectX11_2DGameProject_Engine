#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CAssetMgr.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Tick()
{
	for (int Row = 0; Row < MAX_LAYER; ++Row)
	{
		for (int Col = Row; Col < MAX_LAYER; ++Col)
		{
			if (m_Matrix[Row] & (1 << Col))
			{
				CollisionBtwLayer(Row, Col);
			}
		}
	}
}

void CCollisionMgr::CollisionCheck(UINT Layer1, UINT Layer2)
{
	UINT Row = Layer1;
	UINT Col = Layer2;

	if (Row > Col)
	{
		Row = (UINT)Layer2;
		Col = (UINT)Layer1;
	}

	if (m_Matrix[Row] & (1 << Col))
	{
		m_Matrix[Row] &= ~(1 << Col);
	}
	else
	{
		m_Matrix[Row] |= (1 << Col);
	}
}

void CCollisionMgr::CollisionBtwLayer(UINT _Left, UINT _Right)
{
	// 현재 Level
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pCurLevel == nullptr)
		return;

	// 각 Layer에 속한 모든 오브젝트들을 가져옴
	const vector<CGameObject*>& vecLeft = pCurLevel->GetLayer(_Left)->GetObjects();
	const vector<CGameObject*>& vecRight = pCurLevel->GetLayer(_Right)->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		CCollider2D* pLeftCol = vecLeft[i]->Collider2D();
		if (pLeftCol == nullptr)
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			CCollider2D* pRightCol = vecRight[j]->Collider2D();
			if (pRightCol == nullptr)
				continue;

			COLLIDER_ID id = {};

			id.LeftID = pLeftCol->GetID();
			id.RightID = pRightCol->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);

			// 등록된 적이 없으면 등록시키기
			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			 bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();
			// bool bDeactive = !vecLeft[i]->IsActive() || !vecRight[j]->IsActive();

			// 두 충돌체가 현재 충돌 중
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 이전에도 충돌 중
				if (iter->second)
				{
					pLeftCol->Overlap(pRightCol);
					pRightCol->Overlap(pLeftCol);
				}
				// 이전에는 충돌 중이 X
				else
				{
					pLeftCol->BeginOverlap(pRightCol);
					pRightCol->BeginOverlap(pLeftCol);
				}

				iter->second = true;

				// 두 충돌체 중 하나라도 Dead 상태이거나, 비활성화 상태라면
				// 추가로 충돌 해제를 호출시키기
				if (bDead) // || bDeactive)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
					iter->second = false;
				}
			}

			// 두 충돌체가 현재 충돌중이 아닌 경우
			else
			{
				// 이전에는 충돌 중
				if (iter->second)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
				}

				iter->second = false;
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _Left, CCollider2D* _Right)
{
	// 충돌체의 기본 원형 도형을 가져온다
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	Vtx* pVtx = (Vtx*)pRectMesh->GetVtxSysMem();

	// 각 충돌체의 월드 행렬을 가져오기
	const Matrix& matLeft = _Left->GetWorldMat();
	const Matrix& matRight = _Right->GetWorldMat();

	// OBB 검사 (분리축 검사)
	Vec3 vProjAxis[4] = {};

	vProjAxis[0] = XMVector3TransformCoord(pVtx[3].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);
	vProjAxis[1] = XMVector3TransformCoord(pVtx[1].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);

	vProjAxis[2] = XMVector3TransformCoord(pVtx[3].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);
	vProjAxis[3] = XMVector3TransformCoord(pVtx[1].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);

	// 충돌체의 중심을 잇는 벡터
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight);

	// 투영
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = vProjAxis[i];
		vProj.Normalize();

		float dot = fabs(vProjAxis[0].Dot(vProj));
		dot += fabs(vProjAxis[1].Dot(vProj));
		dot += fabs(vProjAxis[2].Dot(vProj));
		dot += fabs(vProjAxis[3].Dot(vProj));
		dot /= 2.f;

		float fCenter = fabs(vCenter.Dot(vProj));

		if (dot < fCenter)
			return false;
	}
	
	return true;
}