#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QUrl>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QObject>
#include <exception>

/** exception handling */
class DownloadException : public std::exception {
public:
	DownloadException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() {return msg.c_str();}
private:
	std::string msg;
};



class Download : public QObject {
	Q_OBJECT

public:

	/** ctor */
	Download(const std::string& url, QFile& file);

	/** start download (blocks until done) */
	void start();

private slots:

	/** redirect progress */
	void progress(qint64 done, qint64 total);


signals:

	/** download's progress changed */
	void onProgress(float percent);


private:

	QFile& file;
	QUrl url;

};

#endif // DOWNLOAD_H
