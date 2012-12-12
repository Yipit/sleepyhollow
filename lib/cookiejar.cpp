#include <QSettings>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include <hollow/cookiejar.h>


CookieJar::CookieJar(QObject *parent)
  : QNetworkCookieJar(parent)
  , m_cookiePot("SleepyHollow", "Cookies")
{
}

bool
CookieJar::setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url)
{
  QList<QVariant> myCookies;
  bool has_cookies = false;

  foreach (QNetworkCookie cookie, cookieList) {
    has_cookies = true;
    QByteArray rawCookie = cookie.toRawForm(QNetworkCookie::Full);
    // qDebug() << "+++" << rawCookie << "+++" << url;
    myCookies.append(rawCookie);
  }

  m_cookiePot.setValue(url.toString(), myCookies);

  // if (!has_cookies) {
  //   qDebug() << "{NO COOKIES FOR URL: " << url << "}\n";
  // }

  return has_cookies;
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl &url) const
{
  QString key = url.toString();
  QList<QNetworkCookie> cookieList;

  foreach (QVariant vCookie, m_cookiePot.value(key).toList()) {
    QByteArray rawCookie;
    rawCookie.append(vCookie.toString().toStdString().data());
    // qDebug() << "@@@" << rawCookie << "@@@" << url;
    cookieList << QNetworkCookie::parseCookies(rawCookie);
  }
  return cookieList;
}
