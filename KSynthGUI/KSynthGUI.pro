#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T21:02:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KSynthGUI
TEMPLATE = app

# compiler settings
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3 -march=native

QMAKE_CXXFLAGS += -std=c++11


#QMAKE_CXXFLAGS += -Weffc++
QMAKE_CXXFLAGS += -Wconversion -Werror=return-type -Werror=maybe-uninitialized -Wreorder -Werror=reorder
QMAKE_CXXFLAGS += -Werror=uninitialized
QMAKE_CXXFLAGS += -isystem "/usr/include/qt4"


SOURCES += main.cpp\
    controls/Knob.cpp \
    controls/Slider1.cpp \
    misc/Helper.cpp \
    controls/CheckBox.cpp \
    controls/CheckBoxGroup.cpp \
    controls/ImgSpin.cpp \
    SynthWin.cpp \
    controls/VUMeter.cpp \
    controller/Controller.cpp \
    rack/RackElement.cpp \
    rack/Synth1.cpp \
    rack/Sequencer1Selector.cpp \
    rack/Sequencer1.cpp \
    rack/Reverb1.cpp \
    rack/Mixer1.cpp \
    controls/LED1.cpp \
    rack/DrumComputer1.cpp \
    controller/CtrlHelper.cpp \
    controls/ColorButton.cpp \
    MainWin.cpp \
    ControlPanelWidget.cpp \
    SequencerWidget.cpp \
    RackWidget.cpp \
    controls/SequencerTrackItem.cpp \
    controls/SequencerTracksWidget.cpp \
    rack/RackFactory.cpp \
    model/Context.cpp \
    rack/MasterTarget1.cpp \
    model/RackElementModel.cpp \
    rack/Rack.cpp \
    controls/PinConnector.cpp \
    model/WorkspaceLoader.cpp \
    model/WorkspaceSaver.cpp \
    controls/TextLabel.cpp \
    SampleChooserDialog.cpp

HEADERS  += \
    controls/Knob.h \
    controls/Slider1.h \
    model/Context.h \
    misc/Helper.h \
    controls/CheckBox.h \
    controls/CheckBoxGroup.h \
    controls/ImgSpin.h \
    SynthWin.h \
    controls/MidiUI.h \
    controls/VUMeter.h \
    controller/Controller.h \
    rack/RackElement.h \
    rack/Synth1.h \
    rack/Reverb1.h \
    rack/Sequencer1.h \
    rack/Sequencer1Selector.h \
    rack/Mixer1.h \
    controls/LED1.h \
    rack/DrumComputer1.h \
    controller/CtrlHelper.h \
    controls/ColorButton.h \
    MainWin.h \
    ControlPanelWidget.h \
    SequencerWidget.h \
    RackWidget.h \
    model/WorkspaceLoader.h \
    model/WorkspaceSaver.h \
    controls/TextLabel.h \
    controls/Snapper.h \
    controls/SequencerTrackItem.h \
    controls/SequencerTracksWidget.h \
    rack/RackFactory.h \
    rack/MasterTarget1.h \
    model/RackElementModel.h \
    rack/Rack.h \
    controls/PinConnector.h \
    SampleChooserDialog.h \

FORMS    += \
    SynthWin.ui \
    MainWin.ui \
    ControlPanelWidget.ui \
    SequencerWidget.ui \
    RackWidget.ui \
    SampleChooserDialog.ui



# add tiny xml
HEADERS += \
    ../lib/tinyxml2/tinyxml2.h
SOURCES += \
    ../lib/tinyxml2/tinyxml2.cpp

# add rt-midi
HEADERS += \
    ../lib/rtmidi/RtError.h \
    ../lib/rtmidi/RtMidi.h
SOURCES += \
    ../lib/rtmidi/RtMidi.cpp

# add KLib
INCLUDEPATH += \
        ../lib/KLib

# add KSynth
INCLUDEPATH += \
        ../




# OS specific stuff
linux-g++ | linux-g++-64 | linux-g++-32 {
	QMAKE_CXXFLAGS += -D__LINUX_ALSA__ -DWITH_ALSA -DWITH_LAME -DWITH_FLAC
	LIBS    += \
		-lFLAC \
		-lasound \
		-lfftw3 \
		-lmp3lame
}

win32 {
	QMAKE_CXXFLAGS += -DWITH_WAVE_OUT
	LIBS    += \
		-lwinmm
}




RESOURCES += \
    icons.qrc
