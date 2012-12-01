// -*- Mode: c++; tab-width: 2 -*-


#include <stdexcept>

class UrlNotLoadedProperly : public std::runtime_error
{
public:
  UrlNotLoadedProperly(const std::string message) : std::runtime_error (message) {
  }
};
