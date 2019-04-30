//
// Created by mdupont on 13/03/18.
//

#ifndef NUMPY_FROM_CPP_ASCIIFILE_HH
#define NUMPY_FROM_CPP_ASCIIFILE_HH

#include <fstream>
#include <vector>
#include <functional>
#include <cxxabi.h>
#include <strstream>

#include <string>
#include <sstream>
#include <iostream>

#include "GateTreeFile.hh"
#include "GateTreeFileManager.hh"

class AsciiData;

typedef const std::function<void(const AsciiData *, std::fstream&)> save_to_file_f;
typedef const std::function<void(AsciiData *, const std::string&)> read_from_string_f;



class AsciiData : public Data
{
public:
  AsciiData(const void *pointer_to_data, const std::string &name, const std::type_index &type_index,
            save_to_file_f &save_to_file, read_from_string_f &read_from_string);


public:
//  void (*save_to_file)(const void *d, std::ofstream &file);
  save_to_file_f m_save_to_file;
  read_from_string_f m_read_from_string;
  size_t m_index_of_this_data_in_header;
 private:



};

template<typename T>
void save_to_file(const AsciiData *d, std::fstream &file)
{
  T *p = (T*)d->m_pointer_to_data;
  file << *p ;
}

template<typename T>
void read_from_string(AsciiData *d, const std::string& s)
{
    std::stringstream ss(s);

    T *p = (T*)d->m_pointer_to_data;
    ss >> *p ;
}

class GateAsciiTree : public GateTree
{

public:
  GateAsciiTree();


 protected:
  void  open(const char* s, std::ios_base::openmode)  ;
  void register_variable(std::string name, const void *p, std::type_index t_index) override;
  void register_variable(const std::string name, const char *p, size_t nb_char) override;
  void register_variable(const std::string name, const std::string *p, size_t nb_char) override;



 protected:
  template<typename T>
  void register_variable(const std::string name, const T *p)
  {

      if(!std::is_arithmetic<T>::value)
      {
          throw std::invalid_argument(std::string("templated version of register_variable can not be used for type = ") +
                                      abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr));
      }
      register_variable(name, p, typeid(T));
  }

  std::unordered_map<std::type_index, save_to_file_f> m_tmapOfSave_to_file;
  std::unordered_map<std::type_index, read_from_string_f> m_tmapOfRead_from_string;


private:

  template<typename T>
  void add_save_to_file_function()
  {
      m_tmapOfSave_to_file.emplace(typeid(T), &save_to_file<T> );
  }

  template<typename T>
  void add_read_from_string_function()
  {
      m_tmapOfRead_from_string.emplace(typeid(T), &read_from_string<T> );
  }

  template<typename T>
  void attach_read_and_write_function()
  {
      add_read_from_string_function<T>();
      add_save_to_file_function<T>();
  }


 protected:
  std::vector<AsciiData> m_vector_of_pointer_to_data;
  std::string m_sep;
};


class OutputAsciiTreeFile: public GateAsciiTree, public OutputTreeFile
{
 public:
  OutputAsciiTreeFile();
  void open(const std::string& s);
  bool is_open() override ;
  void close() override ;


  void write_variable(std::string name, const void *p, std::type_index t_index) override;
  void write_variable(const std::string name, const std::string *p, size_t nb_char)override ;
  void write_variable(const std::string name, const char *p, size_t nb_char) override  ;


  template<typename T >
  void write_variable(const std::string name, const T *p)
  {
      register_variable(name, p);
  }

  void write_header() override;
  void fill() override;
  static std::string _get_factory_name() { return "txt"; }
  static std::unique_ptr<OutputTreeFile> _create_method() {
    return std::unique_ptr<OutputTreeFile>(new OutputAsciiTreeFile());
  }

private:
  std::fstream m_file;
  static bool s_registered;
};


class InputAsciiTreeFile: public GateAsciiTree, public InputTreeFile {
 public:
  void open(const std::string& s);
  void close() override ;
  bool is_open() override ;
  void read_header() override;
  void read_entrie() override;
  bool data_to_read() override ;

  template<typename T>
  void read_variable(const std::string name, T *p)
  {
      read_variable(name, p, typeid(T));
  }

  void read_variable(const std::string name, void *p, std::type_index t_index);

 private:
  size_t  m_lenght_of_file;
  std::vector<std::string> m_list_from_header;
  std::fstream m_file;

};



#endif //NUMPY_FROM_CPP_ASCIIFILE_HH
