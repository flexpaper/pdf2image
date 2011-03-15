//========================================================================
//
// CharCodeToUnicode.h
//
// Mapping from character codes to Unicode.
//
// Copyright 2001-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef CHARCODETOUNICODE_H
#define CHARCODETOUNICODE_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "CharTypes.h"

#if MULTITHREADED
#include "GMutex.h"
#endif

struct CharCodeToUnicodeString;

//------------------------------------------------------------------------

class CharCodeToUnicode {
public:

  // Read the CID-to-Unicode mapping for <collection> from the file
  // specified by <fileName>.  Sets the initial reference count to 1.
  // Returns NULL on failure.
  static CharCodeToUnicode *parseCIDToUnicode(GString *fileName,
					      GString *collection);

  // Create a Unicode-to-Unicode mapping from the file specified by
  // <fileName>.  Sets the initial reference count to 1.  Returns NULL
  // on failure.
  static CharCodeToUnicode *parseUnicodeToUnicode(GString *fileName);

  // Create the CharCode-to-Unicode mapping for an 8-bit font.
  // <toUnicode> is an array of 256 Unicode indexes.  Sets the initial
  // reference count to 1.
  static CharCodeToUnicode *make8BitToUnicode(Unicode *toUnicode);

  // Parse a ToUnicode CMap for an 8- or 16-bit font.
  static CharCodeToUnicode *parseCMap(GString *buf, int nBits);

  // Parse a ToUnicode CMap for an 8- or 16-bit font, merging it into
  // <this>.
  void mergeCMap(GString *buf, int nBits);

  ~CharCodeToUnicode();

  void incRefCnt();
  void decRefCnt();

  // Return true if this mapping matches the specified <tagA>.
  GBool match(GString *tagA);

  // Set the mapping for <c>.
  void setMapping(CharCode c, Unicode *u, int len);

  // Map a CharCode to Unicode.
  int mapToUnicode(CharCode c, Unicode *u, int size);

  // Return the mapping's length, i.e., one more than the max char
  // code supported by the mapping.
  CharCode getLength() { return mapLen; }

private:

  void parseCMap1(int (*getCharFunc)(void *), void *data, int nBits);
  void addMapping(CharCode code, char *uStr, int n, int offset);
  CharCodeToUnicode(GString *tagA);
  CharCodeToUnicode(GString *tagA, Unicode *mapA,
		    CharCode mapLenA, GBool copyMap,
		    CharCodeToUnicodeString *sMapA,
		    int sMapLenA, int sMapSizeA);

  GString *tag;
  Unicode *map;
  CharCode mapLen;
  CharCodeToUnicodeString *sMap;
  int sMapLen, sMapSize;
  int refCnt;
#if MULTITHREADED
  GMutex mutex;
#endif
};

//------------------------------------------------------------------------

class CharCodeToUnicodeCache {
public:

  CharCodeToUnicodeCache(int sizeA);
  ~CharCodeToUnicodeCache();

  // Get the CharCodeToUnicode object for <tag>.  Increments its
  // reference count; there will be one reference for the cache plus
  // one for the caller of this function.  Returns NULL on failure.
  CharCodeToUnicode *getCharCodeToUnicode(GString *tag);

  // Insert <ctu> into the cache, in the most-recently-used position.
  void add(CharCodeToUnicode *ctu);

private:

  CharCodeToUnicode **cache;
  int size;
};

#endif
