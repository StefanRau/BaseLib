// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 21.12.2022: Extend destructor - Stefan Rau
// 26.01.2023: Add debug support - Stefan Rau
// 27.03.2023: Add filter support - Stefan Rau

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

void ListCollection::Delete(int iIndex)
{
	DebugMethodCalls("ListCollection::Delete");

	ListElement *lIterator;

	lIterator = GetInternal(iIndex);

	if (lIterator == nullptr)
	{
		// If the element does not exist
		return;
	}

	// Adapt the element before
	if (lIterator->_mPrevious != nullptr)
	{
		// If a predecessor exists

		if (lIterator->_mNext == nullptr)
		{
			// the current element is the last one => make entry before the last one
			lIterator->_mPrevious->_mNext = nullptr;
		}
		else
		{
			// If a successor exists => entry before points to the successor of the current entry
			lIterator->_mPrevious->_mNext = lIterator->_mNext;
		}
	}

	// Adapt the element after
	if (lIterator->_mNext != nullptr)
	{
		// If successor exists

		if (lIterator->_mPrevious == nullptr)
		{
			// the current element is the 1st one => make entry after the 1st one
			lIterator->_mNext->_mPrevious = nullptr;
		}
		else
		{
			// If a predecessor exists => entry after points to the predecessor of the current entry
			lIterator->_mNext->_mPrevious = lIterator->_mPrevious;
		}
	}

	// Rempove current element from memory
	if (lIterator->_mObject != nullptr)
	{
		delete lIterator->_mObject;
	}

	if (lIterator != nullptr)
	{
		delete lIterator;
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

	ListElement *lIterator = GetInternal(iIndex);
	return (lIterator == nullptr) ? nullptr : lIterator->_mObject;
}

ListElement *ListCollection::GetInternal(int iIndex)
{
	DebugMethodCalls("ListCollection::GetInternal");

	int lIterator = 0;

	for (ListElement *lCurrentElement = _mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->_mNext, lIterator++)
	{
		if (iIndex == lIterator)
		{
			// return element under this index
			return lCurrentElement;
		}
	}

	// Element not found, index is out of range
	return nullptr;
}

void *ListCollection::Filter(bool (*iCallback)(void* ))
{
	DebugMethodCalls("ListCollection::Filter");

	ListElement *lIterator = GetInternal(iCallback);
	return (lIterator == nullptr) ? nullptr : lIterator->_mObject;
}

ListElement *ListCollection::GetInternal(bool (*iCallback)(void* ))
{
	DebugMethodCalls("ListCollection::GetInternal");

	int lIterator = 0;

	for (ListElement *lCurrentElement = _mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->_mNext, lIterator++)
	{
		// Call customer comparer
		if (iCallback(lCurrentElement->_mObject))
		{
			// return element under this index
			return lCurrentElement;
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
