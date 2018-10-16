<<<<<<< HEAD
# Install script for directory: C:/Proyectos/Algo3Diego/Algo3_TP2/codigo
=======
# Install script for directory: E:/Proyectos/Algo3Diego/Algo3_TP2/codigo
>>>>>>> 011efd61039637a904f5eed584b038e68505ca1a

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Codigo")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< HEAD
file(WRITE "C:/Proyectos/Algo3Diego/Algo3_TP2/codigo/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "E:/Proyectos/Algo3Diego/Algo3_TP2/codigo/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> 011efd61039637a904f5eed584b038e68505ca1a
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
