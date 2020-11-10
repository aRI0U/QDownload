#include "QDownloader.h"

QDownloader::QDownloader(QObject *parent) : QObject(parent)
{

}


void QDownloader::download(const QUrl url, const QString file, int kind) {
    QDownload *dl = newTask(url, file, kind);

    connect(dl, &QDownload::downloadProgress,
            this, &QDownloader::sendDownloadProgress);
    connect(dl, &QDownload::finished,
            this, &QDownloader::terminateDownload);

    dl->get();
}

void QDownloader::download(const QUrl url, const QFile &file, int kind) {
    download(url, file.fileName(), kind);
}


void QDownloader::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download) {
    emit downloadProgress(bytesReceived, bytesTotal, download);
}

void QDownloader::terminateDownload(QDownload *download) {
    m_tasksList.removeOne(download);
    emit downloadTerminated(download);
}


QDownload *QDownloader::newTask(const QUrl &url, const QString &file, int kind) {
    QDownload *dl = new QDownload(url, file, kind, this);
    m_tasksList.append(dl);
    return dl;
}
