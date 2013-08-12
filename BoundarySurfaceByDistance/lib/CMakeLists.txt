
#############################
SET ( LIBRARY_NAME   MeshTools  )
#############################

#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# CREATES A USER OPTION IN CMAKE
OPTION ( BUILD_${LIBRARY_NAME}  "Build ${LIBRARY_NAME} library ?" ON)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
IF ( BUILD_${LIBRARY_NAME} )
#----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # BUILD LIBRARY
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY HEADERS (TO BE INSTALLED)
  # EITHER LIST ALL .h, *.txx IN CURRENT DIR USING NEXT LINE:

  FILE(GLOB ${LIBRARY_NAME}_HEADERS "*.h" "*.txx")
  
  # OR MANUALLY LIST YOUR HEADERS WITH NEXT COMMAND
  #  SET ( ${LIBRARY_NAME}_HEADERS
  #
  #      )
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY SOURCES (TO BE COMPILED)
  # EITHER LIST ALL .cxx, *.cpp, *.cc IN CURRENT DIR USING NEXT LINE:

  FILE(GLOB ${LIBRARY_NAME}_SOURCES *.cxx *.cpp *.cc)

  # OR MANUALLY LIST YOUR FILES WITH NEXT COMMAND (WITHOUT EXTENSION)
  #  SET ( ${LIBRARY_NAME}_SOURCES 
  #   
  #      )
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY DEPENDENCIES (LIBRARIES TO LINK WITH)
  #
  # USER! : Uncomment the Libraries you need
  #
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

  #---------------------------------------------------------------------------
ENDIF ( BUILD_${LIBRARY_NAME} )
