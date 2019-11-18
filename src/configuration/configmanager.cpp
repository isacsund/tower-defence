#include "configmanager.hpp"
#include <boost/filesystem.hpp>

using json = nlohmann::json;

ConfigManager* ConfigManager::get_instance() {
  static ConfigManager instance;
  return &instance;
}

bool ConfigManager::ParseFile(const std::string& file_path,
                              std::string& error_message) {
  if (!boost::filesystem::exists(file_path)) {
    error_message = "File doesn't exist!";
    return false;
  }
  try {
    nlohmann::json config = json::parse(file_path);
    if (config.empty()) {
      error_message = "Config file is empty!";
    }
    config_ = config;
  } catch (const std::exception& e) {
    error_message = e.what();
    return false;
  }
  return true;
}