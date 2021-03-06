#ifndef __CONFIG_H__
#define __CONFIG_H__

/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define to 1 if you have the <ext/hash_map> header file. */
#undef HAVE_EXT_HASH_MAP

/* Define to 1 if you have the <google/dense_hash_map> header file. */
#define HAVE_GOOGLE_DENSE_HASH_MAP 1

/* Define to 1 if you have the <gtest/gtest.h> header file. */
#define HAVE_GTEST_GTEST_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

#ifdef _WIN32

#include <malloc.h>

#include "stdint.h"
#include "win32.h"

#endif // _WIN32

#endif // __CONFIG_H__
