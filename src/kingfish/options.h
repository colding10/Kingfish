#ifndef KINGFISH_OPTIONS_H
#define KINGFISH_OPTIONS_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Option {
  public:
    Option(const std::string& _name,
           const std::string& _defaultValue,
           const std::string& _type,
           const std::string& _min = "",
           const std::string& _max = "")
        : name(_name)
        , defaultValue(_defaultValue)
        , type(_type)
        , min(_min)
        , max(_max) {}

    std::string name;
    std::string defaultValue;
    std::string type;
    std::string min;
    std::string max;
};

class Options {
  public:
    // Default constructor
    Options() {
        // Add options with default values and types
        addOption("Debug Log File", "", "string");
        addOption("Threads", "1", "spin", "1", "1024");
        addOption("Hash", "16", "spin", "1", "33554432");
        addOption("Clear Hash", "", "button");
        addOption("Ponder", "false", "check");
        addOption("MultiPV", "1", "spin", "1", "500");
        addOption("Skill Level", "20", "spin", "0", "20");
        addOption("Move Overhead", "10", "spin", "0", "5000");
        addOption("Slow Mover", "100", "spin", "10", "1000");
        addOption("nodestime", "0", "spin", "0", "10000");
        addOption("UCI_Chess960", "false", "check");
        addOption("UCI_AnalyseMode", "false", "check");
        addOption("UCI_LimitStrength", "false", "check");
        addOption("UCI_Elo", "1350", "spin", "1350", "2850");
        addOption("UCI_ShowWDL", "false", "check");
        addOption("SyzygyPath", "", "string");
        addOption("SyzygyProbeDepth", "1", "spin", "1", "100");
        addOption("Syzygy50MoveRule", "true", "check");
        addOption("SyzygyProbeLimit", "7", "spin", "0", "7");
        addOption("Use NNUE", "true", "check");
        addOption("EvalFile", "nn-ad9b42354671.nnue", "string");
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
    void addOption(const std::string& name,
                   const std::string& defaultValue,
                   const std::string& type,
                   const std::string& min = "",
                   const std::string& max = "") {
        Option option(name, defaultValue, type, min, max);
        options[name]     = defaultValue;
        optionTypes[name] = option;
    }

    // Print all options in UCI format
    void printOptions() const {
        for (const auto& pair : optionTypes) {
            const Option&     option = pair.second;
            std::stringstream ss;
            ss << "option name " << option.name << " type " << option.type
               << " default " << option.defaultValue;
            if (option.type == "spin") {
                ss << " min " << option.min << " max " << option.max;
            }
            std::cout << ss.str() << std::endl;
        }
    }

  private:
    struct Option {
        std::string name;
        std::string type;
        std::string defaultValue;
        int         min;
        int         max;
    };

    std::unordered_map<std::string, Option>      optionTypes;
    std::unordered_map<std::string, std::string> optionValues;
};

#endif