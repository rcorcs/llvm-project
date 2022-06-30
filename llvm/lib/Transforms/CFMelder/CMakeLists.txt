# If we don't need RTTI or EH, there's no reason to export anything
# from the hello plugin.
# if( NOT LLVM_REQUIRES_RTTI )
#   if( NOT LLVM_REQUIRES_EH )
#       set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CFMerger.exports)
#   endif()
# endif()

# if(WIN32 OR CYGWIN)
#   set(LLVM_LINK_COMPONENTS Core Support)
# endif()

# add_llvm_library( LLVMCFMelderPlugin MODULE BUILDTREE_ONLY
#     CFMelder.cpp
#     CFAnalyzer.cpp
#     CFMelderUtils.cpp 
#     InstructionMatch.cpp

#   DEPENDS
#   intrinsics_gen
#   PLUGIN_TOOL
#   opt
#   )

add_llvm_component_library(LLVMCFMelder
  CFMelder.cpp
  CFMelderUtils.cpp 
  RegionAnalyzer.cpp
  RegionMelder.cpp
  RegionReplicator.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/CFMelder



  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  Core
  Support
  )

