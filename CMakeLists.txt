cmake_minimum_required (VERSION 2.8)

project (genuff CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (CMAKE_FIND_FRAMEWORK LAST)

# GDAL
find_package (GDAL REQUIRED)
include_directories (${GDAL_INCLUDE_DIRS})

# MLPACK
if (DEFINED ENV{MLPACK_ROOT})
    message ("@@ Use user provided library MLPACK.")
    message ("@@ MLPACK_ROOT = $ENV{MLPACK_ROOT}")
    set (MLPACK_INCLUDE_DIRS "$ENV{MLPACK_ROOT}/include")
    set (MLPACK_LIBRARY_DIRS "$ENV{MLPACK_ROOT}/lib")
    find_package(Armadillo REQUIRED)
    find_package(LibXml2 2.6.0 REQUIRED)
    include_directories (${MLPACK_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
    link_directories (${MLPACK_LIBRARY_DIRS})
else ()
    message (FATAL_ERROR
        "CMake couldn't find library MLPACK! "
        "If it have been installed and you know where it is, "
        "set MLPACK_ROOT (e.g. in .bashrc) to it."
    )
endif ()

# Set the source directory structure.
set (source_directories
     ${PROJECT_SOURCE_DIR}/src
)

# Collect sources and headers.
foreach (dir ${source_directories})
    include_directories ("${dir}")
    # header files
    file (GLOB tmp1 "${dir}/*.h")
    list (APPEND headers ${tmp1})
    # source files
    aux_source_directory ("${dir}" tmp2)
    list (APPEND sources ${tmp2})
endforeach ()

add_executable (genuff ${headers} ${sources})
target_link_libraries (genuff
    ${GDAL_LIBRARIES}

    mlpack
)