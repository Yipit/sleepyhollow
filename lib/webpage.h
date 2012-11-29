#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebPage>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
  inline int progress() const { return m_progress; }
  QString getLoadedContent(void);

private slots:
  void setProgress(int progress);
  void loadFinished(bool ok);

signals:
  void finishedProcessing(bool ok);

private:
  int m_progress;
  QString m_loadedContent;
};


#endif
