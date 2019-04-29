#pragma once
#include <memory>

class BaseScene;
class GameCtrl;
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	virtual unique_Base UpDate(unique_Base own, const GameCtrl &controller) = 0;		// ���ڰя���������̂��܂Ƃ߂��֐��Astate

private:
	virtual int Init(void) = 0;
};
