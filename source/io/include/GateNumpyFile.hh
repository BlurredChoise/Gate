//
// Created by mdupont on 30/08/17.
//

#ifndef NUMPY_FROM_CPP_NUMPYHEADER_HH
#define NUMPY_FROM_CPP_NUMPYHEADER_HH

#include <string>
#include <ostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include <cxxabi.h>

#include "GateFile.hh"
#include "GateTreeFile.hh"



class NumpyData : public Data
{
public:
  NumpyData(const void * pointer_to_data,
       const size_t size_of_data,
       const std::string _name,
       const std::string numpy_format,
       const std::type_index type_index

  ) : Data(pointer_to_data, _name, type_index),
      m_size_of_data(size_of_data),
      m_numpy_format(numpy_format),
      m_nb_characters(0),
      m_type_index_read(type_index),
      buffer_read(0)
  {
    std::stringstream descr;
    descr << "('" << m_name << "', '" << numpy_format << "')";
    m_numpy_description = descr.str();
  }

  const std::string &numpy_description() const
  {
    return m_numpy_description;
  }

public:
  const size_t m_size_of_data;
  const std::string m_numpy_format;
  size_t m_nb_characters;
  std::string m_numpy_description;

  std::type_index m_type_index_read; // type index of read variable, in case where we want to read a string
  char *buffer_read ;
};

class GateNumpyTree : public GateTree
{
public:
  GateNumpyTree();

public:


protected:
  void register_variable(std::string name, const void *p, std::type_index t_index) override;
  void register_variable(const std::string name, const char *p, size_t nb_char) override;
  void register_variable(const std::string name, const std::string *p, size_t nb_char) override;

//  void write_header() override ;
//
//
  template<typename T>
  void register_variable(const std::string name, const T *p)
  {
//    std::cout << "T is_arithmetic == " << std::is_arithmetic<T>::value << std::endl;
//    std::cout << "T is_scalar == " << std::is_scalar<T>::value << std::endl;

    if(!std::is_arithmetic<T>::value)
    {
      throw std::invalid_argument(std::string("templated version of register_variable can not be used for type = ") +
                                      abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr));
    }
    register_variable(name, p, typeid(T));
  }




private:
  void register_variable(std::string name, const void *p, const size_t size, std::string numpy_format, std::type_index t_index);

  void fill_maps(std::type_index t_index, const std::string);

  template <typename T>
  void fill_maps(const std::string s)
  {
      fill_maps(std::type_index(typeid(T)), s);
  }


protected:
  uint64_t m_nb_elements;
public:
  uint64_t nb_elements() const;

protected:
  std::vector<NumpyData> m_vector_of_pointer_to_data;
  std::fstream m_file;

  uint64_t m_position_before_shape;
  uint64_t m_position_after_shape;

  const std::string magic_prefix = "\x93NUMPY";
  std::unordered_map<std::type_index, std::string> m_tmap_cppToNumpy;
  std::unordered_map<std::string, std::type_index> m_tmap_numpyToCpp;

};


class GateOutputNumpyTreeFile: public GateNumpyTree, public OutputTreeFile
{
public:
  GateOutputNumpyTreeFile();

  bool is_open() override;

  void close() override;

  void open(const std::string& s) override ;
  void write_header() override ;
  virtual void fill() override;

  void write_variable(std::string name, const void *p, std::type_index t_index) override;
  void write_variable(const std::string name, const std::string *p, size_t nb_char)override ;
  void write_variable(const std::string name, const char *p, size_t nb_char) override  ;


  template<typename T >
  void write_variable(const std::string name, const T *p)
  {
    register_variable(name, p);
  }

  static std::string _get_factory_name() { return "npy"; }
  static std::unique_ptr<OutputTreeFile> _create_method() {
    return std::unique_ptr<OutputTreeFile>(new GateOutputNumpyTreeFile());
  }



private:
  bool m_write_header_called;
  static bool s_registered;
};


class InputNumpyTreeFile: public GateNumpyTree, public InputTreeFile
{
public:
  void open(const std::string &name) ;
  bool is_open() override;
  virtual void read_entrie();
  void close() override;

  template<typename T>
  void read_variable(const std::string name, T *p)
  {
      read_variable(name, p, typeid(T));
  }

//  void read_variable(const std::string name, char *p);

  void read_variable(const std::string name, void *p, std::type_index t_index);


public:
  void read_header() override;
  bool data_to_read();

private:
//  using NumpyTree::open;
  size_t  m_lenght_of_file;
};


#endif //NUMPY_FROM_CPP_NUMPYHEADER_HH
