TEMPLATE = subdirs

CONFIG += ordered

CONFIG -= debug

SUBDIRS += \
    PrimitivesLoader \
    Graph \
    Compiler \
    Designer \
    Tests

unix {
    debug {
       copymanual.commands = $(COPY) $$PWD/Designer/Manual.pdf $$OUT_PWD/Designer/
       first.depends = $(first) copymanual
       export(first.depends)
       export(copymanual)
       QMAKE_EXTRA_TARGETS += first copymanual
    }

    release {
        manual.path = /usr/share/$${TARGET}
        manual.files += $$PWD/Designer/Manual.pdf
        INSTALLS += manual
    }
}
win32 {
       copymanual.commands = $(COPY) $$PWD\\Designer\\Manual.pdf $$OUT_PWD\\Designer\\
       first.depends = $(first) copymanual
       export(first.depends)
       export(copymanual)
       QMAKE_EXTRA_TARGETS += first copymanual
}
