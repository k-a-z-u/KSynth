#include "Skin.h"

#include <KLib/fs/File.h>

#include <QMessageBox>
#include "../model/Context.h"
#include "../controller/tasks/Tasks.h"
#include "../MainWin.h"

#define SKIN_URL "https://dl.dropboxusercontent.com/s/zmptnosi6147r4g/skin.7z?dl=1&token_hash=AAHe3ARYTzDq0cbatB_saEL2OzQ6V5-L08v4XIE8QaiHtA"

void Skin::checkPresent(Context& ctx, MainWin& mw) {

	// skin must reside here
	K::File f("skin");
	if (f.isFolder()) {return;}

	// dl message string
	std::string msg =
			"It looks like there is currently no skin installed.<br/>"
			"The skin is expected to reside in this folder:<br/>"+
			f.getAbsolutePath()+"<br/>"+
			"Currently, the latest skin can be downloaded here:<br/>"+
			"<a href='"+SKIN_URL+"'>link</a><br/><br/>"+
			"Do you want to download and extract this file automatically?"
			;

	// show dl question
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(&mw, "skin missing", msg.c_str(), QMessageBox::Yes|QMessageBox::No);
	if (reply == QMessageBox::Yes) {download(ctx, mw);}

}

QImage Skin::getImage(const std::string& file, const std::string& type) {
	K::File f(file);
	return QImage(f.getAbsolutePath().c_str(), type.c_str());
}

QIcon Skin::getIcon(const std::string& file, const std::string& type) {
	QImage img = getImage(file, type);
	return QIcon( QPixmap::fromImage(img) );
}

QCursor Skin::getCursor(const std::string& file, const std::string& type, int hotX, int hotY) {
	QImage img = getImage(file, type);
	return QCursor( QPixmap::fromImage(img), hotX, hotY );
}





#include "Download.h"
#include <iostream>
#include <KLib/archive/Archive7z.h>
#include <QMessageBox>
void Skin::download(Context& ctx, MainWin& mw) {

	class DLTask : public Task {
	public:

		MainWin& mw;

		DLTask(MainWin& mw) : Task("downloading skin", true), mw(mw) {;}

		void exec() {

			try {

				// download to temporal file
				QFile f("skin.7z");
				Download dl(SKIN_URL, f);
				setProgress("downloading skin", 0);
				connect (&dl, SIGNAL(onProgress(float)), this, SLOT(setProgress(float)));
				dl.start();

				auto callback = [this] (const std::string& file, float percent) {setProgress(file, percent);};

				// extract
				setProgress("extracting skin ...", 0);
				K::File folder("skin"); folder.mkdir();
				K::File file(f.fileName().toUtf8().constData());
				K::Archive7z archive(file);
				archive.extractTo(folder, callback);

				// remove temporal file
				file.deleteMe();

			} catch (std::exception& e) {
				QMessageBox::information(&mw, "error", e.what());
			}

		}
	};

	ctx.getTasks()->addTaskForeground( new DLTask(mw) );

}
