// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_H
#define HOLLOW_H

#include <QObject>
#include <QApplication>
#include <QNetworkRequest>
#include <QWebElement>
#include <QWebElementCollection>

#include <hollow/error.h>
#include <hollow/webpage.h>
#include <hollow/response.h>

class Hollow : public QObject
{
  Q_OBJECT

public:
  Hollow (QObject *parent=0);
  ~Hollow();
  Response* request(const char* method,
                    const char* url,
                    const char* payload,
                    StringHashMap& headers,
                    UsernamePasswordPair& credentials,
                    Config& config);

  // Just an informative variable that returns the Qt version that
  // sleepy hollow was compiled with. It's helpful to write tests that
  // won't run due to problems on Qt4.x
  const static int QT_VERSION_NUMBER = QT_VERSION;

  // The user *Must* call this function before using libhollow
  static void setup(void);
  static void teardown(void);
  QVariant evaluateJavaScript(const char*);
  QWebElement document(void);
  QWebElementCollection querySelectorAll(const char*);
  QWebElement querySelector(const char*);
  Response* lastResponse(void);
  QImage renderImage(QWebFrame* frame);
  QByteArray renderPNGBase64(QWebFrame *frame);

private:
  WebPage* m_lastPage;

  QUrl getValidURL(const char*);
  QNetworkAccessManager::Operation parseOperation(QString);
  QNetworkRequest prepareNetworkRequest(QUrl, StringHashMap);
  void waitForPage();
};


#endif
