#include <iostream>
#include <qdebug.h>
#include <QUrl>
#include <QWidget>
#include <QWebPage>
#include <QWebFrame>
#include "webview.h"


WebView::WebView(QWidget *parent)
  : QWebView(parent)
  , m_progress(0)
  , m_page(new QWebPage(this))
  , m_loadedContent()
{
  // A WebView must contain a WebPage, let's set our custom one
  setPage(m_page);

  // Let's connect some signals
  connect(this, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
  connect(this, SIGNAL(loadFinished(bool)), SLOT(loadFinished(bool)));

  // Some more configuration to the page and to the view itself
  setAcceptDrops(true);
  page()->setForwardUnsupportedContent(true);
}


void
WebView::setProgress(int progress)
{
  std::cout << "progress reported " << progress << std::endl;
  m_progress = progress;
}


void
WebView::loadFinished(bool ok)
{
  if (!ok) {
    qWarning() << "Could not load the url" << url().toString();
    return;
  }

  if (m_progress != 100) {
    qWarning() << "Url finished loading but progress still"
               << progress();
    return;
  }

  // Just a nice warning :)
  std::cout << "Url finished successfuly" << std::endl;

  // Reseting the state
  m_progress = 0;

  // Updating our internal container
  m_loadedContent = page()->mainFrame()->toHtml().toAscii().data();
}


QString
WebView::getLoadedContent(void)
{
  return m_loadedContent;
}
