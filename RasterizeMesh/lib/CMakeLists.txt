
#############################
SET ( LIBRARY_NAME   MeshFunctions  )
#############################

#----------------------------------------------------------------------------
# BUILD LIBRARY
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# LIBRARY HEADERS (TO BE INSTALLED)
# EITHER LIST ALL .h, *.txx IN CURRENT DIR USING NEXT LINE:

FILE(GLOB ${LIBRARY_NAME}_HEADERS "*.h" "*.txx")

#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# LIBRARY SOURCES (TO BE COMPILED)
# EITHER LIST ALL .cxx, *.cpp, *.cc IN CURRENT DIR USING NEXT LINE:

FILE(GLOB ${LIBRARY_NAME}_SOURCES *.cxx *.cpp *.cc)

#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# LIBRARY DEPENDENCIES (LIBRARIES TO LINK WITH)

SET ( ${LIBRARY_NAME}_LINK_LIBRARIES
  ${VTK_LIBRARIES}
  )
 
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# LINK
ADD_LIBRARY (${LIBRARY_NAME} ${${LIBRARY_NAME}_SOURCES} ${${LIBRARY_NAME}_HEADERS})  
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${${LIBRARY_NAME}_LINK_LIBRARIES} )
#
#----------------------------------------------------------------------------


