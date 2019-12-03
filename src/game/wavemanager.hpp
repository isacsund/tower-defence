#pragma once
#include <boost/property_tree/ptree.hpp>
#include <string>

class WaveManager {
 public:
  static WaveManager& GetInstance();
  bool ParseFile(const std::string& file_path);

  boost::property_tree::ptree GetSubTree(const std::string& name);

  WaveManager(WaveManager const&) = delete;
  void operator=(WaveManager const&) = delete;

 private:
  WaveManager() {}
  boost::property_tree::ptree config_;
};

#define wave_manager WaveManager::GetInstance()