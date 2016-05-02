#include "util/GenericException.h"

namespace Lonely {


GenericException::GenericException(const std::string& whatString__)
  : std::exception(),
    whatString_(whatString__) { };

const char* GenericException::what() const throw() {
  return whatString_.c_str();
}


};
