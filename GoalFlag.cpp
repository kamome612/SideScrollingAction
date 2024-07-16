#include "GoalFlag.h"

GoalFlag::GoalFlag(GameObject* parent)
	:GameObject(parent,"GoalFlag"),gfImage_(-1)
{
}

GoalFlag::~GoalFlag()
{
	if (gfImage_ > 0) {
		DeleteGraph(gfImage_);
	}
}

void GoalFlag::Initialize()
{
}

void GoalFlag::Update()
{
}

void GoalFlag::Draw()
{
}
