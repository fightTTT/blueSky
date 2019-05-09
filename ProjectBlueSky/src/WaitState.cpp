#include "WaitState.h"
#include "AICharacter.h"

WaitState::WaitState()
{
}

WaitState::~WaitState()
{
}

void WaitState::Init(AICharacter * chara)
{
}

void WaitState::Update(AICharacter * character)
{
	character->SetAnim("‘Ò‹@");
}
