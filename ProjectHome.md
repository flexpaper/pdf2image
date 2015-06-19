PDF2Image is a conversion library based on XPDF (3.02) which can be used for high performance PDF page by page conversion to images. It exports text data to XML and JSON format. It also supports compressing data to minimize size. PDF2Image is available for Windows and Linux.

It requires Ghostscript and Freetype to be installed in order to work. Make sure Ghostscript is part of your system path before running PDF2Image.

```
Usage: pdf2image [options] <PDF-file> [<xml-file>]
  -f <int>          : first page to convert
  -l <int>          : last page to convert
  -JSON             : Write text data in JSON format
  -compress         : Use compressed mode
  -q                : dont print any messages or errors
  -h                : print usage information
  -help             : print usage information
  -i                : ignore images
  -noframes         : use standard output
  -zoom <fp>        : zoom the pdf document (default 1.5)
  -xml              : output for XML post-processing
  -hidden           : output hidden text
  -enc <string>     : output text encoding name
  -dev <string>     : output device name for Ghostscript (png16m, jpeg etc)
  -v                : print copyright and version info
  -opw <string>     : owner password (for encrypted files)
  -upw <string>     : user password (for encrypted files)
```

Ghostscript can be downloaded from  the [Google Code Ghostscript download area](http://code.google.com/p/ghostscript/downloads/list)

---

### Maintained and sponsored by  █▒▓▒░ **[The FlexPaper Project](http://flexpaper.devaldi.com)** ###