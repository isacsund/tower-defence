#include <json.hpp>
#include <string>

class ConfigManager {
 public:
  static ConfigManager* get_instance();
  bool ParseFile(const std::string& file_path, std::string& error_message);

 private:
  nlohmann::json config_;
};