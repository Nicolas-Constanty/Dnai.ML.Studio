#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QMap>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using JsonRequest = QPair<QNetworkRequest, QByteArray>;
class ApiHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString addr READ addr WRITE setAddr NOTIFY addrChanged)
public:
    enum ROUTES {
        AUTH
    };

    static QMap<ApiHandler::ROUTES, QString> routes;

    ApiHandler(QObject *parent = nullptr);
    Q_INVOKABLE void connect(const QString &password);

    QString addr() const;

public slots:
    void setAddr(const QString &addr);
    void replyFinished(QNetworkReply *reply);

    void authReadyRead();

    void slotError(const QNetworkReply::NetworkError &);
    void slotSslErrors(const QList<QSslError> &);
signals:
    void addrChanged(const QString &addr);

private:
    QString m_addr;
    QString m_sessionToken;
    QNetworkAccessManager *m_manager;
    JsonRequest jsonRequest(const QJsonObject &json, const QUrl &url);
    QJsonObject byteArrayToJson(const QByteArray &b);
};

using RouteMap = QMap<ApiHandler::ROUTES, QString>;


#endif // APIHANDLER_H
