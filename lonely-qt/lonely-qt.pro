#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T16:34:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=gnu++11

TARGET = lonely
TEMPLATE = app


SOURCES += main.cpp\
        lonelymainwindow.cpp \
    qtformatconversion.cpp \
    lonelywidgets.cpp \
    romselectdialog.cpp \
    numberedqpushbutton.cpp \
    numberedqaction.cpp \
    leveleditorwidget.cpp \
    editorwidget.cpp \
    layouteditorwidget.cpp \
    addremovelistwidget.cpp \
    levelviewwidget.cpp \
    lonelyglobaldata.cpp \
    lonelyglobals.cpp \
    cachedqimage.cpp \
    createinputeventdata.cpp \
    inputqlabel.cpp \
    spawnpointedwidget.cpp \
    spawnbossedwidget.cpp \
    setscrollingedwidget.cpp \
    blankedwidget.cpp \
    setareaedwidget.cpp \
    setmapedwidget.cpp \
    setelevatorsedwidget.cpp \
    jumptopositionedwidget.cpp \
    nodeeditorstackedwidget.cpp \
    addnodeentrydialog.cpp \
    patterneditorwidget.cpp \
    patternlistwidget.cpp \
    zoomcombobox.cpp \
    standardzoomcombobox.cpp \
    customscrollwidget.cpp \
    areatypecombobox.cpp \
    objectpatternlistwidget.cpp \
    objecteditorwidget.cpp \
    objecteditstackedwidget.cpp \
    objecteditorsubwidget.cpp \
    standardspawneditwidget.cpp \
    delayeditwidget.cpp \
    addobjectpatterndialog.cpp \
    metatileeditorwidget.cpp \
    palettecombobox.cpp \
    metatilebehaviorbox.cpp \
    metatileeditstackedwidget.cpp \
    metatileelevatoreditwidget.cpp \
    metatileeditwidgetbase.cpp \
    metatiledamagingeditwidget.cpp \
    paletteeditorwidget.cpp \
    aboutdialog.cpp \
    vulnerabilityeditorwidget.cpp \
    resistanceeditorwidget.cpp \
    hpeditorwidget.cpp \
    errordialog.cpp

HEADERS  += lonelymainwindow.h \
    qtformatconversion.h \
    lonelywidgets.h \
    romselectdialog.h \
    numberedqpushbutton.h \
    numberedqaction.h \
    leveleditorwidget.h \
    editorwidget.h \
    layouteditorwidget.h \
    addremovelistwidget.h \
    levelviewwidget.h \
    lonelyglobaldata.h \
    lonelyglobals.h \
    cachedqimage.h \
    createinputeventdata.h \
    inputqlabel.h \
    spawnpointedwidget.h \
    spawnbossedwidget.h \
    setscrollingedwidget.h \
    blankedwidget.h \
    setareaedwidget.h \
    setmapedwidget.h \
    setelevatorsedwidget.h \
    jumptopositionedwidget.h \
    nodeeditorstackedwidget.h \
    addnodeentrydialog.h \
    patterneditorwidget.h \
    patternlistwidget.h \
    zoomcombobox.h \
    standardzoomcombobox.h \
    customscrollwidget.h \
    areatypecombobox.h \
    objectpatternlistwidget.h \
    objecteditorwidget.h \
    objecteditstackedwidget.h \
    objecteditorsubwidget.h \
    standardspawneditwidget.h \
    delayeditwidget.h \
    addobjectpatterndialog.h \
    metatileeditorwidget.h \
    palettecombobox.h \
    metatilebehaviorbox.h \
    metatileeditstackedwidget.h \
    metatileelevatoreditwidget.h \
    metatileeditwidgetbase.h \
    metatiledamagingeditwidget.h \
    paletteeditorwidget.h \
    aboutdialog.h \
    vulnerabilityeditorwidget.h \
    resistanceeditorwidget.h \
    hpeditorwidget.h \
    errordialog.h

FORMS    += lonelymainwindow.ui \
    romselectdialog.ui \
    leveleditorwidget.ui \
    layouteditorwidget.ui \
    addremovelistwidget.ui \
    levelviewwidget.ui \
    spawnpointedwidget.ui \
    spawnbossedwidget.ui \
    setscrollingedwidget.ui \
    blankedwidget.ui \
    setareaedwidget.ui \
    setmapedwidget.ui \
    setelevatorsedwidget.ui \
    jumptopositionedwidget.ui \
    addnodeentrydialog.ui \
    patterneditorwidget.ui \
    customscrollwidget.ui \
    objecteditorwidget.ui \
    standardspawneditwidget.ui \
    delayeditwidget.ui \
    addobjectpatterndialog.ui \
    metatileeditorwidget.ui \
    metatileelevatoreditwidget.ui \
    metatiledamagingeditwidget.ui \
    paletteeditorwidget.ui \
    aboutdialog.ui \
    vulnerabilityeditorwidget.ui \
    resistanceeditorwidget.ui \
    hpeditorwidget.ui \
    errordialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../liblonely/release/ -lliblonely
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../liblonely/debug/ -lliblonely
else:symbian: LIBS += -llonely
else:unix: LIBS += -L$$PWD/../liblonely/ -llonely

INCLUDEPATH += $$PWD/../liblonely/include
DEPENDPATH += $$PWD/../liblonely/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../liblonely/release/liblonely.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../liblonely/debug/liblonely.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../liblonely/liblonely.a

RESOURCES += \
    resources.qrc
