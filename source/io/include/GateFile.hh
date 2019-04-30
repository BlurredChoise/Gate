//
// Created by mdupont on 17/12/18.
//

#ifndef NUMPY_FROM_CPP_FILE_HH
#define NUMPY_FROM_CPP_FILE_HH

#include <iostream>

class GateFile
{

public:
  virtual bool is_open() = 0;
  virtual void close() = 0;
  virtual void open(const char* s, std::ios_base::openmode) ;


protected:
  std::ios_base::openmode m_mode;

};


#endif //NUMPY_FROM_CPP_FILE_HH
