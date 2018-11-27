QT += quick
QT += sql
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/editor.cpp \
    src/datasethandler.cpp \
    src/iterableqqmlpropertymap.cpp \
    src/apihandler.cpp \
    src/folderprovider.cpp \
    src/aprovider.cpp \
    src/datasetmodel.cpp \
    src/project.cpp \
    src/databasehandler.cpp \
    src/parsefoldertask.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += include/
INCLUDEPATH += $${PWD}/../libs/TaskManager/

HEADERS += \
    include/editor.h \
    include/datasethandler.h \
    include/iterableqqmlpropertymap.h \
    include/aprovider.h \
    include/apihandler.h \
    include/folderprovider.h \
    include/datasetmodel.h \
    include/project.h \
    include/databasehandler.h \
    include/ifactory.h \
    include/recordfactory.h \
    include/parsefoldertask.h

DISTFILES += \
    dark.json

LIBS += -L$${PWD}/../libs/ -ltaskmanager
