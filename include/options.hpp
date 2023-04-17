#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED

#include <iostream>
#include <map>
#include <string>

class Options {
  public:
    // Default constructor
    Options() {
        // Add options with default values and types
        addOption("Hash", "16", "spin");
        addOption("Clear Hash", "false", "check");
        addOption("UCI_AnalyseMode", "false", "check");
    }

    // Get the value of an option by key
    std::string getOptionValue(const std::string& key) const {
        auto iter = options.find(key);
        if (iter != options.end()) {
            return iter->second;
        } else {
            return "";
        }
    }

    // Add a new option
    void addOption(const std::string& key,
                   const std::string& value,
                   const std::string& type) {
        options[key]     = value;
        optionTypes[key] = type;
    }

    // Print all options in UCI format
    void printOptions() const {
        for (const auto& pair : options) {
            std::cout << "option name " << pair.first << " type "
                      << optionTypes.at(pair.first) << " default "
                      << pair.second << std::endl;
        }
    }

  private:
    std::map<std::string, std::string> options; // Maps option keys to values
    std::map<std::string, std::string> optionTypes; // Maps option keys to types
};

#endif