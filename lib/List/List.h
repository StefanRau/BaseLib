// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Dynamic list class

#pragma once
#ifndef _List_h
#define _List_h

#include <Arduino.h>

/// <summary>
/// Class that contains a single list element. Base is a double chained list.
/// </summary>
class ListElement
{
public:
	ListElement *mPrevious = nullptr; // pointer to predecissor - the 1st element has nullptr
	ListElement *mNext = nullptr;	  // pointer to successor - the last element has nullptr
	void *mObject = nullptr;		  // pointer to the contained object
};

/// <summary>
/// Provides the functionality for list processing
/// </summary>
class ListCollection
{
public:
	// /// <summary>
	// /// Gets a new instance of this class.
	// /// </summary>
	// /// <returns>Instance of this class</returns>
	// static ListCollection *GetInstance();

	/// <summary>
	/// Constructor
	/// </summary>
	ListCollection();

	/// <summary>
	/// Destroyes all content.
	/// </summary>
	~ListCollection();

	/// <summary>
	/// Adds an object to the list
	/// </summary>
	/// <param name="iObject">Object to add</param>
	/// <returns>True if element is sucessfully added</returns>
	bool Add(void *iObject);

	/// <summary>
	/// Deletes an object from the list
	/// </summary>
	/// <param name="iIndex">Index of the object to delete</param>
	/// <returns>True if element is sucessfully deleted</returns>
	bool Delete(int iIndex);

	/// <summary>
	/// Gets the 1st object of the list
	/// </summary>
	/// <returns>Object to get</returns>
	void *GetFirst();

	/// <summary>
	/// Gets the last object of the list
	/// </summary>
	/// <returns>Object to get</returns>
	void *GetLast();

	/// <summary>
	/// Gets the object at the index
	/// </summary>
	/// <param name="iIndex">Index of the object to get</param>
	/// <returns>Object to get</returns>
	void *Get(int iIndex);

	/// <summary>
	/// Gets the object using a customer filter implementation
	/// </summary>
	/// <param name="iCallback">Method that calculates filter criteria</param>
	/// <returns>Object to get</returns>
	/// <remarks>
	/// Users need to implement a static function that receives an object of the list.
	/// The user's implementation checks the input of that function
	/// and returns true, if the input matches a user defined criteria.
	/// In the other case, the user function returns false.
	/// The 1st matching object is then returned by "Filter".
	/// </remarks>
	void *Filter(bool (*iCallback)(void *,void*));

	void SetHostingElement(void *iHostingElement);

	/// <summary>
	/// Calculates the size of the object list
	/// </summary>
	/// <returns>Size of list</returns>
	uint16_t Count();

	/// <summary>
	/// Starts a new iteration
	/// </summary>The 1st element of the list of list</returns>
	ListElement *IterateStart();

	/// <summary>
	/// Iterates the list and gets internally the next element
	/// </summary>
	/// <returns>Object at the current iteration step, is overwritten with the next element</returns>
	void *Iterate(ListElement **iCurrentElement);

private:
	void *mHostingElement;

	ListElement *mFirst = nullptr; // pointer to 1st element of the list
	ListElement *mLast = nullptr;  // pointer to last element of the list

	/// <summary>
	/// Gets the ListElement at the index
	/// </summary>
	/// <param name="iIndex">Index of the object to get</param>
	/// <param name="iCallback">Method that calculates filter criteria</param>
	/// <returns>ListElement to get</returns>
	ListElement *GetInternal(int iIndex);
	ListElement *GetInternal(bool (*iCallback)(void *,void*));
};

#endif