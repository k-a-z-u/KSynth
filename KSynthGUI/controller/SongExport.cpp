#include "SongExport.h"
#include "../model/Context.h"
#include "CtrlHelper.h"
#include "model/SoundSinks.h"

#include "controller/tasks/Tasks.h"
#include "controller/Controller.h"

#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>
#include <KSynth/output/SoundSinkExport.h>

#include <QSemaphore>

SongExport::SongExport(Context& ctx) : ctx(ctx) {
	;
}



class ExpTask : public Task, SequencerBeatListener {
public:

	Context& ctx;
	SoundSinkExport* sse;
	Generator& gen;
	const K::File file;
	unsigned int songLength;

	/** ctor */
	ExpTask(Context& ctx, SoundSinkExport* sse, const K::File& file) :
		Task("exporting song", true), ctx(ctx), sse(sse),
		gen(*ctx.getGenerator()), file(file) {

		songLength = ctx.getSequencer()->getSongLength();

	}

	/** run */
	virtual void exec() override {

		// setup a new generator
		// (the other one is completely attached to the gui -> unnecessary)
		//Generator gen(ctx.getGenerator()->getAudioFormat());
		//gen.setSink(sse);
		//gen.setSequencer(ctx.getSequencer());
		//gen.setSource(ctx.getGenerator()->getSource());


		// get current sink to reset everything later
		SoundSink* ssOld = gen.getSink();
		gen.setSink(sse);

		// register for events
		ctx.getSequencer()->addBeatListener(this);

		// run
		try {
			gen.blockingRender();
		} catch(std::exception& e) {
			//			ctx.getSequencer()->removeBeatListener(this);
			//			gen.setSink(ssOld);
			setError(e.what());
		}

		// reset
		ctx.getSequencer()->removeBeatListener(this);
		gen.setSink(ssOld);

	}

	void onBeat(Beat b, Time t) override {
		(void) t;
		if (b % 100 != 0) {return;}
		char buf[128];
		sprintf(buf, "%.2f", float(file.length()) / 1024.0f / 1024.0f);
		std::string txt;
		txt += "Beat: " + std::to_string(b);
		//txt += " Time: " +
		txt += " File: " + std::string(buf) + " MB";
		float progress = float(b) / float(songLength);
		setProgress(txt, progress);
	}

};



void SongExport::showExportDialog() {

	// list of available exports
	std::vector<SoundSinkExport*> exports = SoundSinks::get().getExportSinks();

	// construct save-dialog's filter list
	std::vector<FileDialogFilter> filter;
	for (SoundSinkExport* sse : exports) {
		filter.push_back( FileDialogFilter(sse->getName(), sse->getFileExtension(), (void*) sse ));
	}

	// show export dialog
	FileDialogFilter selFilter;
	std::string file = CtrlHelper::saveFile("choose export file and format", filter, &selFilter);

	// cancel?
	if (file.empty()) {return;}

	// stop (if running)
	ctx.getGenerator()->stop();

	// start at the beginning of the song
	ctx.getSequencer()->jumpTo(0);

	// TODO
	// support selectable start/end markers?

	// start export
	K::File expFile(file);
	SoundSinkExport* sse = (SoundSinkExport*) selFilter.ptr;
	sse->setFile( expFile );
	ctx.getTasks()->addTaskBackground( new ExpTask(ctx, sse, expFile) );

}



