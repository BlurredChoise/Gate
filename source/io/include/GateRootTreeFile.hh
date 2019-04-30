//
// Created by mdupont on 13/03/18.
//

#ifndef NUMPY_FROM_CPP_ROOTTREEFILE_HH
#define NUMPY_FROM_CPP_ROOTTREEFILE_HH



#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "GateTreeFile.hh"
#include "GateTreeFileManager.hh"

class GateRootTree : public GateTree
{
public:
  GateRootTree();



  void register_variable(std::string name, const void *p, std::type_index t_index) override;
  void register_variable(const std::string name, const std::string *p, size_t nb_char) override;
  void register_variable(const std::string name, const char *p, size_t nb_char) override;

 protected:
  template<typename T>
  void register_variable(const std::string name, const T *p)
  {

//      if(!std::is_arithmetic<T>::value)
//      {
//          throw std::invalid_argument(std::string("templated version of register_variable can not be used for type = ") +
//                                      abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr));
//      }
      register_variable(name, p, typeid(T));
  }



  void open(const char *s, std::ios_base::openmode) ;
  virtual void close();




  TFile * m_file;
  TTree *m_ttree;
private:


  template<typename T>
  void add_leaflist(const std::string &key)
  {
    m_tmapOfDefinition.emplace(typeid(T), key);
  }

  std::unordered_map<std::type_index, std::string> m_tmapOfDefinition;

};



class OutputRootTreeFile: public GateRootTree, public OutputTreeFile
{
 public:
  virtual void open(const std::string& s) ;

  bool is_open() override ;
  void close() override ;

  void write_header() override ;
  virtual void fill() override;


  void set_tree_name(const std::string &name) override ;


  void write_variable(std::string name, const void *p, std::type_index t_index) override;
  void write_variable(const std::string name, const std::string *p, size_t nb_char)override ;
  void write_variable(const std::string name, const char *p, size_t nb_char) override  ;

  template<typename T >
  void write_variable(const std::string name, const T *p)
  {
      register_variable(name, p);
  }


  static std::string _get_factory_name() { return "root"; }
  static std::unique_ptr<OutputTreeFile> _create_method() {
    return std::unique_ptr<OutputTreeFile>(new OutputRootTreeFile());
  }



private:
  std::unordered_map<const std::string*, std::string*> m_mapConstStringToRootString;
  static bool s_registered;


};


class InputRootTreeFile: public GateRootTree, public InputTreeFile
{
 public:
  void open(const std::string& s);
  bool is_open() override ;
  void close() override ;


  bool data_to_read() override ;
  void read_header() override;
  void read_entrie() override;



  template<typename T>
  void read_variable(const std::string name, T *p)
  {
      read_variable(name, p, typeid(T));
  }

  void read_variable(const std::string name, std::string *p);
  virtual ~InputRootTreeFile();

  void read_variable(const std::string name, void *p, std::type_index t_index);



 private:
  Long64_t m_current_entry;
//  std::unordered_map<std::string, std::string*> m_mapNameToRefString;
  std::unordered_map<char*, std::string*> m_mapNewStringToRefString;
//  std::vector<std::string*> m_vectorOfNewString;

};



#endif //NUMPY_FROM_CPP_ROOTTREEFILE_HH
