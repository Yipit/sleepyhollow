#include <iostream>
#include <yipit/hollow/hollow.h>

int
main (int argc, char **argv)
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << ": <url-to-open>" << std::endl;
    return 1;
  }

  Hollow hollow;
  std::cout << hollow.getUrlContent(argv[1]) << std::endl;
  return 0;
}
