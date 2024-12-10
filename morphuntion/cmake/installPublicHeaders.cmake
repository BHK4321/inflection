#
# Copyright 2021-2024 Apple Inc. All rights reserved.
#
file(GLOB_RECURSE MORPHUNTION_HEADERS "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/*.hpp")
foreach(MORPHUNTION_HEADER IN LISTS MORPHUNTION_HEADERS)
    file(READ ${MORPHUNTION_HEADER} HEADER_TEXT)
    if ("${HEADER_TEXT}" MATCHES "MORPHUNTION_C(LASS_)?API")
        string(REGEX REPLACE "^${SOURCE_DIR}" "${PACKAGE_DIR}" PUBLIC_HEADER ${MORPHUNTION_HEADER})
        get_filename_component(PUBLIC_HEADER_DIR ${PUBLIC_HEADER} DIRECTORY)
        file(INSTALL ${MORPHUNTION_HEADER} DESTINATION ${PUBLIC_HEADER_DIR})
    endif ()
endforeach()
file(INSTALL ${BINARY_DIR}/resources/morphuntion/version.h DESTINATION ${PACKAGE_DIR}/morphuntion)
