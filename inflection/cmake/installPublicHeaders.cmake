#
# Copyright 2021-2024 Apple Inc. All rights reserved.
#
file(GLOB_RECURSE INFLECTION_HEADERS "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/*.hpp")
foreach(INFLECTION_HEADER IN LISTS INFLECTION_HEADERS)
    file(READ ${INFLECTION_HEADER} HEADER_TEXT)
    if ("${HEADER_TEXT}" MATCHES "INFLECTION_C(LASS_)?API")
        string(REGEX REPLACE "^${SOURCE_DIR}" "${PACKAGE_DIR}" PUBLIC_HEADER ${INFLECTION_HEADER})
        get_filename_component(PUBLIC_HEADER_DIR ${PUBLIC_HEADER} DIRECTORY)
        file(INSTALL ${INFLECTION_HEADER} DESTINATION ${PUBLIC_HEADER_DIR})
    endif ()
endforeach()
file(INSTALL ${BINARY_DIR}/resources/inflection/version.h DESTINATION ${PACKAGE_DIR}/inflection)
