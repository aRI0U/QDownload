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
    QDownload *dl = newTask(url, file, kind, metadata);

    if (overwrite == QOverwritePolicy::Overwrite
            || (overwrite == QOverwritePolicy::Default && defaultPolicy() == QOverwritePolicy::Overwrite)
            || (!QFileInfo(file).exists())) {
        connect(dl, &QDownload::downloadProgress,
                this, &QDownloader::sendDownloadProgress);
        connect(dl, &QDownload::finished,
                this, &QDownloader::terminateDownload);

        dl->get();
    }
    else
        terminateDownload(dl);
}

void QDownloader::download(const QUrl url, const QFile &file, int kind, QHash<QString, QVariant> metadata, QOverwritePolicy overwrite) {
    download(url, file.fileName(), kind, metadata, overwrite);
}


void QDownloader::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download) {
    emit downloadProgress(bytesReceived, bytesTotal, download);
}

void QDownloader::terminateDownload(QDownload *download) {
    m_tasksList.removeOne(download);
    emit downloadTerminated(download);
}


QDownload *QDownloader::newTask(const QUrl &url, const QString &file, int kind, QHash<QString, QVariant> metadata) {
    QDownload *dl = new QDownload(url, file, kind, metadata, this);
    m_tasksList.append(dl);
    return dl;
}
