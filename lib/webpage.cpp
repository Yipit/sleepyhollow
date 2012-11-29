#include <iostream>
#include <qdebug.h>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include "webpage.h"


WebPage::WebPage(QObject *parent)
  : QWebPage(parent)
  , m_progress(0)
  , m_loadedContent()
{
  // Let's connect some signals
  connect(this, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
  connect(this, SIGNAL(loadFinished(bool)), SLOT(loadFinished(bool)));

  // Some more configuration to the page and to the page itself
  setForwardUnsupportedContent(true);
}


void
WebPage::setProgress(int progress)
{
  std::cout << "progress reported " << progress << std::endl;
  m_progress = progress;
}


void
WebPage::loadFinished(bool ok)
{
  if (!ok) {
    qWarning() << "Could not load the url" << mainFrame()->url().toString();
    emit finishedProcessing(false);
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
  m_loadedContent = mainFrame()->toHtml().toAscii().data();

  emit finishedProcessing(true);
}


QString
WebPage::getLoadedContent(void)
{
  return m_loadedContent;
}
