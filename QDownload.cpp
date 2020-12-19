﻿#include "QDownload.h"


QDownload::QDownload(const QUrl &url, const QString &file, int kind, QHash<QString, QVariant> metadata, QObject *parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this)),
      m_error("No error"),
      m_targetUrl(url),
      m_kind(kind),
      m_metadata(metadata)
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &QDownload::finishDownload);
    setTargetFile(file);
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

QVariant QDownload::metadata(const QString &key) const {
    return m_metadata.value(key);
}

QVariant QDownload::metadata(const QString &key, const QVariant &defaultValue) const {
    return m_metadata.value(key, defaultValue);
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

void QDownload::setMetadata(const QHash<QString, QVariant> metadata) {
    m_metadata = metadata;
}


void QDownload::get() const {
    QNetworkRequest request(m_targetUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla Firefox");
    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::downloadProgress,
            this, &QDownload::sendDownloadProgress);
}


void QDownload::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit downloadProgress(bytesReceived, bytesTotal, this);
}

void QDownload::finishDownload(QNetworkReply *reply) {
    // propagate errors if any
    if (reply->error() != QNetworkReply::NoError)
        m_error = tr("Connection error: ") + reply->errorString();

//    else if (!m_targetFile.setPermissions(QFileDevice::ReadOwner
//                                     | QFileDevice::WriteOwner
//                                     | QFileDevice::ReadUser
//                                     | QFileDevice::WriteUser
//                                     | QFileDevice::ReadGroup
//                                     | QFileDevice::WriteGroup
//                                     | QFileDevice::ReadOther
//                                     | QFileDevice::WriteOther))
//        m_error = m_targetFile.errorString();

    else if (!m_targetFile.open(QIODevice::WriteOnly))
        m_error = tr("I/O error: ") + m_targetFile.errorString();

    else {
        m_error = "No error";
        writeDownloadedData(reply->readAll());
    }

    qDebug() << targetFile() << m_error;

    reply->deleteLater();

    emit finished(this);
}


void QDownload::writeDownloadedData(const QByteArray data) {
    m_targetFile.write(data);
    m_targetFile.close();
}
