#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslSocket>
#include <QSslConfiguration>

#include <hollow/networkaccessmanager.h>

NetworkAccessManager::NetworkAccessManager(QObject *parent)
  : QNetworkAccessManager(parent)
{
    setCookieJar(new CookieJar(parent));

  if (QSslSocket::supportsSsl()) {
    m_sslConfiguration = QSslConfiguration::defaultConfiguration();
    m_sslConfiguration.setProtocol(QSsl::SslV3);
  }
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
NetworkAccessManager::createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
  QNetworkRequest req(request);
  if (!QSslSocket::supportsSsl()) {
    if (req.url().scheme().toLower() == QLatin1String("https"))
      qWarning() << "Request using https scheme without SSL support";
  } else {
    req.setSslConfiguration(m_sslConfiguration);
  }
  QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
  emit resourceRequested(req);
  return reply;
}
