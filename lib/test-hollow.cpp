#include <iostream>
#include <hollow/hollow.h>

int
main (int argc, char **argv)
{
  Hollow hollow;
  const char* payload = NULL;
  StringHashMap headers;
  headers["Content-Type"] = "application/x-www-form-urlencoded";

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << ": <url-to-open>" << std::endl;
    return 1;
  }

  std::cout << "Creating the hollow instance" << std::endl;


  std::cout << "Making the request" << std::endl;
  Response *resp = hollow.request("get", argv[1], payload, headers);

  if (resp == NULL) {
    std::cout << "Not good bro, "
              << "we got an error while trying to request the url "
              << argv[1]
              << "."
              << std::endl;

    // Giving a better sense of what actually happen
    Error *error = Error::last();
    if (error != NULL) {
      std::cout << "Here's the error: "
                << error->what()
                << std::endl;
      return 1;
    }
  }

  std::cout << "Reading the response:" << std::endl
            << "status: " << resp->getStatusCode()
            << std::endl;
  return 0;
}
