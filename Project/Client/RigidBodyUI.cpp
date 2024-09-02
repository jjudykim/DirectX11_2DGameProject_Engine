#include "pch.h"
#include "RigidBodyUI.h"

RigidBodyUI::RigidBodyUI()
	: ComponentUI(COMPONENT_TYPE::RIGIDBODY)
{
}

RigidBodyUI::~RigidBodyUI()
{
}

void RigidBodyUI::Update()
{
	Title();
}