#include <QJsonObject>
#include <QJsonDocument>

#include "apihandler.h"

RouteMap ApiHandler::routes = {
    { ApiHandler::AUTH, "/auth"}
};

ApiHandler::ApiHandler(QObject *parent) : QObject (parent)
{
    m_manager = new QNetworkAccessManager(this);
//    QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replyFinished(QNetworkReply*)));
}

QString ApiHandler::addr() const
{
    return m_addr;
}

void ApiHandler::setAddr(const QString &addr)
{
    if (m_addr == addr)
        return;

    m_addr = addr;
    emit addrChanged(m_addr);
}

void ApiHandler::replyFinished(QNetworkReply *)
{

}

QJsonObject ApiHandler::byteArrayToJson(const QByteArray &b)
{
    QString strReply = static_cast<QString>(b);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    qDebug() << "==App.ApiHandler== " << jsonObject;
    return jsonObject;
}

void ApiHandler::authReadyRead()
{
    auto obj = QObject::sender();
    auto reply = dynamic_cast<QNetworkReply*>(obj);
    if (reply == nullptr || reply->error() != QNetworkReply::NetworkError::NoError) return;
    auto r = reply->readAll();
    auto json = byteArrayToJson(r);
    m_sessionToken = json["auth"].toString();
    reply->deleteLater();
}

void ApiHandler::slotError(const QNetworkReply::NetworkError &e)
{
    qDebug() << "==App.ApiHandler== " << "Error :" << e;
    auto obj = QObject::sender();
    auto reply = dynamic_cast<QNetworkReply*>(obj);
    if (reply == nullptr) return;
    auto r = reply->readAll();
    auto json = byteArrayToJson(r);
    qDebug() << "==App.ApiHandler== " << json["error"].toString();
}

void ApiHandler::slotSslErrors(const QList<QSslError> &)
{

}

JsonRequest ApiHandler::jsonRequest(const QJsonObject &json, const QUrl &url)
{
    QJsonDocument doc(json);
    // Build your JSON string as usual
    QByteArray jsonString = doc.toJson(QJsonDocument::Compact);

    // For your "Content-Length" header
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", "ML Studio v0.1");
    request.setRawHeader("X-Custom-User-Agent", "ML Studio v0.1");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);
    JsonRequest r;
    r.first = request;
    r.second = jsonString;
    return r;
}

void ApiHandler::connect(const QString &password)
{
    QJsonObject o;
    o["password"] = password;

    auto request = jsonRequest(o, QUrl("http://" + m_addr + routes[AUTH]));

    qDebug() << "==App.ApiHandler== " << "Connect to :" << request.first.url() << "with" << request.second;
    QNetworkReply *reply = m_manager->post(request.first, request.second);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(authReadyRead()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
}
