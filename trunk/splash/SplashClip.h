//========================================================================
//
// SplashClip.h
//
//========================================================================

#ifndef SPLASHCLIP_H
#define SPLASHCLIP_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "SplashTypes.h"
#include "SplashMath.h"

class SplashPath;
class SplashXPath;
class SplashXPathScanner;
class SplashBitmap;

//------------------------------------------------------------------------

enum SplashClipResult {
  splashClipAllInside,
  splashClipAllOutside,
  splashClipPartial
};

//------------------------------------------------------------------------
// SplashClip
//------------------------------------------------------------------------

class SplashClip {
public:

  // Create a clip, for the given rectangle.
  SplashClip(SplashCoord x0, SplashCoord y0,
	     SplashCoord x1, SplashCoord y1,
	     GBool antialiasA);

  // Copy a clip.
  SplashClip *copy() { return new SplashClip(this); }

  ~SplashClip();

  // Reset the clip to a rectangle.
  void resetToRect(SplashCoord x0, SplashCoord y0,
		   SplashCoord x1, SplashCoord y1);

  // Intersect the clip with a rectangle.
  SplashError clipToRect(SplashCoord x0, SplashCoord y0,
			 SplashCoord x1, SplashCoord y1);

  // Interesect the clip with <path>.
  SplashError clipToPath(SplashPath *path, SplashCoord *matrix,
			 SplashCoord flatness, GBool eo);

  // Returns true if (<x>,<y>) is inside the clip.
  GBool test(int x, int y);

  // Tests a rectangle against the clipping region.  Returns one of:
  //   - splashClipAllInside if the entire rectangle is inside the
  //     clipping region, i.e., all pixels in the rectangle are
  //     visible
  //   - splashClipAllOutside if the entire rectangle is outside the
  //     clipping region, i.e., all the pixels in the rectangle are
  //     clipped
  //   - splashClipPartial if the rectangle is part inside and part
  //     outside the clipping region
  SplashClipResult testRect(int rectXMin, int rectYMin,
			    int rectXMax, int rectYMax);

  // Similar to testRect, but tests a horizontal span.
  SplashClipResult testSpan(int spanXMin, int spanXMax, int spanY);

  // Clips an anti-aliased line by setting pixels to zero.  On entry,
  // all non-zero pixels are between <x0> and <x1>.  This function
  // will update <x0> and <x1>.
  void clipAALine(SplashBitmap *aaBuf, int *x0, int *x1, int y);

  // Get the rectangle part of the clip region, in integer coordinates.
  int getXMinI() { return xMinI; }
  int getXMaxI() { return xMaxI; }
  int getYMinI() { return yMinI; }
  int getYMaxI() { return yMaxI; }

  // Get the number of arbitrary paths used by the clip region.
  int getNumPaths() { return length; }

private:

  SplashClip(SplashClip *clip);
  void grow(int nPaths);

  GBool antialias;
  SplashCoord xMin, yMin, xMax, yMax;
  int xMinI, yMinI, xMaxI, yMaxI;
  SplashXPath **paths;
  Guchar *flags;
  SplashXPathScanner **scanners;
  int length, size;
};

#endif
