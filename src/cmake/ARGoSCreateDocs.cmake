if(ARGOS_DOCUMENTATION)
  #
  # Process man page
  #
  add_custom_command(OUTPUT argos3.1.gz
    COMMAND gzip -9 -c ${CMAKE_SOURCE_DIR}/../doc/argos3.1 > argos3.1.gz
    DEPENDS ${CMAKE_SOURCE_DIR}/../doc/argos3.1)
  add_custom_target(man
    DEPENDS argos3.1.gz)
  install(FILES ${CMAKE_BINARY_DIR}/argos3.1.gz DESTINATION man/man1)
  set(ARGOS_DOCUMENTATION_DEPENDS man)

  #
  # Create doxygen documentation
  #
  if(ARGOS_BUILD_API)
    configure_file(${CMAKE_SOURCE_DIR}/../doc/Doxyfile.in
      ${CMAKE_BINARY_DIR}/Doxyfile
      @ONLY)
    add_custom_target(api
      COMMAND
      ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/../doc/api DESTINATION doc/argos3)
    set(ARGOS_DOCUMENTATION_DEPENDS ${ARGOS_DOCUMENTATION_DEPENDS} api)
  endif(ARGOS_BUILD_API)

  #
  # Add a target doc so that a 'make doc' creates all documentation
  #
  add_custom_target(doc
    DEPENDS ${ARGOS_DOCUMENTATION_DEPENDS})
endif(ARGOS_DOCUMENTATION)
