cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -fPIC -g)

project(zubr-sdk)
add_subdirectory(Deps/pybind11)

# Building CXX Library
file( GLOB SRCS Src/*)
add_library(ZubrSDK ${SRCS})
target_include_directories(ZubrSDK PUBLIC Inc/)

# Building Python shared lib
pybind11_add_module(zubrsdk Binding/PyBinding.cpp)
target_link_libraries(zubrsdk PUBLIC ZubrSDK)

execute_process(COMMAND python3 -c "import site; print(site.getsitepackages()[0])"
  OUTPUT_VARIABLE PYTHON3_SITEDIR 
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(INFO " Python site-packages directory: ${PYTHON3_SITEDIR}")
install(TARGETS zubrsdk DESTINATION ${PYTHON3_SITEDIR})

# Building test script
add_executable(Test Misc/Test.cpp)
target_link_libraries(Test PRIVATE ZubrSDK)

# Building test script
add_executable(ADCCalib Misc/ADCCalib.cpp)
target_link_libraries(ADCCalib PRIVATE ZubrSDK)