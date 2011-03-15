/*
 * gmem.c
 *
 * Memory routines with out-of-memory checking.
 *
 * Copyright 1996-2003 Glyph & Cog, LLC
 */

#include <aconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "gmem.h"

#ifdef DEBUG_MEM

typedef struct _GMemHdr {
  unsigned int magic;
  int size;
  int index;
  struct _GMemHdr *next, *prev;
} GMemHdr;

#define gMemHdrSize ((sizeof(GMemHdr) + 7) & ~7)
#define gMemTrlSize (sizeof(long))

#define gMemMagic 0xabcd9999

#if gmemTrlSize==8
#define gMemDeadVal 0xdeadbeefdeadbeefUL
#else
#define gMemDeadVal 0xdeadbeefUL
#endif

/* round data size so trailer will be aligned */
#define gMemDataSize(size) \
  ((((size) + gMemTrlSize - 1) / gMemTrlSize) * gMemTrlSize)

static GMemHdr *gMemHead = NULL;
static GMemHdr *gMemTail = NULL;

static int gMemIndex = 0;
static int gMemAlloc = 0;
static int gMemInUse = 0;

#endif /* DEBUG_MEM */

void *gmalloc(int size) GMEM_EXCEP {
#ifdef DEBUG_MEM
  int size1;
  char *mem;
  GMemHdr *hdr;
  void *data;
  unsigned long *trl, *p;

  if (size <= 0) {
    return NULL;
  }
  size1 = gMemDataSize(size);
  if (!(mem = (char *)malloc(size1 + gMemHdrSize + gMemTrlSize))) {
#if USE_EXCEPTIONS
    throw GMemException();
#else
    fprintf(stderr, "Out of memory\n");
    exit(1);
#endif
  }
  hdr = (GMemHdr *)mem;
  data = (void *)(mem + gMemHdrSize);
  trl = (unsigned long *)(mem + gMemHdrSize + size1);
  hdr->magic = gMemMagic;
  hdr->size = size;
  hdr->index = gMemIndex++;
  if (gMemTail) {
    gMemTail->next = hdr;
    hdr->prev = gMemTail;
    gMemTail = hdr;
  } else {
    hdr->prev = NULL;
    gMemHead = gMemTail = hdr;
  }
  hdr->next = NULL;
  ++gMemAlloc;
  gMemInUse += size;
  for (p = (unsigned long *)data; p <= trl; ++p) {
    *p = gMemDeadVal;
  }
  return data;
#else
  void *p;

  if (size <= 0) {
    return NULL;
  }
  if (!(p = malloc(size))) {
#if USE_EXCEPTIONS
    throw GMemException();
#else
    fprintf(stderr, "Out of memory\n");
    exit(1);
#endif
  }
  return p;
#endif
}

void *grealloc(void *p, int size) GMEM_EXCEP {
#ifdef DEBUG_MEM
  GMemHdr *hdr;
  void *q;
  int oldSize;

  if (size <= 0) {
    if (p) {
      gfree(p);
    }
    return NULL;
  }
  if (p) {
    hdr = (GMemHdr *)((char *)p - gMemHdrSize);
    oldSize = hdr->size;
    q = gmalloc(size);
    memcpy(q, p, size < oldSize ? size : oldSize);
    gfree(p);
  } else {
    q = gmalloc(size);
  }
  return q;
#else
  void *q;

  if (size <= 0) {
    if (p) {
      free(p);
    }
    return NULL;
  }
  if (p) {
    q = realloc(p, size);
  } else {
    q = malloc(size);
  }
  if (!q) {
#if USE_EXCEPTIONS
    throw GMemException();
#else
    fprintf(stderr, "Out of memory\n");
    exit(1);
#endif
  }
  return q;
#endif
}

void *gmallocn(int nObjs, int objSize) GMEM_EXCEP {
  int n;

  if (nObjs == 0) {
    return NULL;
  }
  n = nObjs * objSize;
  if (objSize <= 0 || nObjs < 0 || nObjs >= INT_MAX / objSize) {
#if USE_EXCEPTIONS
    throw GMemException();
#else
    fprintf(stderr, "Bogus memory allocation size\n");
    exit(1);
#endif
  }
  return gmalloc(n);
}

void *greallocn(void *p, int nObjs, int objSize) GMEM_EXCEP {
  int n;

  if (nObjs == 0) {
    if (p) {
      gfree(p);
    }
    return NULL;
  }
  n = nObjs * objSize;
  if (objSize <= 0 || nObjs < 0 || nObjs >= INT_MAX / objSize) {
#if USE_EXCEPTIONS
    throw GMemException();
#else
    fprintf(stderr, "Bogus memory allocation size\n");
    exit(1);
#endif
  }
  return grealloc(p, n);
}

void gfree(void *p) {
#ifdef DEBUG_MEM
  int size;
  GMemHdr *hdr;
  unsigned long *trl, *clr;

  if (p) {
    hdr = (GMemHdr *)((char *)p - gMemHdrSize);
    if (hdr->magic == gMemMagic &&
	((hdr->prev == NULL) == (hdr == gMemHead)) &&
	((hdr->next == NULL) == (hdr == gMemTail))) {
      if (hdr->prev) {
	hdr->prev->next = hdr->next;
      } else {
	gMemHead = hdr->next;
      }
      if (hdr->next) {
	hdr->next->prev = hdr->prev;
      } else {
	gMemTail = hdr->prev;
      }
      --gMemAlloc;
      gMemInUse -= hdr->size;
      size = gMemDataSize(hdr->size);
      trl = (unsigned long *)((char *)hdr + gMemHdrSize + size);
      if (*trl != gMemDeadVal) {
	fprintf(stderr, "Overwrite past end of block %d at address %p\n",
		hdr->index, p);
      }
      for (clr = (unsigned long *)hdr; clr <= trl; ++clr) {
	*clr = gMemDeadVal;
      }
      free(hdr);
    } else {
      fprintf(stderr, "Attempted to free bad address %p\n", p);
    }
  }
#else
  if (p) {
    free(p);
  }
#endif
}

#ifdef DEBUG_MEM
void gMemReport(FILE *f) {
  GMemHdr *p;

  fprintf(f, "%d memory allocations in all\n", gMemIndex);
  if (gMemAlloc > 0) {
    fprintf(f, "%d memory blocks left allocated:\n", gMemAlloc);
    fprintf(f, " index     size\n");
    fprintf(f, "-------- --------\n");
    for (p = gMemHead; p; p = p->next) {
      fprintf(f, "%8d %8d\n", p->index, p->size);
    }
  } else {
    fprintf(f, "No memory blocks left allocated\n");
  }
}
#endif

char *copyString(char *s) {
  char *s1;

  s1 = (char *)gmalloc(strlen(s) + 1);
  strcpy(s1, s);
  return s1;
}
