#pragma once
#include <list>
#include <memory>

class Obj;

using uniqueObj		=	std::unique_ptr<Obj>;
using uniqueObjList =	std::list<uniqueObj>;
using sharedListObj =	std::shared_ptr<uniqueObjList>;
using weakListObj	=	std::weak_ptr<uniqueObjList>;
using ListObj_itr	=	uniqueObjList::iterator;

struct AddObjList	// AddObjList()(�ǉ�����ؽĂ̼������߲��,�ǉ������޼ު���ư��߲��);
{
	ListObj_itr operator()(weakListObj objList, uniqueObj objPtr)
	{
		objList.expired();
		// weakPtr������Ԃɱ�������Ƃ���lock()���K�v
		objList.lock()->push_back(std::move(objPtr));
		ListObj_itr itr = objList.lock()->end();
		itr--;
		return itr;
	}
};