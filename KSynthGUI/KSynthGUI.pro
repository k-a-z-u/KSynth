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
QMAKE_CXXFLAGS += -Wconversion -Werror=return-type -Wreorder -Werror=reorder
QMAKE_CXXFLAGS += -Werror=uninitialized -Wno-sign-conversion
#QMAKE_CXXFLAGS += -Wno-c99-extensions
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
    model/SystemSettings.cpp \
    controller/SongExport.cpp \
    controls/editor/EditorHelper.cpp \
    controls/editor/EditorSheetNoteModel.cpp \
    controls/Image.cpp \
    controls/NumberSpin.cpp \
    rack/SamplePad1.cpp \
    rack/Reverb1LCD.cpp \
    controls/LCD.cpp \
    rack/Phaser1.cpp

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
    model/SystemSettings.h \
    controller/SongExport.h \
    ../KSynth/output/SoundSinkHardware.h \
    controls/editor/EditorSheetNoteModel.h \
    controls/editor/EditorHelper.h \
    controls/editor/SelectionRect.h \
    misc/PixelFont.h \
    controls/Image.h \
    controls/NumberSpin.h \
    ../KSynth/tts/TTSProvider.h \
    rack/SamplePad1.h \
    ../KSynth/sampler/SimpleSamplePad.h \
    ../KSynth/fx/DelayBufferFixed.h \
    ../KSynth/fx/AllPassReverb.h \
    rack/Reverb1LCD.h \
    controls/LCD.h \
    rack/Phaser1.h

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
QMAKE_CXXFLAGS += -DWITH_7Z

# add KSynth
INCLUDEPATH += \
        ../

HEADERS += \
		$$files(../KSynth/*.h) \
		$$files(../KSynth/analyzer/*.h) \
		$$files(../KSynth/fx/*.h) \
		$$files(../KSynth/huell/*.h) \
		$$files(../KSynth/midi/*.h) \
		$$files(../KSynth/midi/rt/*.h) \
		$$files(../KSynth/midi/rt/devices/*.h) \
		$$files(../KSynth/misc/*.h) \
		$$files(../KSynth/mixer/*.h) \
		$$files(../KSynth/notes/*.h) \
		$$files(../KSynth/oscillator/*.h) \
		$$files(../KSynth/output/*.h) \
		$$files(../KSynth/pattern/*.h) \
		$$files(../KSynth/sampler/*.h) \
		$$files(../KSynth/sampler/formats/*.h) \
		$$files(../KSynth/synth/*.h) \
		$$files(../KSynth/tts/*.h)


# linux specific build steps
linux-g++ | linux-g++-64 | linux-g++-32 | unix {

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

# compile libfftw3 support (e.g. for spectrum analysis)
FFTW3 {
    message("using FFTW3")
    QMAKE_CXXFLAGS += -DWITH_FFTW3
    LIBS += -lfftw3
}

# compile support for ZLib (e.g. for loading/saving compressed workspaces)
ZLIB {
    message("using ZLIB")
    QMAKE_CXXFLAGS += -DWITH_ZLIB
    LIBS += -lz
}

# compile support for FLite TTS engine?
TTS_FLITE {
    message("using TTS FLITE")
    QMAKE_CXXFLAGS += -DWITH_TTS_FLITE
    LIBS += -lflite_cmu_us_kal16 -lflite_usenglish -lflite_cmulex -lflite
}

# compile support for ESpeak TTS engine?
TTS_ESPEAK {
     message("using TTS ESPEAK")
    QMAKE_CXXFLAGS += -DWITH_TTS_ESPEAK
    LIBS += -lespeak
}
