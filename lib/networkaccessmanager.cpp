#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <hollow/networkaccessmanager.h>


NetworkAccessManager::NetworkAccessManager(QObject *parent)
  : QNetworkAccessManager(parent)
  , m_cookieJar(new CookieJar(this))
{
  setCookieJar(m_cookieJar);
}

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
