// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_ERROR_H
#define HOLLOW_ERROR_H

#include <stdexcept>

class UrlNotLoadedProperly : public std::runtime_error
{
public:
  UrlNotLoadedProperly(const std::string message) : std::runtime_error (message) {
  }
};


#endif  // HOLLOW_ERROR_H
