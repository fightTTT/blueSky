#pragma once
#include<list>
#include<memory>

class Obj;

using sharedObj = std::shared_ptr<Obj>;
using sharedObjList = std::list <sharedObj>;
using sharedListObj = std::shared_ptr<sharedObjList>;
using weekListObj = std::weak_ptr<sharedObjList>;
using ListObj_itr = sharedObjList::iterator;

struct AddObjList		// objList�̖����ɗv�f��ǉ�����
{
	ListObj_itr operator()(weekListObj objList, sharedObj objPtr)
	{
		// objList.expired() true:�I�����Ă���, false:�����Ă���

		objList.lock()->push_back(std::move(objPtr));
		ListObj_itr itr = objList.lock()->end();
		itr--;
		return itr;
	}
};
