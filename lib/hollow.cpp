#include <stdlib.h>
#include <QApplication>
#include <yipit/hollow/hollow.h>

#include "webview.h"

struct y_hollow
{
  QApplication *app;
  WebView *webview;
};


y_hollow_t *
y_hollow_new (void)
{
  y_hollow_t *hollow;

  /* Mocking data needed to create a new app */
  int argc = 0;
  char **argv = NULL;

  hollow = (y_hollow_t *) malloc (sizeof (y_hollow_t));
  hollow->app = new QApplication(argc, argv, false);
  hollow->webview = new WebView();

  return hollow;
}


void
y_hollow_free (y_hollow_t *hollow)
{
  delete hollow->webview;
  delete hollow->app;
  free (hollow);
}


char *
y_hollow_load (y_hollow_t *hollow, const char *url)
{
  hollow->webview->setUrl (QUrl (url));
  QApplication::processEvents();
  return hollow->webview->getLoadedContent().toAscii().data();
}
