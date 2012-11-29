#include <stdio.h>
#include <yipit/hollow/hollow.h>

int
main (int argc, char **argv)
{
  y_hollow_t *hollow;

  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s: <url-to-open>\n", argv[0]);
      return 1;
    }

  hollow = y_hollow_new ();
  printf ("%s\n", y_hollow_load (hollow, argv[1]));
  y_hollow_free (hollow);
  return 0;
}
