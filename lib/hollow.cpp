#include <stdlib.h>
#include <string.h>
#include <QUrl>
#include <yipit/hollow/hollow.h>
#include "manager.h"
#include "error.h"

struct y_hollow
{
  Manager *manager;
};


y_hollow_t *
y_hollow_new (void)
{
  y_hollow_t *hollow;

  hollow = (y_hollow_t *) malloc (sizeof (y_hollow_t));
  hollow->manager = new Manager();
  return hollow;
}


void
y_hollow_free (y_hollow_t *hollow)
{
  delete hollow->manager;
  free (hollow);
}


char *
y_hollow_load (y_hollow_t *hollow, const char *url)
{
  QUrl qurl(url);
  try {
    return strdup (hollow->manager->getUrlContent(qurl).toAscii().data());
  } catch (UrlNotLoadedProperly& exc) {
    fprintf (stderr, "Fuck! %s", exc.what());
    return NULL;
  }
}
