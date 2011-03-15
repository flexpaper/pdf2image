/* aconf.h.  Generated automatically by configure.  */
/*
 * aconf.h
 *
 * Copyright 2002 Derek B. Noonburg
 */

#ifndef ACONF_H
#define ACONF_H

/*
 * Use A4 paper size instead of Letter for PostScript output.
 */
/* #undef A4_PAPER */

/*
 * Do not allow text selection.
 */
/* #undef NO_TEXT_SELECT */

/*
 * Include support for OPI comments.
 */
/* #undef OPI_SUPPORT */

/*
 * Use gzip instead of uncompress.
 */
/* #undef USE_GZIP */

/*
 * Directory with the Xpdf app-defaults file.
 */
/* #undef APPDEFDIR */

/*
 * Full path for the system-wide xpdfrc file.
 */
#define SYSTEM_XPDFRC "/usr/local/etc/xpdfrc"

/*
 * Various include files and functions.
 */
#define HAVE_DIRENT_H 1
/* #undef HAVE_SYS_NDIR_H */
/* #undef HAVE_SYS_DIR_H */
/* #undef HAVE_NDIR_H */
/* #undef HAVE_SYS_SELECT_H */
/* #undef HAVE_SYS_BSDTYPES_H */
#define HAVE_STRINGS_H 1
/* #undef HAVE_BSTRING_H */
#define HAVE_POPEN 1
#define HAVE_MKSTEMP 1
#define HAVE_MKSTEMPS 0
/* #undef SELECT_TAKES_INT */
#define GHOSTSCRIPT "gs"

/*
 * This is defined if using libXpm.
 */
#define HAVE_X11_XPM_H 1

/*
 * This is defined if using t1lib.
 */
/* #undef HAVE_T1LIB_H */

/*
 * One of these is defined if using FreeType (version 1 or 2).
 */
#define HAVE_FREETYPE_FREETYPE_H 1

/*
 * This is defined if using FreeType version 2.
 */
/* #undef FREETYPE2 */

/*
 * This is defined if using libpaper.
 */
/* #undef HAVE_PAPER_H */


/*
 * Defined if the Splash library is avaiable.
 */
#define HAVE_SPLASH 1

#endif
