#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <hollow/networkaccessmanager.h>


NetworkAccessManager::NetworkAccessManager(QObject *parent)
  : QNetworkAccessManager(parent)
{}

NetworkAccessManager*
NetworkAccessManager::instance(void) {
  static NetworkAccessManager *singleton;
  if (!singleton) {
    singleton = new NetworkAccessManager();
  }
  return singleton;
}

QNetworkReply *
NetworkAccessManager::createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData)
{
  QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
  emit resourceRequested(req);
  return reply;
}
