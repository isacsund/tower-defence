#include "wavemanager.hpp"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

WaveManager& WaveManager::GetInstance() {
  static WaveManager instance;
  return instance;
}

bool WaveManager::ParseFile(const std::string& file_path) {
  if (!boost::filesystem::exists(file_path)) {
    return false;
  }
  try {
    boost::property_tree::ptree full_tree;

    boost::property_tree::json_parser::read_json(file_path, full_tree);

    if (full_tree.empty()) {
      return false;
    }
    config_ = full_tree;
  } catch (boost::property_tree::json_parser::json_parser_error const& e) {
    return false;
  }
  return true;
}

boost::property_tree::ptree WaveManager::GetSubTree(const std::string& name) {
  return config_.get_child(name);
}