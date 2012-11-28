#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebPage>

class WebView : public QWebView
{
  Q_OBJECT

public:
  WebView(QWidget *parent=0);
  inline int progress() const { return m_progress; }
  QString getLoadedContent(void);

private slots:
  void setProgress(int progress);
  void loadFinished(bool);

private:
  int m_progress;
  QWebPage *m_page;
  QString m_loadedContent;
};


#endif
