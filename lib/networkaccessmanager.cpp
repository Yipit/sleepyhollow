#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <hollow/networkaccessmanager.h>


NetworkAccessManager::NetworkAccessManager(QObject *parent)
  : QNetworkAccessManager(parent)
{ }


QNetworkReply *
NetworkAccessManager::createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData)
{
  QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
  emit resourceRequested(req);
  return reply;
}
