CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(metagam3d)

FIND_PACKAGE(OpenSceneGraph)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(Boost 1.41 COMPONENTS python)
FIND_PACKAGE(PythonLibs)

SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

ADD_LIBRARY(metagam3d SHARED
    src/StacklessChannelValue.cc
    src/StacklessChannel.cc
    src/StacklessMessage.cc
    src/StacklessMailbox.cc
    src/StacklessThread.cc
    src/StacklessMath.cc
    src/OSGMessage.cc
    src/OSGMailbox.cc
    src/Engine.cc
    src/errors.cc
    src/Object.cc
    src/ObjectManager.cc
    src/Variant.cc
    src/DynamicValue.cc
    src/StacklessObject.cc
    src/RunScriptCommand.cc
    src/StacklessObjectParam.cc
    src/ObjectParamHandler.cc
    src/ObjectParamSetMessage.cc
    src/ObjectMotionHandler.cc
    src/ObjectTextHandler.cc
    src/ScriptEnvironment.cc
    src/ScriptExpression.cc
    src/ScriptAssembler.cc
    src/CameraManipulator.cc
    src/StacklessObjectLoader.cc
    src/StacklessText.cc
    src/ObjectMaterialHandler.cc
    src/StacklessMaterialParam.cc
    src/OSGConsole.cc
    src/ConsoleObject.cc
    src/StacklessConsole.cc
    src/ObjectVectorHandler.cc
    src/StacklessLines.cc
    src/ObjectDestroyMessage.cc
)

INCLUDE_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/include"
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${OSGVIEWER_INCLUDE_DIR}
    ${OSGDB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(metagam3d
    ${OPENSCENEGRAPH_LIBRARIES}
    ${OSGVIEWER_LIBRARY}
    ${OSGDB_LIBRARY}
    ${Boost_LIBRARIES}
    ${Boost_python_LIBRARY}
    ${PYTHON_LIBRARIES}
)

ADD_EXECUTABLE(testVariant test/Variant.cc)
TARGET_LINK_LIBRARIES(testVariant metagam3d)

ADD_EXECUTABLE(testScriptExpression test/ScriptExpression.cc)
TARGET_LINK_LIBRARIES(testScriptExpression metagam3d)

ADD_EXECUTABLE(testDynamicValue test/DynamicValue.cc)
TARGET_LINK_LIBRARIES(testDynamicValue metagam3d)

ADD_EXECUTABLE(testScriptAssembler test/ScriptAssembler.cc)
TARGET_LINK_LIBRARIES(testScriptAssembler metagam3d)

ADD_EXECUTABLE(convertModel tools/convertModel.cc)
TARGET_LINK_LIBRARIES(convertModel
    ${OSGDB_LIBRARY}
)
