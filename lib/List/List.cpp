// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 21.12.2022: Extend destructor - Stefan Rau
// 26.01.2023: Add debug support - Stefan Rau
// 27.03.2023: Add filter support - Stefan Rau
// 02.06.2023: Add additional debug support and fix bug in destruction - Stefan Rau
// 10.06.2023: Add and delete return false when failing - Stefan Rau
// 09.07.2023: Parralel iterations are now possible as well

#include "List.h"

ListCollection::ListCollection()
{
	DEBUG_INSTANTIATION("ListCollection");
}

ListCollection::~ListCollection()
{
	// destroy all list objects and their contained content objects
	DEBUG_DESTROY("ListCollection");

	ListElement *lNextObject;
	ListElement *lCurrentObject = mFirst;

	// Delete each single element and the stored instances
	while (lCurrentObject != nullptr)
	{
		lNextObject = lCurrentObject->mNext;
		delete lCurrentObject->mObject;
		delete lCurrentObject;
		lCurrentObject = lNextObject;
	}
}

ListCollection *ListCollection::GetInstance()
{
	DEBUG_METHOD_CALL("ListCollection::GetInstance");
	return new ListCollection();
}

bool ListCollection::Add(void *iObject)
{
	DEBUG_METHOD_CALL("ListCollection::Add");

	ListElement *lNewElement;

	lNewElement = new ListElement();
	if (lNewElement == nullptr)
	{
		return false;
	}

	lNewElement->mObject = iObject;

	if (mFirst == nullptr)
	{
		mFirst = lNewElement;
		mLast = lNewElement;
	}
	else
	{
		// insert new element at the end of the list
		mLast->mNext = lNewElement;
		lNewElement->mPrevious = mLast;
		mLast = lNewElement;
	}

	DEBUG_PRINT_LN("Entry inserted into ListCollection");
	return true;
}

bool ListCollection::Delete(int iIndex)
{
	DEBUG_METHOD_CALL("ListCollection::Delete");

	ListElement *lIterator;

	lIterator = GetInternal(iIndex);

	if (lIterator == nullptr)
	{
		// If the element does not exist
		return false;
	}

	// Process one and only element
	if ((lIterator->mPrevious == nullptr) && (lIterator->mNext == nullptr))
	{
		mFirst = nullptr;
		mLast = nullptr;
	}

	// Adapt the element before
	if (lIterator->mPrevious != nullptr)
	{
		// If a predecessor exists

		if (lIterator->mNext == nullptr)
		{
			// the current element is the last one => make entry before the last one
			lIterator->mPrevious->mNext = nullptr;
			// Take the previous element as last one
			mLast = lIterator->mPrevious;
		}
		else
		{
			// If a successor exists => entry before points to the successor of the current entry
			lIterator->mPrevious->mNext = lIterator->mNext;
		}
	}

	// Adapt the element after
	if (lIterator->mNext != nullptr)
	{
		// If a successor exists

		if (lIterator->mPrevious == nullptr)
		{
			// the current element is the 1st one => make entry after the 1st one
			lIterator->mNext->mPrevious = nullptr;
			// Take the next element as 1st one
			mFirst = lIterator->mNext;
		}
		else
		{
			// If a predecessor exists => entry after points to the predecessor of the current entry
			lIterator->mNext->mPrevious = lIterator->mPrevious;
		}
	}

	// Rempove current element from memory
	if (lIterator->mObject != nullptr)
	{
		delete lIterator->mObject;
	}

	delete lIterator;
	DEBUG_PRINT_LN("Entry " + String(iIndex) + " deleted from ListCollection");
	return true;
}

void *ListCollection::GetFirst()
{
	DEBUG_METHOD_CALL("ListCollection::GetFirst");
	return mFirst->mObject;
}

void *ListCollection::GetLast()
{
	DEBUG_METHOD_CALL("ListCollection::GetLast");
	return mLast->mObject;
}

void *ListCollection::Get(int iIndex)
{
	DEBUG_METHOD_CALL("ListCollection::Get");

	ListElement *lIterator = GetInternal(iIndex);
	return (lIterator == nullptr) ? nullptr : lIterator->mObject;
}

ListElement *ListCollection::GetInternal(int iIndex)
{
	DEBUG_METHOD_CALL("ListCollection::GetInternal");

	int lIterator = 0;

	for (ListElement *lCurrentElement = mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->mNext, lIterator++)
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

void *ListCollection::Filter(bool (*iCallback)(void *))
{
	DEBUG_METHOD_CALL("ListCollection::Filter");

	ListElement *lIterator = GetInternal(iCallback);
	return (lIterator == nullptr) ? nullptr : lIterator->mObject;
}

ListElement *ListCollection::GetInternal(bool (*iCallback)(void *))
{
	DEBUG_METHOD_CALL("ListCollection::GetInternal");

	int lIterator = 0;

	for (ListElement *lCurrentElement = mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->mNext, lIterator++)
	{
		// Call customer comparer
		if (iCallback(lCurrentElement->mObject))
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
	DEBUG_METHOD_CALL("ListCollection::Count");

	int lIterator = 0;

	for (ListElement *lCurrentElement = mFirst; lCurrentElement != nullptr; lCurrentElement = lCurrentElement->mNext, lIterator++)
		;

	return lIterator;
}

ListElement *ListCollection::IterateStart()
{
	DEBUG_METHOD_CALL("ListCollection::IterateStart");

	return mFirst;
}

void *ListCollection::Iterate(ListElement **iCurrentElement)
{
	DEBUG_METHOD_CALL("ListCollection::Iterate");

	void *lCurrentObject;

	if (*iCurrentElement != nullptr)
	{
		lCurrentObject = (*iCurrentElement)->mObject;
		(*iCurrentElement) = (*iCurrentElement)->mNext;
		return lCurrentObject;
	}
	return nullptr;
}
