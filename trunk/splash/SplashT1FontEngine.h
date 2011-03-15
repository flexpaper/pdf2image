//========================================================================
//
// SplashT1FontEngine.h
//
//========================================================================

#ifndef SPLASHT1FONTENGINE_H
#define SPLASHT1FONTENGINE_H

#include <aconf.h>

#if HAVE_T1LIB_H

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "gtypes.h"

class SplashFontFile;
class SplashFontFileID;

//------------------------------------------------------------------------
// SplashT1FontEngine
//------------------------------------------------------------------------

class SplashT1FontEngine {
public:

  static SplashT1FontEngine *init(GBool aaA);

  ~SplashT1FontEngine();

  // Load fonts.
  SplashFontFile *loadType1Font(SplashFontFileID *idA, char *fileName,
				GBool deleteFile, char **enc);
  SplashFontFile *loadType1CFont(SplashFontFileID *idA, char *fileName,
				 GBool deleteFile, char **enc);

private:

  SplashT1FontEngine(GBool aaA);

  static int t1libInitCount;
  GBool aa;

  friend class SplashT1FontFile;
  friend class SplashT1Font;
};

#endif // HAVE_T1LIB_H

#endif
