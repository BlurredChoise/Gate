//
// Created by mdupont on 12/03/18.
//

#ifndef NUMPY_FROM_CPP_TREEFILE_HH
#define NUMPY_FROM_CPP_TREEFILE_HH

#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <iostream>
#include <memory>


#include "GateFile.hh"

class Data
{
public:
  Data(const void * pointer_to_data,
       const std::string _name,
       const std::type_index type_index

  ) : m_pointer_to_data(pointer_to_data),
      m_name(_name),
      m_type_index(type_index)
  {

  }

  const std::string &name() const
  {
    return m_name;
  }

public:
  const void *m_pointer_to_data;
//  const size_t m_size_of_data;
  const std::string m_name;
  std::type_index m_type_index;
};



class GateTree
{
public:
  GateTree();



protected:
  virtual void register_variable(std::string name, const void *p, std::type_index t_index) = 0;
  virtual void register_variable(const std::string name, const std::string *p, size_t nb_char) = 0;
  virtual void register_variable(const std::string name, const char *p, size_t nb_char) = 0;


private:
  template<typename T>
  void add_size()
  {
    m_tmapOfSize[typeid(T)] = sizeof(T);
  }

  template<typename T>
  void add_name(const std::string &name)
  {
//    m_tmapOfName[typeid(T)] = name;
    m_tmapOfName.emplace(typeid(T), name);
  }
protected:
  const std::string type_to_name(std::type_index t_index);
  std::unordered_map<std::type_index, std::size_t> m_tmapOfSize;
  std::unordered_map<std::type_index, std::string> m_tmapOfName;



};



class OutputTreeFile : public GateFile
{
public:
  OutputTreeFile();

  virtual void open(const std::string& s) = 0;
  virtual void write_header() = 0;
  virtual void fill() = 0;


  virtual void write_variable(std::string name, const void *p, std::type_index t_index) = 0;
  virtual void write_variable(const std::string name, const std::string *p, size_t nb_char) = 0;
  virtual void write_variable(const std::string name, const char *p, size_t nb_char) = 0;
  virtual void set_tree_name(const std::string &name) ;



  virtual ~OutputTreeFile();

protected:
  std::string m_nameOfTree;


};


class InputTreeFile : public GateFile
{
public:
  virtual void read_header() = 0;
  virtual void read_entrie() = 0;
  virtual bool data_to_read() = 0;
};




#endif //NUMPY_FROM_CPP_TREEFILE_HH
