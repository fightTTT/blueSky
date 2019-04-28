#pragma once

#include <list>
#include <memory>

class Chicken;

using uniqueChicken = std::unique_ptr<Chicken>;
using uniqueChickenList = std::list<uniqueChicken>;
using sharedListChicken = std::shared_ptr<uniqueChickenList>;
using weakListChicken = std::weak_ptr<uniqueChickenList>;
using ListChicken_itr = uniqueChickenList::iterator;

struct AddChickenList	// AddObjList()(’Ç‰Á‚·‚éØ½Ä‚Ì¼ª±°ÄÞÎß²ÝÀ,’Ç‰Á‚·‚éµÌÞ¼Þª‚ÌÕÆ°¸Îß²ÝÀ);
{
	ListChicken_itr operator()(weakListChicken chickenList, uniqueChicken chickenPtr)
	{
		chickenList.expired();
		// weakPtr‚©‚çŽÀ‘Ô‚É±¸¾½‚·‚é‚Æ‚«‚Ílock()‚ª•K—v
		chickenList.lock()->push_back(std::move(chickenPtr));
		ListChicken_itr itr = chickenList.lock()->end();
		itr--;
		return itr;
	}
};