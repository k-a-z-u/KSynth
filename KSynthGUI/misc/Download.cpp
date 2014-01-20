#include "Download.h"

Download::Download(const std::string& url, QFile& file) : file(file) {
	this->url = QUrl::fromUserInput(url.c_str());
}

void Download::start() {

	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkReply* reply = manager->get( QNetworkRequest(url) );
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progress(qint64,qint64)));
	loop.exec();

	// error handling
	if (reply->error() != QNetworkReply::NoError) {
		throw DownloadException(reply->errorString().toUtf8().constData());
	}

	file.open(QIODevice::WriteOnly);
	file.write(reply->readAll());

	delete reply;

}

void Download::progress(qint64 done, qint64 total) {
	float percent = float(done) / float(total);
	emit onProgress(percent);
}
