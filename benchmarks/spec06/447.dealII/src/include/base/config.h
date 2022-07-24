//----------------------------  config.h  ---------------------------
//  This is an umbrella config file that first defines flags pertaining
//  to compiler bugs to their default value; it then includes the 
//  appropriate "real" config.h_xx file where deviating values can
//  be entered for individual compilers.


#ifndef deal_II_config_h
#define deal_II_config_h

/* Flag indicating whether there is a bug in the compiler that leads to bogus
   warnings for inline class members in anonymous namespaces */
/* #undef DEAL_II_ANON_NAMESPACE_BOGUS_WARNING */

/* Defined if the compiler needs to see the static keyword even for functions
   in anonymous namespaces, to avoid duplicate symbol errors when linking. For
   the details, look at aclocal.m4 in the top-level directory. */
/* #undef DEAL_II_ANON_NAMESPACE_BUG */

/* Another test if the compiler needs to see the static keyword even for
   functions in anonymous namespaces, to avoid duplicate symbol errors when
   linking. For the details, look at aclocal.m4 in the top-level directory. */
/* #undef DEAL_II_ANON_NAMESPACE_LINKAGE_BUG */

/* Defined if the compiler has a problem with assigning arrays in conditionals
   */
/* #undef DEAL_II_ARRAY_CONDITIONAL_DECAY_BUG */

/* Defined if the compiler needs a workaround for certain problems with taking
   the address of template template functions. For the details, look at
   aclocal.m4 in the top-level directory. */
/* #undef DEAL_II_FUNPTR_TEMPLATE_TEMPLATE_BUG */

/* Defined if the compiler refuses to compile the definition of a function
   that was previously declared abstract. */
/* #undef DEAL_II_IMPLEMENTED_PURE_FUNCTION_BUG */

/* Define if we have to work around a bug in Sun's Forte compiler. See the
   aclocal.m4 file in the top-level directory for a description of this bug.
   */
/* #undef DEAL_II_LOCAL_TYPEDEF_COMP_WORKAROUND */

/* Defined if the compiler gets an internal compiler upon some code involving
   long doubles, and with optimization. For the details, look at aclocal.m4 in
   the top-level directory. */
/* #undef DEAL_II_LONG_DOUBLE_LOOP_BUG */

/* Define if we have to work around a bug in gcc with explicitly instantiating
   template member operators. See the aclocal.m4 file in the top-level
   directory for a description of this bug. */
#define DEAL_II_MEMBER_OP_TEMPLATE_INST 

/* Defined if the compiler refuses to specialize an outer class template while
   keeping a member as a template. For the exact failure mode, look at
   aclocal.m4 in the top-level directory. */
/* #undef DEAL_II_MEMBER_TEMPLATE_SPECIALIZATION_BUG */

/* Defined if the compiler refuses to allow the explicit specialization of
   static member variables. For the exact failure mode, look at aclocal.m4 in
   the top-level directory. */
/* #undef DEAL_II_MEMBER_VAR_SPECIALIZATION_BUG */

/* Define if we have to work around a bug in gcc with marking all instances of
   a template class as friends to this class if the class is inside a
   namespace. See the aclocal.m4 file in the top-level directory for a
   description of this bug. */
/* #undef DEAL_II_NAMESP_TEMPL_FRIEND_BUG */

/* Define if we have to work around another bug in gcc with marking all
   instances of a template class as friends to this class if the class is
   inside a namespace. See the aclocal.m4 file in the top-level directory for
   a description of this bug. */
/* #undef DEAL_II_NAMESP_TEMPL_FRIEND_BUG2 */

/* Defined if the compiler does not properly implement the resolution of
   defect report #45 to the C++ standard, which makes nested types implicit
   friends of the enclosing class. */
/* #undef DEAL_II_NESTED_CLASS_FRIEND_BUG */

/* Defined if the compiler does not understand friend declarations for nested
   member classes when giving a full class specification. */
/* #undef DEAL_II_NESTED_CLASS_TEMPL_FRIEND_BUG */

/* Defined if the compiler refuses to allow a typedef to a template template
   class template parameter. For the exact failure mode, look at aclocal.m4 in
   the top-level directory. */
/* #undef DEAL_II_TEMPLATE_TEMPLATE_TYPEDEF_BUG */

/* Defined if the compiler requires the use of the template keyword for
   disambiguation keyword in certain contexts in which it is not supposed to
   do so. For the exact failure mode, look at aclocal.m4 in the top-level
   directory. */
/* #undef DEAL_II_TEMPL_OP_DISAMBIGUATION_BUG */

/* Define if we have to work around a bug with some compilers that will not
   allow us to specify a fully specialized class of a template as a friend.
   See the aclocal.m4 file in the top-level directory for a description of
   this bug. */
/* #undef DEAL_II_TEMPL_SPEC_FRIEND_BUG */

/* Flag indicating whether the library shall be compiled for multithreaded
   applications. If so, then it is set to one, otherwise to zero. */
#define DEAL_II_USE_MT 0

/* Defined if multi-threading is to be achieved by using the POSIX functions
   */
/* #undef DEAL_II_USE_MT_POSIX */

/* Defined if POSIX is supported but not the newer POSIX barrier functions.
   Barriers will then not work in the library, but the other threading
   functionality is available. */
/* #undef DEAL_II_USE_MT_POSIX_NO_BARRIERS */

/* Defined if a PETSc installation was found and is going to be used */
/* #undef DEAL_II_USE_PETSC */

/* Define if the compiler provides an <iosfwd> header file */
#define HAVE_STD_IOSFWD_HEADER 1

/* Define if the compiler's library in use provides a std::iterator class
   (early gcc versions did not) */
#define HAVE_STD_ITERATOR_CLASS 1

/* Define if the compiler's library in use provides std::numeric_limits
   classes in the appropriate header file */
#define HAVE_STD_NUMERIC_LIMITS 1

/* Define if the compiler provides an <ostream> header file */
#define HAVE_STD_OSTREAM_HEADER 1

/* Define if the compiler's library in use provides std::i/ostringstream
   classes (early gcc versions did not) */
#define HAVE_STD_STRINGSTREAM 1




/* The configs below are not used for CPU2006.  They've been moved to
 * 447.dealII/Docs/configs for reference.
 */
#if !defined(SPEC_CPU)
#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#  if (__GNUC__==3) && (__GNUC_MINOR__==2)
#    include <base/config.h.gcc-3.2>
#  elif (__GNUC__==3) && (__GNUC_MINOR__==3)
#    include <base/config.h.gcc-3.3>
#  elif (__GNUC__==3) && (__GNUC_MINOR__==4)
#    include <base/config.h.gcc-3.4>
#  elif (__GNUC__==3) && (__GNUC_MINOR__==5)
#    include <base/config.h.gcc-3.5>
#  else
#    include <base/config.h.gcc-4.0>
#  endif
#elif defined(__INTEL_COMPILER)
#  include <base/config.h.icc8>
#elif defined(_MSC_VER)
#  include <base/config.h.msvc>
#elif defined(__IBMCPP__)
#  include <base/config.h.xlC>
#elif SPEC_CPU_HPUX_IA64
#  include <base/config.h.HPUX1123IPF>
#elif SPEC_CPU_turboblasterAlpha
#  include <base/config.h_turboblasterAlpha>
#elif SPEC_CPU_turboblasterBeta
#  include <base/config.h_turboblasterBeta>
#elif  SPEC_CPU_IRIX
#  include <base/config.h.IRIX>
#elif defined(__SUNPRO_CC_COMPAT)
#  include <base/config.h.sunpro>
#elif defined(__PGI)
#  include <base/config.h.pgi>
#else
#  include <please/enter/your/compiler/and/config/file/here>
#endif
#endif /* SPEC_CPU */




/**
 * Have a namespace into which we declare some numeric constants, such
 * as pi. Unfortunately, these are not always available, on all systems
 * or depending on compiler flags, so we duplicate their declarations
 * here to make them unconditionally available to all other parts of the
 * library.
 *
 * The constants defined here are a subset of the <tt>M_XXX</tt> constants
 * sometimes declared in the system include file <tt>math.h</tt>, but without
 * the prefix <tt>M_</tt>.
 *
 * In addition to that, we declare  <tt>invalid_unsigned_int</tt> to be the
 * largest unsigned integer representable; this value is widely used in
 * the library as a marker for an invalid index, an invalid size of an
 * array, and similar purposes.
 */
namespace deal_II_numbers {
                                             /**
                                              * Representation of the
                                              * largest number that
                                              * can be put into an
                                              * unsigned integer. This
                                              * value is widely used
                                              * throughout the library
                                              * as a marker for an
                                              * invalid unsigned
                                              * integer value, such as
                                              * an invalid array
                                              * index, an invalid
                                              * array size, and the
                                              * like.
                                              */
  static const unsigned int
    invalid_unsigned_int = static_cast<unsigned int> (-1);

                                             /**
                                              * e
                                              */
  static const double  E       = 2.7182818284590452354;

                                             /**
                                              * log_2 e
                                              */
  static const double  LOG2E   = 1.4426950408889634074;

                                             /**
                                              * log_10 e
                                              */
  static const double  LOG10E  = 0.43429448190325182765;

                                             /**
                                              * log_e 2
                                              */
  static const double  LN2     = 0.69314718055994530942;

                                             /**
                                              * log_e 10
                                              */
  static const double  LN10    = 2.30258509299404568402;

                                             /**
                                              * pi
                                              */
  static const double  PI      = 3.14159265358979323846;

                                             /**
                                              * pi/2
                                              */
  static const double  PI_2    = 1.57079632679489661923;

                                             /**
                                              * pi/4
                                              */
  static const double  PI_4    = 0.78539816339744830962;

                                             /**
                                              * sqrt(2)
                                              */
  static const double  SQRT2   = 1.41421356237309504880;

                                             /**
                                              * 1/sqrt(2)
                                              */
  static const double  SQRT1_2 = 0.70710678118654752440;
}


#endif // deal_II_config_h
