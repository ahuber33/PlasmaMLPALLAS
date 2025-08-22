#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.21)
#set (CMAKE_CXX_STANDARD 11)

# must be set before project(...) call; version module is needed before
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#
project(PlasmaMLPALLAS)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(GEANT4_USE_QT ON)
if(WITH_GEANT4_UIVIS)
#find_package(Geant4 REQUIRED ui_all vis_all)
find_package(Geant4 REQUIRED gdml ui_all vis_all)
else()
find_package(Geant4 REQUIRED)
endif()



# Chemins à adapter si jamais tu déplaces les libs
set(OnnxRuntime_DIR "/mnt/d/Simulations/PlasmaMLPALLAS/onnxruntime-linux-x64-1.17.1")
set(OnnxRuntime_INCLUDE_DIR "${OnnxRuntime_DIR}/include")
set(OnnxRuntime_LIBRARY "${OnnxRuntime_DIR}/lib/libonnxruntime.so")


find_package(OnnxRuntime REQUIRED)



#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
if(ROOT_FOUND)
  message(STATUS "ROOT ok !!!")
  message("Directory :" ${ROOT_USE_FILE})
endif()

# Include ROOT's CMake functions for dictionary generation
#  since root6.20, the file is renamed and included by default, so include
#  only when we find the *old* name
   if(EXISTS "${ROOT_DIR}/modules/RootNewMacros.cmake")
     include("${ROOT_DIR}/modules/RootNewMacros.cmake")
   endif()

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs


set(PROJECT_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASPhysics.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/Geometry.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASEventAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASVisManager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASGeometryMessenger.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASGeometryConstruction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASPrimaryGeneratorAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASPrimaryGeneratorMessenger.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASRunAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASSteppingAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASActionInitialization.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASMagneticField.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASOnnxInference.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PlasmaMLPALLASQuadrupoleUtils.cc
    )

set(PROJECT_HEADER
  ${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASPhysics.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/Geometry.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASEventAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASVisManager.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASGeometryMessenger.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASGeometryConstruction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASPrimaryGeneratorAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASPrimaryGeneratorMessenger.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASRunAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASSteppingAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASActionInitialization.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASOnnxInference.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASOnnxParameters.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASMagneticField.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PlasmaMLPALLASQuadrupoleUtils.hh
    )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(PlasmaMLPALLAS PlasmaMLPALLAS.cc ${PROJECT_HEADER} ${PROJECT_SRC})
#
target_link_libraries(PlasmaMLPALLAS ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

target_include_directories(PlasmaMLPALLAS PUBLIC ${OnnxRuntime_INCLUDE_DIR})
add_library(onnxruntime_lib SHARED IMPORTED)

# Spécifie le chemin complet vers la librairie (définie par la variable passée à CMake)
set_target_properties(onnxruntime_lib PROPERTIES
    IMPORTED_LOCATION "${OnnxRuntime_LIBRARY}"
)

target_link_libraries(PlasmaMLPALLAS onnxruntime_lib)


#link_directories( ${ROOT_LIBRARY_DIR} )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS PlasmaMLPALLAS DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(TARGETS PlasmaMLPALLAS DESTINATION ../bin)
message("Directory :" ${CMAKE_CURRENT_SOURCE_DIR}/bin)
