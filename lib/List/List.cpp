// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 21.12.2022: Extend destructor - Stefan Rau
// 26.01.2023: Add debug support - Stefan Rau

#include "List.h"

ListCollection::ListCollection()
{
	DebugInstantiation("ListCollection");
}

ListCollection::~ListCollection()
{
	// destroy all list objects and their contained content objects
	DebugDestroy("ListCollection");

	ListElement *lIterator;
	bool lIterationRuns = true;

	IterateStart();

	do
	{
		// Delete each single element
		lIterator = (ListElement *)Iterate();
		if (lIterator != nullptr)
		{
			delete lIterator->_mObject;
			delete lIterator;
		}
		else
		{
			lIterationRuns = false;
		}
	} while (lIterationRuns);
}

ListCollection *ListCollection::GetInstance()
{
	DebugMethodCalls("ListCollection::GetInstance");
	return new ListCollection();
}

void ListCollection::Add(void *iObject)
{
	DebugMethodCalls("ListCollection::Add");

	ListElement *lNewElement;

	lNewElement = new ListElement();
	lNewElement->_mObject = iObject;

	if (_mFirst == nullptr)
	{
		_mFirst = lNewElement;
		_mLast = lNewElement;
	}
	else
	{
		// insert new element at the end of the list
		_mLast->_mNext = lNewElement;
		lNewElement->_mPrevious = _mLast;
		_mLast = lNewElement;
	}
}

void *ListCollection::GetFirst()
{
	DebugMethodCalls("ListCollection::GetFirst");
	return _mFirst->_mObject;
}

void *ListCollection::GetLast()
{
	DebugMethodCalls("ListCollection::GetLast");
	return _mLast->_mObject;
}

void *ListCollection::Get(int iIndex)
{
	DebugMethodCalls("ListCollection::Get");

	int lIterator = 0;

	for (ListElement *lCurrentElement = _mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->_mNext, lIterator++)
	{
		if (iIndex == lIterator)
		{
			// return element under this index
			return lCurrentElement->_mObject;
		}
	}

	// Element not found, index is out of range
	return nullptr;
}

int ListCollection::Count()
{
	DebugMethodCalls("ListCollection::Count");

	int lIterator = 0;

	for (ListElement *lCurrentElement = _mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->_mNext, lIterator++)
		;

	return lIterator;
}

void ListCollection::IterateStart()
{
	DebugMethodCalls("ListCollection::IterateStart");

	_mIterator = _mFirst;
}

void *ListCollection::Iterate()
{
	DebugMethodCalls("ListCollection::Iterate");

	void *lCurrentObject;

	if (_mIterator != nullptr)
	{
		lCurrentObject = _mIterator->_mObject;
		_mIterator = _mIterator->_mNext;
		return lCurrentObject;
	}
	return nullptr;
}
