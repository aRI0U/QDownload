#include "QDownloader.h"

QDownloader::QDownloader(QObject *parent) : QObject(parent)
{

}


QOverwritePolicy QDownloader::defaultPolicy() const {
    return m_defaultPolicy;
}

void QDownloader::setDefaultPolicy(QOverwritePolicy overwrite) {
    if (overwrite == QOverwritePolicy::Default)
        overwrite = QOverwritePolicy::Overwrite;

    m_defaultPolicy = overwrite;
}


void QDownloader::download(const QUrl url, const QString file, int kind, QHash<QString, QVariant> metadata, QOverwritePolicy overwrite) {
    QDownload *dl = new QDownload(url, file, kind, metadata, this);

    if (overwrite == QOverwritePolicy::Overwrite
            || (overwrite == QOverwritePolicy::Default && defaultPolicy() == QOverwritePolicy::Overwrite)
            || (!QFileInfo(file).exists())) {
        connect(dl, &QDownload::downloadProgress,
                this, &QDownloader::sendDownloadProgress);
        connect(dl, &QDownload::finished,
                this, &QDownloader::terminateDownload);

        newTask(dl);
    }
    else
        emit downloadTerminated(dl);
}

void QDownloader::download(const QUrl url, const QFile &file, int kind, QHash<QString, QVariant> metadata, QOverwritePolicy overwrite) {
    download(url, file.fileName(), kind, metadata, overwrite);
}


void QDownloader::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download) {
    emit downloadProgress(bytesReceived, bytesTotal, download);
}

void QDownloader::terminateDownload(QDownload *download) {
    emit downloadTerminated(download);
    if (--m_numberRunningTasks < maxRunningTasks && !m_queue.isEmpty())
        launchDownload(m_queue.dequeue());
}


void QDownloader::launchDownload(const QDownload *download) {
    download->get();
    ++m_numberRunningTasks;
}

void QDownloader::newTask(QDownload *download) {
    if (m_numberRunningTasks + 1 < maxRunningTasks)
        launchDownload(download);
    else
        m_queue.enqueue(download);
}
