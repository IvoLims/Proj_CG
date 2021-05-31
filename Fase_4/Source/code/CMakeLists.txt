cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(fase_4)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

add_executable(${PROJECT_NAME} engine.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")

	message(STATUS "Xml_DIR set to: " ${XML_FOLDER})
        set(XML_FOLDER "" CACHE PATH "Path to XML folder")

	message(STATUS "FigTex_DIR set to: " ${FIGTEX_FOLDER})
	set(FIGTEX_FOLDER "" CACHE PATH "Path to FIGTEX folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/DevIL.lib")
		message(ERROR ": DEVIL not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/DevIL.lib")

	if (NOT EXISTS "${FIGTEX_FOLDER}" OR NOT EXISTS "${FIGTEX_FOLDER}")
		message(ERROR ": TEXTURES not found")
	endif (NOT EXISTS "${FIGTEX_FOLDER}" OR NOT EXISTS "${FIGTEX_FOLDER}")

	
	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil ${XML_FOLDER} ${FIGTEX_FOLDER})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
										  ${TOOLKITS_FOLDER}/glut/glut32.lib 
										  ${TOOLKITS_FOLDER}/glew/glew32.lib 
										  ${TOOLKITS_FOLDER}/devil/DevIL.lib )
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll" AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll" AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")

	if (EXISTS "${XML_FOLDER}")
		file(COPY ${XML_FOLDER}/config/xmlconf.xml DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${XML_FOLDER}")

	if (EXISTS "${FIGTEX_FOLDER}")
	    file(COPY ${FIGTEX_FOLDER}/textures/ DESTINATION ${CMAKE_BINARY_DIR})
	    file(COPY ${FIGTEX_FOLDER}/figs/ DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${FIGTEX_FOLDER}")
	
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGSÂ  "${CMAKE_CXX_FLAGS} Wno-deprecated")

	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
	
	find_package(DevIL REQUIRED)
    link_libraries(${IL_LIBRARIES} )
    include_directories(${IL_INCLUDE_DIR})

	if (NOT APPLE)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif(NOT APPLE)

	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${IL_LIBRARIES})

	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	if(NOT GLEW_FOUND)
	   message(ERROR ": GLEW not found!")
	endif(NOT GLEW_FOUND)
	if(NOT IL_FOUND)
	   message(ERROR ": DEVIL not found!")
	endif(NOT IL_FOUND)
endif(WIN32)
