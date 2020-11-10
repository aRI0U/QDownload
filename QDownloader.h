#ifndef QDOWNLOADER_H
#define QDOWNLOADER_H

#include <QFile>
#include <QUrl>

#include "QDownload.h"

class QDownloader : public QObject
{
    Q_OBJECT
    typedef QList<QDownload *> QDownloadList;

public:
    explicit QDownloader(QObject *parent = nullptr);

public slots:
    void download(const QUrl url, const QString file, int kind = 0);
    void download(const QUrl url, const QFile &file, int kind = 0);

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download);
    void downloadTerminated(QDownload *download);

private slots:
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download);
    void terminateDownload(QDownload *download);

private:
    QDownload *newTask(const QUrl &url, const QString &file, int kind);

    QDownloadList m_tasksList;
};

#endif // QDOWNLOADER_H
