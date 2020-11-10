#include "QDownload.h"

QDownload::QDownload(QObject *parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &QDownload::finishDownload);
}

QDownload::QDownload(const QUrl &url, const QString &file, int kind, QObject *parent)
    : QDownload(parent)
{
    setTargetUrl(url);
    setTargetFile(file);
    setKind(kind);
}


QUrl QDownload::targetUrl() const {
    return m_targetUrl;
}

QString QDownload::targetFile() const {
    return m_targetFile.fileName();
}

int QDownload::kind() const {
    return m_kind;
}

QString QDownload::error() const {
    return m_error;
}

bool QDownload::success() const {
    return (m_error == "No error");
}

void QDownload::setTargetUrl(const QUrl &url) {
    m_targetUrl = url;
}

void QDownload::setTargetFile(const QString &file) {
    m_targetFile.setFileName(file);
}

void QDownload::setKind(const int kind) {
    m_kind = kind;
}


void QDownload::get() const {
    QNetworkReply *reply = m_manager->get(QNetworkRequest(m_targetUrl));

    connect(reply, &QNetworkReply::downloadProgress,
            this, &QDownload::sendDownloadProgress);
}


void QDownload::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit downloadProgress(bytesReceived, bytesTotal, this);
}

void QDownload::finishDownload(QNetworkReply *reply) {
    // propagate errors if any
    if (reply->error() != QNetworkReply::NoError)
        m_error = reply->errorString();
    else if (!m_targetFile.open(QIODevice::WriteOnly))
        m_error = m_targetFile.errorString();

    else {
        m_error = "No error";
        writeDownloadedData(reply->readAll());
    }

    reply->deleteLater();

    emit finished(this);
}


void QDownload::writeDownloadedData(const QByteArray data) {
    m_targetFile.write(data);
    m_targetFile.close();
}
