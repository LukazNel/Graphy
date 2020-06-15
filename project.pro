QT += quick
CONFIG += c++14 qt qmltypes qml_debug warn_on debug
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += include/
HEADERS = include/adjacencymatrix.h \
    include/adjacencylistq.h \
    include/vertexlistq.h
SOURCES = src/adjacencymatrix.cpp src/main.cpp \
    src/adjacencylistq.cpp \
    src/vertexlistq.cpp
RESOURCES = qml/qml.qrc

QML_IMPORT_PATH += "/"
QML_IMPORT_NAME = custom
QML_IMPORT_MAJOR_VERSION = 1
