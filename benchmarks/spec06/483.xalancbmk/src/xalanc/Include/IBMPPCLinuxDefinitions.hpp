#if !defined(IBMPPCLINUXDEFINITIONS_HEADER_GUARD_1357924680)
#define IBMPPCLINUXDEFINITIONS_HEADER_GUARD_1357924680

// ---------------------------------------------------------------------------
//  A define in the build for each project is also used to control whether
//  the export keyword is from the project's viewpoint or the client's.
//  These defines provide the platform specific keywords that they need
//  to do this.
// ---------------------------------------------------------------------------


#define XALAN_PLATFORM_EXPORT
#define XALAN_PLATFORM_IMPORT
#define XALAN_PLATFORM_EXPORT_FUNCTION(T) T XALAN_PLATFORM_EXPORT
#define XALAN_PLATFORM_IMPORT_FUNCTION(T) T XALAN_PLATFORM_IMPORT


#define XALAN_LSTRSUPPORT
#if !defined(XML_BITSTOBUILD_64)
#define XALAN_USE_WCHAR_CAST_HACK
#endif
#define XALAN_POSIX2_AVAILABLE
#define XALAN_XALANDOMCHAR_USHORT_MISMATCH

#define XALAN_EXPLICIT_SCOPE_IN_TEMPLATE_BUG
#define XALAN_NEW_STD_ALLOCATOR
#define XALAN_HAS_CPP_NAMESPACE
#define XALAN_CANNOT_DELETE_CONST

#define XALAN_UNALIGNED


#endif	// AIXDEFINITIONS_HEADER_GUARD_1357924680
