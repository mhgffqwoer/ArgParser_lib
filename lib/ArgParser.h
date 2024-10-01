#pragma once
#include <iostream>
#include <vector>

namespace ArgumentParser {

template <class T>
class Argument {
   public:
    std::string short_name_ = "";
    std::string long_name_ = "";
    std::string description_;
    bool default_ = false;
    T default_value_;
    bool multi_value_ = false;
    bool position_ = false;
    bool pointer_ = false;
    size_t min_args_count_ = 0;
    std::vector<T> values_;

    std::vector<T>* ps_ = nullptr;
    T* p_ = nullptr;

    Argument& Default(const T& value) {
        default_ = true;
        default_value_ = value;
        return *this;
    };

    Argument& StoreValue(T& value) {
        pointer_ = true;
        p_ = &value;
        return *this;
    };

    Argument& StoreValues(std::vector<T>& value) {
        pointer_ = true;
        ps_ = &value;
        return *this;
    };

    Argument& MultiValue(const size_t& min_count = 0) {
        multi_value_ = true;
        min_args_count_ = min_count;
        return *this;
    };

    Argument& Positional() {
        position_ = true;
        return *this;
    };
};

class ArgParser {
   private:
    // all arguments
    std::vector<Argument<std::string>*> string_arguments_ = {};
    std::vector<Argument<int>*> int_arguments_ = {};
    std::vector<Argument<bool>*> flag_arguments_ = {};

    // help argument
    std::string long_name_help_argument_;
    std::string short_name_help_argument_;
    std::string description_help_argument_;
    bool having_help_argument_ = false;
    bool use_help_argument_ = false;

    bool isInteger(const std::string& s) const {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    bool Find(const std::string& arg, const std::string& arg_name) const {
        return arg.find(arg_name) != std::string::npos;
    };

    bool isCorrect() const;

    void Pointer();

   public:
    std::string parser_name_;

    ArgParser(std::string s) : parser_name_(s){};

    ~ArgParser(){};

    // Parsing
    bool Parse(int argc, char** argv);

    bool Parse(std::vector<std::string> mass);

    // for String arguments
    Argument<std::string>& AddStringArgument(
        const std::string& long_name, const std::string& description = "");

    Argument<std::string>& AddStringArgument(
        const char short_name, const std::string& long_name = "",
        const std::string& description = "");

    std::string GetStringValue(std::string arg_name, size_t index = 0) const;

    // for Int arguments
    Argument<int>& AddIntArgument(const std::string& long_name,
                                  const std::string& description = "");

    Argument<int>& AddIntArgument(const char short_name,
                                  const std::string& long_name = "",
                                  const std::string& description = "");

    int GetIntValue(std::string arg_name, size_t index = 0) const;

    // for flag arguments
    Argument<bool>& AddFlag(const std::string& long_name,
                            const std::string& description = "");

    Argument<bool>& AddFlag(const char short_name,
                            const std::string& long_name = "",
                            const std::string& description = "");

    bool GetFlag(std::string arg_name, size_t index = 0) const;

    // for help argument
    void AddHelp(const std::string& long_name,
                 const std::string& description = "");

    void AddHelp(const char short_name, const std::string& long_name = "",
                 const std::string& description = "");

    const std::string HelpDescription() const;

    bool Help() const { return use_help_argument_; };
};

}  // namespace ArgumentParser