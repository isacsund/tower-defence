#include <boost/property_tree/ptree.hpp>
#include <iostream>

class ConfigManager {
 public:
  static ConfigManager* get_instance();
  bool ParseFile(const std::string& file_path, std::string& error_message);
  template <typename T>
  T GetValueOrDefault(const std::string& name, T default_value) {
    try {
      T value =
          (T)config_.get<T>(boost::property_tree::ptree::path_type(name, '/'));
      std::cout << value;
      if (std::is_same<T, std::string>::value) {
        std::string* str_val = (std::string*)&value;

        // Erase all quotes in the string
        str_val->erase(std::remove(str_val->begin(), str_val->end(), '"'),
                       str_val->end());

        return value;
      }
    } catch (boost::property_tree::ptree_bad_path& e) {
      std::cout << e.what() << std::endl;
    } catch (boost::property_tree::ptree_bad_data& e) {
      std::cout << e.what() << std::endl;
    }

    // If any errors occurred, return the default value
    return default_value;
  }

 private:
  boost::property_tree::ptree config_;
};