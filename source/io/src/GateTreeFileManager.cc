//
// Created by mdupont on 12/03/18.
//

#include "GateTreeFileManager.hh"


#include <iostream>
#include <memory>
#include <iostream>
#include <exception>
#include "GateExceptions.hh"

#include "GateRootTreeFile.hh"

using namespace std;


//map<const string, const string> OutputTreeFileFactory::s_methods;


bool GateOutputTreeFileFactory::_register(const string name,
                                        TCreateMethod& funcCreate)
{
    auto it = get_methods_map().find(name);
    if(it != get_methods_map().end())
    {
      throw KeyAlreadyExistsException("nooooooooo");
    }

    get_methods_map().emplace(name, funcCreate);
    return true;
  return false;
}

std::unique_ptr<OutputTreeFile>
GateOutputTreeFileFactory::_create(const string& name)
{
  auto it = get_methods_map().find(name);
  if(it != get_methods_map().end())
  {
    return it->second(); // call the createFunc
  }

  return nullptr;
}




GateOutputTreeFileManager::GateOutputTreeFileManager()
{
//  cout << "TreeFileManager::TreeFileManager()\n";


}




void GateOutputTreeFileManager::write_variable(const std::string name, const std::string *p, size_t nb_char)
{
  for(auto&& f : m_listOfTreeFile)
  {
    f->write_variable(name, p, nb_char);
  }

}

void GateOutputTreeFileManager::write_variable(const std::string name, const char *p, size_t nb_char)
{
  for(auto&& f : m_listOfTreeFile)
  {
    f->write_variable(name, p, nb_char);
  }
}

void GateOutputTreeFileManager::close()
{
  for(auto&& f : m_listOfTreeFile)
  {
    f->close();
  }
}

void GateOutputTreeFileManager::fill()
{
  for(auto& f : m_listOfTreeFile)
  {
    f->fill();
  }
}

void GateOutputTreeFileManager::write_header()
{

  for(auto&& f : m_listOfTreeFile)
  {
    f->set_tree_name(m_nameOfTree);
    f->write_header();
  }
}

std::unique_ptr<OutputTreeFile> GateOutputTreeFileManager::add_file(const std::string &file_path, const std::string &kind)
{

  auto h = GateOutputTreeFileFactory::_create(kind);

  if(!h)
  {
    std::stringstream ss;
    ss << "Error do not know type '"  << kind; ;
    throw std::runtime_error(ss.str());
  }


//  std::cout << "add_file in TreeFileManager done h = " << h << std::endl;

  h->open(file_path);


  m_listOfTreeFile.push_back(move(h));
  return h;
}

GateOutputTreeFileManager::~GateOutputTreeFileManager()
{

}

void GateOutputTreeFileManager::set_tree_name(const std::string name)
{
  m_nameOfTree = name;

}
