//
// Created by mdupont on 04/04/19.
//

#ifndef NUMPY_FROM_CPP_EXCEPTIONS_HH
#define NUMPY_FROM_CPP_EXCEPTIONS_HH

#include <stdexcept>

class FileExceptions : public std::runtime_error {
 public:
  FileExceptions(const std::string &arg);

};




class ClosedFileException: public FileExceptions {
 public:
  ClosedFileException(const std::string &arg);

};

class ModeFileException: public FileExceptions {
 public:
  ModeFileException(const std::string &arg);
};

class HeaderException: public FileExceptions {
 public:
  HeaderException(const std::string &arg);

};

class MissingHeaderException: public HeaderException {
 public:
  MissingHeaderException(const std::string &arg);

};

class MalFormedHeaderException: public HeaderException {
 public:
  MalFormedHeaderException(const std::string &arg);

};


class KeyAlreadyExistsException: public HeaderException {
 public:
  KeyAlreadyExistsException(const std::string &arg);

};





#endif //NUMPY_FROM_CPP_EXCEPTIONS_HH
