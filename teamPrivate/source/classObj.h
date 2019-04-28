#pragma once
#include <list>
#include <memory>

class Obj;

using uniqueObj		=	std::unique_ptr<Obj>;
using uniqueObjList =	std::list<uniqueObj>;
using sharedListObj =	std::shared_ptr<uniqueObjList>;
using weakListObj	=	std::weak_ptr<uniqueObjList>;
using ListObj_itr	=	uniqueObjList::iterator;

struct AddObjList	// AddObjList()(’Ç‰Á‚·‚éØ½Ä‚Ì¼ª±°ÄÞÎß²ÝÀ,’Ç‰Á‚·‚éµÌÞ¼Þª‚ÌÕÆ°¸Îß²ÝÀ);
{
	ListObj_itr operator()(weakListObj objList, uniqueObj objPtr)
	{
		objList.expired();
		// weakPtr‚©‚çŽÀ‘Ô‚É±¸¾½‚·‚é‚Æ‚«‚Ílock()‚ª•K—v
		objList.lock()->push_back(std::move(objPtr));
		ListObj_itr itr = objList.lock()->end();
		itr--;
		return itr;
	}
};