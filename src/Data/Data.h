// Copyright 2019 [BVU CMSC491 class]
#ifndef DATA_DATA_H_
#define DATA_DATA_H_

#include <sys/time.h>
#include <any>
#include <any>

/**
* The representation of data used by the stream processing engine.
*
* The @b Data class consists of two primary members, a `value` and a
* `timestamp`.  Application programmers must cast their data to and from a
* any in order to get their data in and out of the stream processing
* environment.  A `timestamp` is included to measure timings through the
* stream and to let the application programmers make temporally-based
* decisions.
*/
class Data {
public:
	/**
	* Creates Data from the user's `value` with the current `timestamp`.
	*
	* This constructor requires the application programmer to only provide the
	* `value` any. The `timestamp` will be generated on-the-fly from the
	* current time.
	*
	* @param value The any representation of the application programmer's
	* data.
	*/
	//explicit Data(std::any value);
	explicit Data(std::any value);

	/**
	* Creates Data from the users `value` and their provided `timestamp`.
	*
	* Creating Data with this constructor allows the application programmer to
	* specify their data via the `value` any and also provide their
	* `timestamp` which we apply to the object.
	*
	* @param value The any representation of the application programmer's
	* data.
	*
	* @param timestamp The unix timestamp that the application programmer wishes
	* to apply to the data as it enters the stream processing engine.
	*/
	//Data(std::any value, timeval timestamp);
	Data(std::any value, timeval timestamp);

	/// A any representation of the data
	std::any *value;

	/// The time the data entered the system
	timeval timestamp;
};

#endif  // DATA_DATA_H_