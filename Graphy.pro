QT += quick quickcontrols2
CONFIG += c++14 qt qmltypes warn_on
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += include/
HEADERS = include/adjacencymatrix.h \
    include/adjacencylistq.h \
    include/vertexlistq.h
SOURCES = src/adjacencymatrix.cpp src/main.cpp \
    src/adjacencylistq.cpp \
    src/vertexlistq.cpp
RESOURCES = qml/qml.qrc
win32:RC_ICONS += bitmap.ico

QML_IMPORT_PATH += "/"
QML_IMPORT_NAME = custom
QML_IMPORT_MAJOR_VERSION = 1
