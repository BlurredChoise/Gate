//
// Created by mdupont on 12/03/18.
//

#ifndef NUMPY_FROM_CPP_TREEFILEMANAGER_HH
#define NUMPY_FROM_CPP_TREEFILEMANAGER_HH

#include <functional>
#include <vector>
#include <string>
#include <map>

#include "GateTreeFile.hh"


typedef const std::function<std::unique_ptr<OutputTreeFile>()> TCreateMethod;
//typedef const std::function<OutputTreeFile*()> TCreateMethod;
typedef std::map<const std::string,TCreateMethod> CreateMethodMap;

//typedef std::unique_ptr<OutputTreeFile> (TCreateMethod)();

class GateOutputTreeFileFactory
{
public:
//  using TCreateMethod = std::unique_ptr<OutputTreeFile>(*)();

public:
  GateOutputTreeFileFactory() = delete;

  static bool _register(const std::string name, TCreateMethod& funcCreate);

  static std::unique_ptr<OutputTreeFile> _create(const std::string& name);


private:
  static CreateMethodMap& get_methods_map()
  {
    static CreateMethodMap s_methods;
    return s_methods;
  }


};



class GateOutputTreeFileManager
{
public:
  GateOutputTreeFileManager();

  virtual ~GateOutputTreeFileManager();


//  void add_file(OutputTreeFile *f);
  std::unique_ptr<OutputTreeFile> add_file(const std::string &file_path, const std::string &kind);

  template<typename T>
  void write_variable(const std::string name, const T *p)
  {
    for(auto&& f : m_listOfTreeFile)
    {
      f->write_variable(name, p, typeid(T));
    }
  }


  void write_variable(const std::string name, const std::string *p, size_t nb_char);
  void write_variable(const std::string name, const char *p, size_t nb_char);
  virtual void set_tree_name(const std::string name);

  void fill();
  void close();
  void write_header();



private:
  std::vector<std::unique_ptr<OutputTreeFile>> m_listOfTreeFile;
  std::string m_nameOfTree;
};


#endif //NUMPY_FROM_CPP_TREEFILEMANAGER_HH
