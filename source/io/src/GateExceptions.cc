//
// Created by mdupont on 04/04/19.
//

#include "GateExceptions.hh"
FileExceptions::FileExceptions(const std::string &arg) : runtime_error(arg)
{}
ClosedFileException::ClosedFileException(const std::string &arg) : FileExceptions(arg)
{}
HeaderException::HeaderException(const std::string &arg) : FileExceptions(arg)
{}
MissingHeaderException::MissingHeaderException(const std::string &arg) : HeaderException(arg)
{}
MalFormedHeaderException::MalFormedHeaderException(const std::string &arg) : HeaderException(arg)
{}
KeyAlreadyExistsException::KeyAlreadyExistsException(const std::string &arg) : HeaderException(arg)
{}
ModeFileException::ModeFileException(const std::string &arg) : FileExceptions(arg)
{}
