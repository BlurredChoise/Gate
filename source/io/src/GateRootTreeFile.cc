//
// Created by mdupont on 13/03/18.
//

#include "GateRootTreeFile.hh"
#include <sstream>
#include <iostream>
#include <fstream>

#include "GateTreeFileManager.hh"

using namespace std;


GateRootTree::GateRootTree() : GateTree()
{

//  m_tmapOfDefinition[typeid(Char_t)] = "C";

  add_leaflist<Char_t>("B");
  add_leaflist<UChar_t>("b");
  add_leaflist<Short_t>("S");
  add_leaflist<UShort_t>("s");
  add_leaflist<Int_t>("I");
  add_leaflist<UInt_t>("i");
  add_leaflist<Float_t>("F");
  add_leaflist<Double_t>("D");
  add_leaflist<Long64_t>("L");
  add_leaflist<ULong64_t>("l");
  add_leaflist<Bool_t>("O");

  add_leaflist<string>("C");
}

void GateRootTree::register_variable(std::string name, const void *p, std::type_index t_index)
{

  void *pp = (void*)p;


  std::stringstream leaf_ss;
  leaf_ss << name << "/" << m_tmapOfDefinition.at(t_index);
  string leaf_s = leaf_ss.str();

//  cout << "RootTreeFile::register_variable name = " << name << " t_index = " << t_index.name() << " leaf_s = " << leaf_s << "\n";


  m_ttree->Branch(name.c_str(), pp, leaf_s.c_str());
//  m_ttree->Branch(name.c_str(), pp);

}

void GateRootTree::register_variable(const std::string name, const std::string *p, size_t nb_char)
{

}

void GateRootTree::register_variable(const std::string name, const char *p, size_t nb_char)
{
  register_variable(name, p, typeid(string));

}

void OutputRootTreeFile::open(const std::string& s)
{
  GateFile::open(s.c_str(), ios_base::out);

  m_file = new TFile(s.c_str(), "RECREATE");
  cout << "open, name = " << m_nameOfTree << "\n";
  m_ttree = new TTree(m_nameOfTree.c_str(), m_nameOfTree.c_str());
}

void InputRootTreeFile::open(const std::string& s)
{
  GateFile::open(s.c_str(), ios_base::in);
  m_file = new TFile(s.c_str(), "READ");
  m_file->GetObject("tree", m_ttree);
}

void GateRootTree::close()
{
  m_file->Close();
  delete m_file;
}

void OutputRootTreeFile::close()
{
  m_file->Write();
  GateRootTree::close();
}

void InputRootTreeFile::close()
{
  GateRootTree::close();
}


bool OutputRootTreeFile::is_open()
{
    return m_file->IsOpen();
}

bool InputRootTreeFile::is_open()
{
  return m_file->IsOpen();
}

void OutputRootTreeFile::fill()
{

  for(auto ss: m_mapConstStringToRootString)
  {
    ss.second->assign(*ss.first);
  }

  m_ttree->Fill();
}

void OutputRootTreeFile::write_header()
{

}

void OutputRootTreeFile::write_variable(std::string name, const void *p, std::type_index t_index)
{
    this->register_variable(name, p, t_index);
}
void OutputRootTreeFile::write_variable(const std::string name, const std::string *p, size_t nb_char)
{
    std::string* s = new std::string();
    m_mapConstStringToRootString.emplace(p, s);
    m_ttree->Branch(name.c_str(), s);
//    this->register_variable(name, s, nb_char);
}
void OutputRootTreeFile::write_variable(const std::string name, const char *p, size_t nb_char)
{
    this->register_variable(name, p, nb_char);
}

void InputRootTreeFile::read_variable(const std::string name, void *p, std::type_index t_index)
{
    m_ttree->SetBranchAddress(name.c_str(), p);
}
void InputRootTreeFile::read_header()
{
    m_current_entry = 0;

}
void InputRootTreeFile::read_entrie()
{
    m_ttree->GetEntry(m_current_entry);

    for(auto ss: m_mapNewStringToRefString)
    {
        ss.second->assign(ss.first);
    }

    m_current_entry++;
}
bool InputRootTreeFile::data_to_read()
{
    return m_current_entry < m_ttree->GetEntries();
}
void InputRootTreeFile::read_variable(const std::string name, std::string *p)
{
    cout << "read_variable for string" << endl;
//    string *s = new string();
    char* s = new char[1024];
    m_mapNewStringToRefString.emplace(s, p);

    m_ttree->SetBranchAddress(name.c_str(), s);

//    m_vectorOfNewString.push_back(s);
//    m_ttree->SetBranchAddress(name.c_str(), p);

}
InputRootTreeFile::~InputRootTreeFile()
{
    for(auto ss: m_mapNewStringToRefString)
    {
//        cout << "~InputRootTreeFile" << endl;
        delete[](ss.first);
    }
}


void OutputRootTreeFile::set_tree_name(const std::string &name)
{
  OutputTreeFile::set_tree_name(name);
  m_ttree->SetName(name.c_str());
}


bool OutputRootTreeFile::s_registered =
    GateOutputTreeFileFactory::_register(OutputRootTreeFile::_get_factory_name(), &OutputRootTreeFile::_create_method);




