#pragma once
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

class JsonFetcher
{
public:
  static QJsonObject getJsonFromURL(const QString &url)
  {
    const auto qurl = QUrl(url);
    QNetworkAccessManager manager;
    const QNetworkRequest request(qurl);
    QNetworkReply *reply = manager.get(request);
    QEventLoop loop;

    // Send GET request

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
      qWarning() << "Failed to fetch data from URL: " << reply->errorString();
      return QJsonObject();
    }

    const QByteArray responseData = reply->readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject json = doc.object();

    reply->deleteLater();
    return json;
  }
};
