# -------------------------------------------------
# Project created by QtCreator 2010-03-06T19:15:17
# -------------------------------------------------
QT += sql \
    testlib
TARGET = QST
TEMPLATE = app
SOURCES += main.cpp \
    qst/qstvalue.cpp \
    qst/qstabstractmodelhandler.cpp \
    qst/qstglobal.cpp \
    qst/qstbatch.cpp \
    qst/qstfield.cpp \
    qst/qstquerygenerator.cpp \
    qst/qstquerycomposer.cpp \
    qst/qstquerydescriptor.cpp \
    qst/qstmodeldescriptor.cpp \
    qst/qstplainquerymodel.cpp \
    qst/qsttreequerymodel.cpp \
    qst/qsttreeitem.cpp \
    qst/qstdbconnection.cpp \
    qst/qsttextcodec.cpp \
    qst/qstabstractitemnameextractor.cpp \
    qst/qstdefaultitemnameextractor.cpp \
    qst/qstrolevisibility.cpp \
    test/ut_qstvalue.cpp \
    test/ut_qstquerygenerator.cpp \
    test/ut_qstfield.cpp \
    test/ut_qstdefaultitemnameextractor.cpp \
    test/ut_qstbatch.cpp \
    test/ut_qstabstractmodelhandler.cpp \
    test/testhandler.cpp \
    mainwindow.cpp \
    qst/qstif.cpp \
    qst/qstconnectionsettings.cpp \
    test/ut_qstconnectionsettings.cpp
HEADERS += qst/qstvalue.h \
    qst/qstabstractmodelhandler.h \
    qst/qstglobal.h \
    qst/qstbatch.h \
    qst/qstfield.h \
    qst/qstquerygenerator.h \
    qst/qstquerycomposer.h \
    qst/qstquerydescriptor.h \
    qst/qstmodeldescriptor.h \
    qst/qstplainquerymodel.h \
    qst/qsttreequerymodel.h \
    qst/qsttreeitem.h \
    qst/qstdbconnection.h \
    qst/qstgenconstants.h \
    qst/qsttextcodec.h \
    qst/qstabstractitemnameextractor.h \
    qst/qstdefaultitemnameextractor.h \
    qst/qstrolevisibility.h \
    test/ut_qstvalue.h \
    test/ut_qstquerygenerator.h \
    test/ut_qstfield.h \
    test/ut_qstdefaultitemnameextractor.h \
    test/ut_qstbatch.h \
    test/ut_qstabstractmodelhandler.h \
    test/testhandler.h \
    mainwindow.h \
    qst/qstif.h \
    qst/qstconnectionsettings.h \
    test/ut_qstconnectionsettings.h
FORMS += mainwindow.ui
