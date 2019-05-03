/*
* Copyright 2008, 2009 Free Software Foundation, Inc.
*
* This software is distributed under the terms of the GNU Affero Public License.
* See the COPYING file in the main directory for details.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef FECVECTORS_H
#define FECVECTORS_H

#include "Vector.h"
#include <stdint.h>


class BitVector : public Vector<char> {


	public:

	/**@name Constructors. */
	//@{

	/**@name Casts of Vector constructors. */
	//@{
	BitVector(char* wData, char* wStart, char* wEnd)
		:Vector<char>(wData,wStart,wEnd)
	{ }
	BitVector(size_t len=0):Vector<char>(len) {}
	BitVector(const Vector<char>& source):Vector<char>(source) {}
	BitVector(Vector<char>& source):Vector<char>(source) {}
	BitVector(const Vector<char>& source1, const Vector<char> source2):Vector<char>(source1,source2) {}
	//@}

	/** Construct from a string of "0" and "1". */
	BitVector(const char* valString);
	//@}

	/** Index a single bit. */
	bool bit(size_t index) const
	{
		// We put this code in .h for fast inlining.
		const char *dp = mStart+index;
		assert(dp<mEnd);
		return (*dp) & 0x01;
	}

	/**@name Casts and overrides of Vector operators. */
	//@{
	BitVector segment(size_t start, size_t span)
	{
		char* wStart = mStart + start;
		char* wEnd = wStart + span;
		assert(wEnd<=mEnd);
		return BitVector(NULL,wStart,wEnd);
	}

	BitVector alias()
		{ return segment(0,size()); }

	const BitVector segment(size_t start, size_t span) const
		{ return (BitVector)(Vector<char>::segment(start,span)); }

	BitVector head(size_t span) { return segment(0,span); }
	const BitVector head(size_t span) const { return segment(0,span); }
	BitVector tail(size_t start) { return segment(start,size()-start); }
	const BitVector tail(size_t start) const { return segment(start,size()-start); }
	//@}


	void zero() { fill(0); }


	/** Invert 0<->1. */
	void invert();

	/**@name Byte-wise operations. */
	//@{
	/** Reverse an 8-bit vector. */
	void reverse8();
	/** Reverse groups of 8 within the vector (byte reversal). */
	void LSB8MSB();
	//@}

	/**@name Serialization and deserialization. */
	//@{
	uint64_t peekField(size_t readIndex, unsigned length) const;
	uint64_t peekFieldReversed(size_t readIndex, unsigned length) const;
	uint64_t readField(size_t& readIndex, unsigned length) const;
	uint64_t readFieldReversed(size_t& readIndex, unsigned length) const;
	void fillField(size_t writeIndex, uint64_t value, unsigned length);
	void fillFieldReversed(size_t writeIndex, uint64_t value, unsigned length);
	void writeField(size_t& writeIndex, uint64_t value, unsigned length);
	void writeFieldReversed(size_t& writeIndex, uint64_t value, unsigned length);
	void write0(size_t& writeIndex) { writeField(writeIndex,0,1); }
	void write1(size_t& writeIndex) { writeField(writeIndex,1,1); }

	//@}

	/** Sum of bits. */
	unsigned sum() const;

	/** Reorder bits, dest[i] = this[map[i]]. */
	void map(const unsigned *map, size_t mapSize, BitVector& dest) const;

	/** Reorder bits, dest[map[i]] = this[i]. */
	void unmap(const unsigned *map, size_t mapSize, BitVector& dest) const;

	/** Pack into a char array. */
	void pack(unsigned char*) const;

	/** Unpack from a char array. */
	void unpack(const unsigned char*);

	/** Make a hexdump string. */
	void hex(std::ostream&) const;
	std::string hexstr() const;

	/** Unpack from a hexdump string.
	*  @returns true on success, false on error. */
	bool unhex(const char*);

	void set(BitVector other)	// That's right.  No ampersand.
	{
		clear();
		mData=other.mData;
		mStart=other.mStart;
		mEnd=other.mEnd;
		other.mData=NULL;
	}

	void settfb(int i, int j) const
	{
		mStart[i] = j;
	}

};



std::ostream& operator<<(std::ostream&, const BitVector&);






/**
  The SoftVector class is used to represent a soft-decision signal.
  Values 0..1 represent probabilities that a bit is "true".
 */
class SoftVector: public Vector<float> {

	public:

	/** Build a SoftVector of a given length. */
	SoftVector(size_t wSize=0):Vector<float>(wSize) {}

	/** Construct a SoftVector from a C string of "0", "1", and "X". */
	SoftVector(const char* valString);

	/** Construct a SoftVector from a BitVector. */
	SoftVector(const BitVector& source);

	/**
		Wrap a SoftVector around a block of floats.
		The block will be delete[]ed upon desctuction.
	*/
	SoftVector(float *wData, unsigned length)
		:Vector<float>(wData,length)
	{}

	SoftVector(float* wData, float* wStart, float* wEnd)
		:Vector<float>(wData,wStart,wEnd)
	{ }

	/**
		Casting from a Vector<float>.
		Note that this is NOT pass-by-reference.
	*/
	SoftVector(Vector<float> source)
		:Vector<float>(source)
	{}


	/**@name Casts and overrides of Vector operators. */
	//@{
	SoftVector segment(size_t start, size_t span)
	{
		float* wStart = mStart + start;
		float* wEnd = wStart + span;
		assert(wEnd<=mEnd);
		return SoftVector(NULL,wStart,wEnd);
	}

	SoftVector alias()
		{ return segment(0,size()); }

	const SoftVector segment(size_t start, size_t span) const
		{ return (SoftVector)(Vector<float>::segment(start,span)); }

	SoftVector head(size_t span) { return segment(0,span); }
	const SoftVector head(size_t span) const { return segment(0,span); }
	SoftVector tail(size_t start) { return segment(start,size()-start); }
	const SoftVector tail(size_t start) const { return segment(start,size()-start); }
	//@}

	// How good is the SoftVector in the sense of the bits being solid?
	// Result of 1 is perfect and 0 means all the bits were 0.0
	// If plow is non-NULL, also return the lowest energy bit.
	float getEnergy(float *low=0) const;

	/** Fill with "unknown" values. */
	void unknown() { fill(0.0F); }

	/** Return a hard bit value from a given index by slicing. */
	bool bit(size_t index) const
	{
		const float *dp = mStart+index;
		assert(dp<mEnd);
		return (*dp)>0.0F;
	}

	/** Slice the whole signal into bits. */
	BitVector sliced() const;

};



std::ostream& operator<<(std::ostream&, const SoftVector&);






#endif
// vim: ts=4 sw=4
