#ifndef QDOWNLOAD_H
#define QDOWNLOAD_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class QDownload : public QObject
{
    Q_OBJECT

public:
    explicit QDownload(const QUrl &url, const QString &file, int kind = 0, QHash<QString, QVariant> metadata = QHash<QString, QVariant>(), QObject *parent = nullptr);


    QUrl targetUrl() const;
    QString targetFile() const;
    int kind() const;
    QVariant metadata(const QString &key) const;
    QVariant metadata(const QString &key, const QVariant &defaultValue) const;

    QString error() const;
    bool success() const;

    void setTargetFile(const QString &file);

    void get() const;

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal, QDownload *download);
    void finished(QDownload *download);

private slots:
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finishDownload(QNetworkReply *reply);

private:
    void writeDownloadedData(const QByteArray data);

    QNetworkAccessManager *m_manager;
    QString m_error;

    QFile m_targetFile;
    QUrl m_targetUrl;
    int m_kind;
    QHash<QString,QVariant> m_metadata;

};

#endif // QDOWNLOAD_H
