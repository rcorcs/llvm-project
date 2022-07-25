#if !defined(CONFIG_H)
#define CONFIG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>
#if !defined(SPEC_CPU_WINDOWS_ICL)
#include <math.h>
#else
#include <mathimf.h>
#endif /* SPEC_CPU_WINDOWS_ICL */
#if !defined(SPEC_CPU_WINDOWS)
#include <unistd.h> // unlink
#else
#include <direct.h>
#define getcwd _getcwd
#define vsnprintf _vsnprintf
#endif /* SPEC_CPU_WINDOWS */

// ****************************************************************************************************
// The following are changes from the default, less portable configuration of POV-Ray.
// There should be no need to edit them as they provide a rather conservative configuration.
// ****************************************************************************************************
#if defined(SPEC_CPU)
namespace std {} 
#endif /* SPEC_CPU */
using namespace std;

// disable memory leak tracking and memory usage statistics
#define MEM_RECLAIM							0
#define MEM_STATS							0
#if defined(SPEC_CPU)
#define MEM_TRACE							0
#endif /* SPEC_CPU */

// general precision used when comparing two floating-point (double) values for equality
#define EPSILON								1.0e-7 // 1.0e-10

// distribution specific information marking this as an official but "special" version for use by SPEC
#define POVRAY_PLATFORM_NAME				"for SPEC"
#define COMPILER_VER						"by POV-Team"

// disables support for external image format support library version display
// the image format support libraries are not needed by this version of POV-Ray
#define DONT_SHOW_IMAGE_LIB_VERSIONS		1

// message displayed when the POV-ray scene description parser detects a feature that has been disabled
#define POV_SPEC_NOT_SUPPORTED				"This feature is not supported in the POV-Ray 3.6 SPEC version!"

// display render statistics for every line instead of regular and much simpler progress ouput
#define POV_SPEC_MORE_RENDER_STATS			1

// disables a not fully portable remote-control feature of POVMS
#define POVMS_NO_ORDERED_STREAM_DATA		1

// disables a not fully portable debug feature of POVMS
#define POVMS_NO_DUMP_SUPPORT

// usually setjmp is used to exit gracefully in case of a fatal error
// however, the SPEC version of POV-Ray is not meant for general user use and thus simply returns an error code
#define POVRAY_COOPERATE_GLOBAL
#define POVRAY_BEGIN_COOPERATE	{
#define POVRAY_END_COOPERATE	}
#define	EXIT_POVRAY(n)			if(n > 0) exit(-n); else exit(n)

// Mac OS 9 GUI using Metrowerks CodeWarrior specific (the only C++ compiler which still supports Mac OS 9)
// presents a GUI dialog to get the command-line as there is no command-line support in Mac OS 9 and earlier
#if defined(TARGET_OS_MAC) && defined(__MWERKS__)
	#include <console.h>
	#define GETCOMMANDLINE(ac,av) ccommand(&(av)); //getcmdline(&av);
	#define FILENAME_SEPARATOR ':'
#endif // TARGET_OS_MAC && __MWERKS__


// ****************************************************************************************************
// The following are changes from the default configuration of POV-Ray.
// These changes from the default configuration of POV-Ray exist in order to prevent the code
// from not compiling on non ISO C++ 98 standard-conforming compilers. However, for a peroper opertion
// on standard conforming compilers they may need to be adjusted.
// ****************************************************************************************************

// POV-Ray is able to track memory managed by new/delete using some not always implemented ISO C++ features.
// All dependencies on these features are disabled by these defines, which has the effect that if memory
// allocation fails using a standard-conforming new (which throws an excpetion rather than returning NULL),
// the exception will not be caught! This may result in the application terminating without a proper error
// code!!!
#define POV_CPP_MEM_HAS_NEW_INCLUDE			0
#define POV_CPP_MEM_HAS_PLACEMENT_FORMS		0
#define POV_CPP_MEM_HAS_NOTHROW_SUPPORT		0

// Some code performing the radiosity calculation (not used in the supplied test scene) depends on "float"
// being a 32 bit IEEE 754 floating point number.  This is of course not portable.  However, functions to
// extract the necessary information had not been available in many implementations until recently.  These
// functions are either logbf/logb and copysign or ilogbf/ilogb and copysign.  The define below assumes
// logbf, logb and copysign are available.  For more information, see file octree.cpp line 72.
#if defined(SPEC_CPU_NEED_INVHYP)
#define C99_COMPATIBLE_RADIOSITY 0
#else
#define C99_COMPATIBLE_RADIOSITY 3
#endif /* SPEC_CPU_NEED_INVHYP */

// ****************************************************************************************************
// The following are changes from the default configuration of POV-Ray.
// These  changes from the default configuration of POV-Ray exist to let the code run on all
// standard-conforming compilers whose "int" data type is at least 32 bit in size.
//
// Note about integer data types in POV-Ray:
// For use on 64 bit platforms, most code in POV-Ray assumes those platforms follow the LP64 programming model.
// (See <http://www.opengroup.org/public/tech/aspen/lp64_wp.htm> for details.)
// In addition to this, on 32 bit platforms POV-Ray will not function if the platform "int" is not at least
// 32 bit in size.  In short, POV-Ray has only been run and tested on platforms either using the LP64, LLP64
// or ILP32 programming models.
// Further, even on 32 bit platforms, which are still the most common type of platform used by POV-Ray users,
// there is real use for 64 bit integer support in POV-Ray as (non-essential) statistics gathering easily
// results in numbers that cannot be represented in a 32 bit integer (signed or unsigned).  However, if the
// precise statistics are not needed, no problems have ever been reporting in using a 32 bit integer for this
// purpose.  On the other hand, no platform officially supported by the POV-Team does not offer at least a
// non-standard 64 bit integer type and hence there is little experience using 32 bit POV_LONG.
// ****************************************************************************************************

// For statistics gathering of very complex scenes, usually a 32 bit integer is not sufficient. By default
// POV-Ray uses the common non-standard extension (on compilers targeting 32 bit platforms) "long long".
// However, as this is non-portable, the following simply uses long.  It would be preferred if this would
// be defined to a 64 bit integer type, but for the supplied test scene the statistics counter will
// leave the range of a 32 bit signed integer anyway.
#if !defined(POV_LONG)
# if defined(SPEC_CPU_LP64) || defined(SPEC_CPU_ILP64)
#  define POV_LONG long
# elif defined(SPEC_CPU_WINDOWS)
#  define POV_LONG __int64
# else
#  define POV_LONG long long
# endif /* SPEC_CPU_LP64 || SPEC_CPU_ILP64 */
#endif /* !defined(POV_LONG) */

// There is a second place where POV-Ray needs a 64 bit integer type.  The code depends on it being 8 bytes
// in size and hence the following include a small support class which provides such a type if no native
// 64 bit integer type is available.  However, this is not really recommended and has never been well tested.
#if !defined(SPEC_CPU)
#include "povmslng.h"
#endif /* SPEC_CPU */



// ****************************************************************************************************
// Note about floating-point data types in POV-Ray:
// By default POV-Ray assumes "float" is 32 bit in size and "double" is 64 bit in size, and usually it is assumed
// both offer a precision similar to that of IEEE 754 floating-point 32/64 bit numbers.  Of course these defaults
// can be changed here in config.h.
// Further, as far as the POV-Team is knowns, POV-Ray has never been ported to a platform that did not offer some
// kind 32 bit and 64 bit floating-point numbers as native format.  Hence, tzhe code has never been fully tested
// on platforms were either a 32 bit and 64 bit floating-point floating-point format is not available.
// ****************************************************************************************************

// currently no floating-point specific default is changed


#endif // CONFIG_H
