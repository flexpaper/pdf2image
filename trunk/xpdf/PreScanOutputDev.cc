//========================================================================
//
// PreScanOutputDev.cc
//
// Copyright 2005 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <math.h>
#include "GlobalParams.h"
#include "GfxFont.h"
#include "Link.h"
#include "PreScanOutputDev.h"

//------------------------------------------------------------------------
// PreScanOutputDev
//------------------------------------------------------------------------

PreScanOutputDev::PreScanOutputDev() {
  clearStats();
}

PreScanOutputDev::~PreScanOutputDev() {
}

void PreScanOutputDev::startPage(int pageNum, GfxState *state) {
}

void PreScanOutputDev::endPage() {
}

void PreScanOutputDev::stroke(GfxState *state) {
  double *dash;
  int dashLen;
  double dashStart;

  check(state->getStrokeColorSpace(), state->getStrokeColor(),
	state->getStrokeOpacity(), state->getBlendMode());
  state->getLineDash(&dash, &dashLen, &dashStart);
  if (dashLen != 0) {
    gdi = gFalse;
  }
}

void PreScanOutputDev::fill(GfxState *state) {
  check(state->getFillColorSpace(), state->getFillColor(),
	state->getFillOpacity(), state->getBlendMode());
}

void PreScanOutputDev::eoFill(GfxState *state) {
  check(state->getFillColorSpace(), state->getFillColor(),
	state->getFillOpacity(), state->getBlendMode());
}

void PreScanOutputDev::clip(GfxState *state) {
  //~ check for a rectangle "near" the edge of the page;
  //~   else set gdi to false
}

void PreScanOutputDev::eoClip(GfxState *state) {
  //~ see clip()
}

void PreScanOutputDev::beginStringOp(GfxState *state) {
  int render;
  GfxFont *font;
  double m11, m12, m21, m22;
  Ref embRef;
  DisplayFontParam *dfp;
  GBool simpleTTF;

  render = state->getRender();
  if (!(render & 1)) {
    check(state->getFillColorSpace(), state->getFillColor(),
	  state->getFillOpacity(), state->getBlendMode());
  }
  if ((render & 3) == 1 || (render & 3) == 2) {
    check(state->getStrokeColorSpace(), state->getStrokeColor(),
	  state->getStrokeOpacity(), state->getBlendMode());
  }

  font = state->getFont();
  state->getFontTransMat(&m11, &m12, &m21, &m22);
  simpleTTF = fabs(m11 + m22) < 0.01 &&
              m11 > 0 &&
              fabs(m12) < 0.01 &&
              fabs(m21) < 0.01 &&
              fabs(state->getHorizScaling() - 1) < 0.001 &&
              (font->getType() == fontTrueType ||
	       font->getType() == fontTrueTypeOT) &&
              (font->getEmbeddedFontID(&embRef) ||
	       font->getExtFontFile() ||
	       (font->getName() &&
		(dfp = globalParams->getDisplayFont(font->getName())) &&
		dfp->kind == displayFontTT));
  if (simpleTTF) {
    //~ need to create a FoFiTrueType object, and check for a Unicode cmap
  }
  if (state->getRender() != 0 || !simpleTTF) {
    gdi = gFalse;
  }
}

void PreScanOutputDev::endStringOp(GfxState *state) {
}

GBool PreScanOutputDev::beginType3Char(GfxState *state, double x, double y,
				       double dx, double dy,
				       CharCode code, Unicode *u, int uLen) {
  // return false so all Type 3 chars get rendered (no caching)
  return gFalse;
}

void PreScanOutputDev::endType3Char(GfxState *state) {
}

void PreScanOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				     int width, int height, GBool invert,
				     GBool inlineImg) {
  int i, j;

  check(state->getFillColorSpace(), state->getFillColor(),
	state->getFillOpacity(), state->getBlendMode());
  gdi = gFalse;

  if (inlineImg) {
    str->reset();
    j = height * ((width + 7) / 8);
    for (i = 0; i < j; ++i)
      str->getChar();
    str->close();
  }
}

void PreScanOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
				 int width, int height,
				 GfxImageColorMap *colorMap,
				 int *maskColors, GBool inlineImg) {
  GfxColorSpace *colorSpace;
  int i, j;

  colorSpace = colorMap->getColorSpace();
  if (colorSpace->getMode() == csIndexed) {
    colorSpace = ((GfxIndexedColorSpace *)colorSpace)->getBase();
  }
  if (colorSpace->getMode() != csDeviceGray &&
      colorSpace->getMode() != csCalGray) {
    gray = gFalse;
  }
  mono = gFalse;
  if (state->getBlendMode() != gfxBlendNormal) {
    transparency = gTrue;
  }
  gdi = gFalse;

  if (inlineImg) {
    str->reset();
    j = height * ((width * colorMap->getNumPixelComps() *
		   colorMap->getBits() + 7) / 8);
    for (i = 0; i < j; ++i)
      str->getChar();
    str->close();
  }
}

void PreScanOutputDev::drawMaskedImage(GfxState *state, Object *ref,
				       Stream *str,
				       int width, int height,
				       GfxImageColorMap *colorMap,
				       Stream *maskStr,
				       int maskWidth, int maskHeight,
				       GBool maskInvert) {
  GfxColorSpace *colorSpace;

  colorSpace = colorMap->getColorSpace();
  if (colorSpace->getMode() == csIndexed) {
    colorSpace = ((GfxIndexedColorSpace *)colorSpace)->getBase();
  }
  if (colorSpace->getMode() != csDeviceGray &&
      colorSpace->getMode() != csCalGray) {
    gray = gFalse;
  }
  mono = gFalse;
  if (state->getBlendMode() != gfxBlendNormal) {
    transparency = gTrue;
  }
  gdi = gFalse;
}

void PreScanOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref,
					   Stream *str,
					   int width, int height,
					   GfxImageColorMap *colorMap,
					   Stream *maskStr,
					   int maskWidth, int maskHeight,
					   GfxImageColorMap *maskColorMap) {
  GfxColorSpace *colorSpace;

  colorSpace = colorMap->getColorSpace();
  if (colorSpace->getMode() == csIndexed) {
    colorSpace = ((GfxIndexedColorSpace *)colorSpace)->getBase();
  }
  if (colorSpace->getMode() != csDeviceGray &&
      colorSpace->getMode() != csCalGray) {
    gray = gFalse;
  }
  mono = gFalse;
  transparency = gTrue;
  gdi = gFalse;
}

void PreScanOutputDev::beginTransparencyGroup(
			   GfxState *state, double *bbox,
			   GfxColorSpace *blendingColorSpace,
			   GBool isolated, GBool knockout,
			   GBool forSoftMask) {
  transparency = gTrue;
  gdi = gFalse;
}

void PreScanOutputDev::check(GfxColorSpace *colorSpace, GfxColor *color,
			     double opacity, GfxBlendMode blendMode) {
  GfxRGB rgb;

  if (colorSpace->getMode() == csPattern) {
    mono = gFalse;
    gray = gFalse;
    gdi = gFalse;
  } else {
    colorSpace->getRGB(color, &rgb);
    if (rgb.r != rgb.g || rgb.g != rgb.b || rgb.b != rgb.r) {
      mono = gFalse;
      gray = gFalse;
    } else if (!((rgb.r == 0 && rgb.g == 0 && rgb.b == 0) ||
		 (rgb.r == gfxColorComp1 &&
		  rgb.g == gfxColorComp1 &&
		  rgb.b == gfxColorComp1))) {
      mono = gFalse;
    }
  }
  if (opacity != 1 || blendMode != gfxBlendNormal) {
    transparency = gTrue;
  }
}

void PreScanOutputDev::clearStats() {
  mono = gTrue;
  gray = gTrue;
  transparency = gFalse;
  gdi = gTrue;
}
