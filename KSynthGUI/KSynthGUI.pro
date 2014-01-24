#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T21:02:23
#
#-------------------------------------------------

QT       += core gui network

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
QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
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
    SampleChooserDialog.cpp \
    controls/editor/Editor.cpp \
    controls/editor/EditorTimeline.cpp \
    controls/editor/EditorSlider.cpp \
    controls/editor/EditorSheet.cpp \
    controls/editor/EditorSheetNote.cpp \
    controls/scroll/AdvScrollArea.cpp \
    controls/editor/EditorSheetHeader.cpp \
    controls/editor/EditorSheetDrawNote.cpp \
    ProgressDialog.cpp \
    controller/tasks/Task.cpp \
    controller/tasks/Tasks.cpp \
    misc/Skin.cpp \
    misc/Download.cpp \
    MidiBindingDialog.cpp \
    controls/RightClickMenu.cpp \
    controls/MidiUI.cpp \
    controls/misc/OSStats.cpp \
    SettingsDialog.cpp \
    model/SoundSinks.cpp \
    model/SystemSettings.cpp

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
    controls/editor/Editor.h \
    controls/editor/EditorTimeline.h \
    controls/editor/EditorSlider.h \
    controls/editor/EditorSheet.h \
    controls/editor/EditorSheetNote.h \
    controls/editor/EditorNote.h \
    controls/scroll/AdvScrollArea.h \
    controls/editor/EditorSheetHeader.h \
    controls/editor/Grabable.h \
    controls/editor/EditorMode.h \
    controls/editor/EditorScaler.h \
    controls/editor/EditorSheetDrawNote.h \
    ProgressDialog.h \
    model/StatusCallback.h \
    controller/tasks/Task.h \
    controller/tasks/Tasks.h \
    misc/Skin.h \
    misc/Download.h \
    MidiBindingDialog.h \
    controls/RightClickMenu.h \
    controls/misc/OSStats.h \
    ../lib/KLib/os/CPUUsage.h \
    ../lib/KLib/os/Time.h \
    ../lib/KLib/os/MemoryUsage.h \
    SettingsDialog.h \
    model/SoundSinks.h \
    ../KSynth/output/SoundSinkExport.h \
    misc/SystemVersion.h \
    model/SystemSettings.h

FORMS    += \
    SynthWin.ui \
    MainWin.ui \
    ControlPanelWidget.ui \
    SequencerWidget.ui \
    RackWidget.ui \
    SampleChooserDialog.ui \
    ProgressDialog.ui \
    MidiBindingDialog.ui \
    SettingsDialog.ui

RESOURCES += \
    icons.qrc




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
        ../lib
SOURCES += \
        ../lib/KLib/inc/7z/*.c
HEADERS += \
        ../lib/KLib/memory/* \
        ../lib/KLib/os/*

QMAKE_CFLAGS += -D_7ZIP_ST

# add KSynth
INCLUDEPATH += \
        ../
HEADERS += \
        ../KSynth/*.h \
        ../KSynth/analyzer/*.h \
        ../KSynth/fx/*.h \
        ../KSynth/huell/*.h \
        ../KSynth/midi/*.h \
        ../KSynth/midi/rt/*.h \
        ../KSynth/midi/rt/devices/*.h \
        ../KSynth/misc/*.h \
        ../KSynth/mixer/*.h \
        ../KSynth/notes/*.h \
        ../KSynth/oscillator/*.h \
        ../KSynth/output/*.h \
        ../KSynth/pattern/*.h \
        ../KSynth/sampler/*.h \
        ../KSynth/sampler/formats/*.h \
        ../KSynth/synth/*.h


# linux specific build steps
linux-g++ | linux-g++-64 | linux-g++-32 {

    # compile with alsa support?
    ALSA {
        message("using ALSA")
        QMAKE_CXXFLAGS += -D__LINUX_ALSA__ -DWITH_ALSA
        LIBS += -lasound
    }

    PULSE_AUDIO {

    }

}

# windows specific stuff
win32 {

    WAVE_OUT {
        message("using WaveOut")
        QMAKE_CXXFLAGS += -DWITH_WAVE_OUT
        LIBS += -lwinmm
    }

    # needed to get process's memory consumption
    LIBS += -lpsapi

}



# compile with FLAC support?
FLAC {
    message("using FLAC")
    QMAKE_CXXFLAGS += -DWITH_FLAC
    LIBS += -lFLAC
}

# compile with LAME (mp3) support?
LAME {
    message("using LAME")
    QMAKE_CXXFLAGS += -DWITH_LAME
    LIBS += -lmp3lame
}

FFTW3 {
    message("using FFTW3")
    QMAKE_CXXFLAGS += -DWITH_FFTW3
    LIBS += -lfftw3
}

ZLIB {
    message("using ZLIB")
    QMAKE_CXXFLAGS += -DWWITH_ZLIB
    LIBS += -lz
}








