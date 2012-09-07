# -------------------------------------------------
# Project created by QtCreator 2010-03-06T19:15:17
# -------------------------------------------------
QT += sql \
    testlib
TARGET = QST
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    qst/treeitem.cpp \
    qst/sqlvalue.cpp \
    qst/sqltreemodel.cpp \
    qst/sqlquerymodel.cpp \
    qst/sqlquerydescriptor.cpp \
    qst/sqlquerycomposer.cpp \
    qst/sqlmodeldescriptor.cpp \
    qst/sqlglobal.cpp \
    qst/sqlgen.cpp \
    qst/sqlfield.cpp \
    qst/sqlbatch.cpp \
    qst/dbconnection.cpp \
    qst/cyrillic.cpp \
    qst/abstractmodelhandler.cpp \
    test/ut_sqlvalue.cpp \
    test/ut_sqlgen.cpp \
    test/ut_sqlfield.cpp \
    test/ut_sqlbatch.cpp
HEADERS += mainwindow.h \
    qst/treeitem.h \
    qst/sqlvalue.h \
    qst/sqltreemodel.h \
    qst/sqlquerymodel.h \
    qst/sqlquerydescriptor.h \
    qst/sqlquerycomposer.h \
    qst/sqlmodeldescriptor.h \
    qst/sqlglobal.h \
    qst/sqlgenconstants.h \
    qst/sqlgen.h \
    qst/sqlfield.h \
    qst/sqlbatch.h \
    qst/dbconnection.h \
    qst/cyrillic.h \
    qst/abstractmodelhandler.h \
    test/ut_sqlvalue.h \
    test/ut_sqlgen.h \
    test/ut_sqlfield.h \
    test/ut_sqlbatch.h \
    test/ui_mainwindow.h
FORMS += mainwindow.ui
