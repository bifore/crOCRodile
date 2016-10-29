cmake_minimum_required(VERSION 3.6)
project(crOCRodile)
find_package(PkgConfig REQUIRED)

set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -g -lm ")

# SOURCE FILES
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(crOCRodile ${SOURCE_FILES})

# LIBRARIES LIST
set(LIBRARIES
        gtk+-3.0)

# LIBRARY LINKER
message("-------- LINKING LIBRARIES ---------")
foreach (LIB ${LIBRARIES})
    message(" ---- LOOKING FOR ${LIB}")

    pkg_check_modules(${LIB} REQUIRED ${LIB})
    include_directories(${${LIB}_INCLUDE_DIRS})
    link_directories(${${LIB}_LIBRARY_DIRS})
    add_definitions(${${LIB}_CFLAGS_OTHER})

    message("  -- FOUND ${LIB} as :")
    message("    incldirs    : ${${LIB}_INCLUDE_DIRS}")
    message("    librarydirs : ${${LIB}_LIBRARY_DIRS}")
    message("    definitions : ${${LIB}_CFLAGS_OTHER}")

    target_link_libraries(crOCRodile ${${LIB}_LIBRARIES})
endforeach ()

# RESOURCES LIST
set(RESOURCES
        resources)

# RESOURCES LINKER
foreach (RESOURCE ${RESOURCES})
    message("LINKING RESOURCE : ${RESOURCE}")
    add_custom_command(TARGET crOCRodile POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${RESOURCE} $<TARGET_FILE_DIR:crOCRodile>)
endforeach ()
