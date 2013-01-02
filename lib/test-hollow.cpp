#include <iostream>
#include <hollow/hollow.h>
#include <hollow/webpage.h>
#include <hollow/webpage.h>

#include <QApplication>
#include <QMainWindow>
#include <QWebView>

#define URL "http://bananarepublic.gap.com/browse/product.do?cid=71294&vid=1&pid=236175002"

int
main ()
{
  Hollow hollow;
  StringHashMap headers;
  UsernamePasswordPair credentials;
  Config config;

  hollow.setup();
  hollow.request("get", URL, "", headers, credentials, config);
  hollow.evaluateJavaScript("'123'");

}
