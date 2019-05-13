#include "WaitState.h"
#include "AICharacter.h"

WaitState::WaitState()
{
}

WaitState::~WaitState()
{
}

void WaitState::Init(AICharacter * character)
{
	stateTime = 0;
}

void WaitState::Update(AICharacter * character)
{
	character->SetAnim("‘Ò‹@");
}
