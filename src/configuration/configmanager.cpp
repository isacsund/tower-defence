#include "configmanager.hpp"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

ConfigManager* ConfigManager::GetInstance() {
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
    boost::property_tree::ptree full_tree;

    boost::property_tree::json_parser::read_json(file_path, full_tree);

    if (full_tree.empty()) {
      error_message = "Configuration file is empty!";
      return false;
    }
    config_ = full_tree;
  } catch (boost::property_tree::json_parser::json_parser_error const& e) {
    if (e.line() == 0)
      error_message = e.message() + " (" + e.filename() + ")";
    else
      error_message = e.message() + " (" + e.filename() + ":" +
                      std::to_string(e.line()) + ")";
    return false;
  }
  return true;
}