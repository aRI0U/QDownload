#ifndef QDOWNLOADER_H
#define QDOWNLOADER_H

#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "QDownload.h"

enum QOverwritePolicy {
    Overwrite,
    NoOverwrite,
    Default
};

class QDownloader : public QObject
{
    Q_OBJECT
    typedef QList<QDownload *> QDownloadList;

public:
    explicit QDownloader(QObject *parent = nullptr);

    QOverwritePolicy defaultPolicy() const;
    void setDefaultPolicy(QOverwritePolicy policy);

public slots:
    void download(const QUrl url,
                  const QString file,
                  int kind = 0,
                  QHash<QString,QVariant> metadata = QHash<QString, QVariant>(),
                  QOverwritePolicy overwrite = Default);

    void download(const QUrl url,
                  const QFile &file,
                  int kind = 0,
                  QHash<QString,QVariant> metadata = QHash<QString, QVariant>(),
                  QOverwritePolicy = Default);

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download);
    void downloadTerminated(QDownload *download);

private slots:
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download);
    void terminateDownload(QDownload *download);

private:
    QDownload *newTask(const QUrl &url, const QString &file, int kind, QHash<QString, QVariant> metadata);

    QDownloadList m_tasksList;
    QOverwritePolicy m_defaultPolicy = QOverwritePolicy::Overwrite;
};

#endif // QDOWNLOADER_H
