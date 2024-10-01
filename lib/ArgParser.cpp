#include "ArgParser.h"
using AP = ArgumentParser::ArgParser;
using namespace ArgumentParser;

void AP::Pointer() {
    for (auto arg_ : string_arguments_) {
        if (arg_->pointer_ && arg_->values_.size() > 0) {
            if (arg_->p_ != nullptr) {
                *arg_->p_ = arg_->values_[0];
            }
            if (arg_->ps_ != nullptr) {
                *arg_->ps_ = arg_->values_;
            }
        }
    }
    for (auto arg_ : int_arguments_) {
        if (arg_->pointer_ && arg_->values_.size() > 0) {
            if (arg_->p_ != nullptr) {
                *arg_->p_ = arg_->values_[0];
            }
            if (arg_->ps_ != nullptr) {
                *arg_->ps_ = arg_->values_;
            }
        }
    }
    for (auto arg_ : flag_arguments_) {
        if (arg_->pointer_ && arg_->values_.size() > 0) {
            if (arg_->p_ != nullptr) {
                *arg_->p_ = arg_->values_[0];
            }
            if (arg_->ps_ != nullptr) {
                *arg_->ps_ = arg_->values_;
            }
        }
    }
}

bool AP::isCorrect() const {
    for (auto arg_ : string_arguments_) {
        if (arg_->values_.size() < arg_->min_args_count_) {
            return false;
        }
        if (!arg_->default_ && arg_->values_.size() == 0) {
            return false;
        }
    }
    for (auto arg_ : int_arguments_) {
        if (arg_->values_.size() < arg_->min_args_count_) {
            return false;
        }
        if (!arg_->default_ && arg_->values_.size() == 0) {
            return false;
        }
    }
    for (auto arg_ : flag_arguments_) {
        if (arg_->values_.size() < arg_->min_args_count_) {
            return false;
        }
        if (!arg_->default_ && arg_->values_.size() == 0) {
            return false;
        }
    }
    return true;
}

// Parsing
bool AP::Parse(int argc, char** argv) {
    std::vector<std::string> mass;
    for (int i = 0; i < argc; i++) {
        mass.push_back(argv[i]);
    }
    return Parse(mass);
}

bool AP::Parse(std::vector<std::string> argv) {
    for (int i = 1; i < argv.size(); ++i) {
        auto arg = argv[i];
        bool is_added = false;
        if (having_help_argument_) {
            if (Find(arg, long_name_help_argument_) ||
                Find(arg, short_name_help_argument_)) {
                std::cout << HelpDescription();
                use_help_argument_ = true;
                return true;
            }
        }
        if (arg[0] == '-' || arg[1] == '-') {
            if (Find(arg, "=")) {
                std::string temp = arg.substr(arg.find("=") + 1);
                if (Find(arg, "--")) {
                    if (isInteger(temp)) {
                        for (auto arg_ : int_arguments_) {
                            if (Find(arg, arg_->long_name_)) {
                                arg_->values_.push_back(std::stoi(temp));
                                is_added = true;
                                break;
                            }
                        }
                    } else {
                        for (auto arg_ : string_arguments_) {
                            if (Find(arg, arg_->long_name_)) {
                                arg_->values_.push_back(temp);
                                is_added = true;
                                break;
                            }
                        }
                    }
                } else {
                    if (isInteger(temp)) {
                        for (auto arg_ : int_arguments_) {
                            if (Find(arg, arg_->short_name_)) {
                                arg_->values_.push_back(std::stoi(temp));
                                is_added = true;
                                break;
                            }
                        }
                    } else {
                        for (auto arg_ : string_arguments_) {
                            if (Find(arg, arg_->short_name_)) {
                                arg_->values_.push_back(temp);
                                is_added = true;
                                break;
                            }
                        }
                    }
                }
            } else {
                if (Find(arg, "--")) {
                    for (auto arg_ : flag_arguments_) {
                        if (Find(arg, arg_->long_name_)) {
                            arg_->values_.push_back(true);
                            is_added = true;
                            break;
                        }
                    }
                } else {
                    for (auto arg_ : flag_arguments_) {
                        if (Find(arg, arg_->short_name_)) {
                            arg_->values_.push_back(true);
                            is_added = true;
                        }
                    }
                }
            }
        } else {
            if (isInteger(arg)) {
                for (auto arg_ : int_arguments_) {
                    if (arg_->position_) {
                        arg_->values_.push_back(std::stoi(arg));
                        is_added = true;
                        break;
                    }
                }
            } else {
                for (auto arg_ : string_arguments_) {
                    if (arg_->position_) {
                        arg_->values_.push_back(arg);
                        is_added = true;
                        break;
                    }
                }
            }
        }
        if (!is_added) {
            std::cerr << "Argument " << arg << " not found." << '\n';
            return false;
        }
    }
    Pointer();
    return isCorrect();
}

// for String arguments
Argument<std::string>& AP::AddStringArgument(const std::string& long_name,
                                             const std::string& description) {
    Argument<std::string>* argument = new Argument<std::string>;
    argument->long_name_ = long_name;
    argument->description_ = description;
    string_arguments_.push_back(argument);
    return *argument;
}

Argument<std::string>& AP::AddStringArgument(const char short_name,
                                             const std::string& long_name,
                                             const std::string& description) {
    Argument<std::string>* argument = new Argument<std::string>;
    argument->short_name_ = short_name;
    argument->long_name_ = long_name;
    argument->description_ = description;
    string_arguments_.push_back(argument);
    return *argument;
}

std::string AP::GetStringValue(std::string arg_name, size_t index) const {
    for (auto arg : string_arguments_) {
        if (arg->long_name_ == arg_name || arg->short_name_ == arg_name) {
            if (arg->default_ && arg->values_.size() == 0) {
                return arg->default_value_;
            }
            return arg->values_[index];
        }
    }
    std::cerr << "Argument " << arg_name << " not found." << '\n';
    exit(1);
}

// for Int arguments
Argument<int>& AP::AddIntArgument(const std::string& long_name,
                                  const std::string& description) {
    Argument<int>* argument = new Argument<int>;
    argument->long_name_ = long_name;
    argument->description_ = description;
    int_arguments_.push_back(argument);
    return *argument;
}

Argument<int>& AP::AddIntArgument(const char short_name,
                                  const std::string& long_name,
                                  const std::string& description) {
    Argument<int>* argument = new Argument<int>;
    argument->short_name_ = short_name;
    argument->long_name_ = long_name;
    argument->description_ = description;
    int_arguments_.push_back(argument);
    return *argument;
}

int AP::GetIntValue(std::string arg_name, size_t index) const {
    for (auto arg : int_arguments_) {
        if (arg->long_name_ == arg_name || arg->short_name_ == arg_name) {
            if (arg->default_ && arg->values_.size() == 0) {
                return arg->default_value_;
            }
            return arg->values_[index];
        }
    }
    std::cerr << "Argument " << arg_name << " not found." << '\n';
    exit(1);
}

// for flag arguments
Argument<bool>& AP::AddFlag(const std::string& long_name,
                            const std::string& description) {
    Argument<bool>* argument = new Argument<bool>;
    argument->long_name_ = long_name;
    argument->description_ = description;
    flag_arguments_.push_back(&argument->Default(false));
    return *argument;
}

Argument<bool>& AP::AddFlag(const char short_name, const std::string& long_name,
                            const std::string& description) {
    Argument<bool>* argument = new Argument<bool>;
    argument->short_name_ = short_name;
    argument->long_name_ = long_name;
    argument->description_ = description;
    flag_arguments_.push_back(&argument->Default(false));
    return *argument;
}

bool AP::GetFlag(std::string arg_name, size_t index) const {
    for (auto arg : flag_arguments_) {
        if (arg->long_name_ == arg_name || arg->short_name_ == arg_name) {
            if (arg->default_ && arg->values_.size() == 0) {
                return arg->default_value_;
            }
            return arg->values_[index];
        }
    }
    std::cerr << "Argument " << arg_name << " not found." << '\n';
    exit(1);
}

// for help argument
void AP::AddHelp(const std::string& long_name, const std::string& description) {
    long_name_help_argument_ = long_name;
    description_help_argument_ = description;
    having_help_argument_ = true;
}

void AP::AddHelp(const char short_name, const std::string& long_name,
                 const std::string& description) {
    short_name_help_argument_ = short_name;
    long_name_help_argument_ = long_name;
    description_help_argument_ = description;
    having_help_argument_ = true;
}

const std::string AP::HelpDescription() const {
    std::string result;

    result += parser_name_ + "\n" + description_help_argument_ + "\n" + "\n";
    for (auto arg : string_arguments_) {
        if (arg->short_name_ != "") {
            result += "  -" + arg->short_name_ + ", ";
        }
        if (arg->long_name_ != "") {
            result += "--" + arg->long_name_ + ", ";
        }
        result += arg->description_ + " ";
        if (arg->multi_value_ || arg->position_ || arg->default_) {
            result += "[ ";
            if (arg->multi_value_) {
                result += "repeated, min args = " +
                          std::to_string(arg->min_args_count_) + " ";
            }
            if (arg->position_) {
                result += "position = " + std::to_string(arg->position_) + " ";
            }
            if (arg->default_) {
                result += "default = " + std::to_string(arg->default_) + " ";
            }
            result += "]";
        }
        result += "\n";
    }
    for (auto arg : int_arguments_) {
        if (arg->short_name_ != "") {
            result += "  -" + arg->short_name_ + ", ";
        }
        if (arg->long_name_ != "") {
            result += "--" + arg->long_name_ + ", ";
        }
        result += arg->description_ + " ";
        if (arg->multi_value_ || arg->position_ || arg->default_value_) {
            result += "[ ";
            if (arg->multi_value_) {
                result += "repeated, min args = " +
                          std::to_string(arg->min_args_count_) + " ";
            }
            if (arg->position_) {
                result += "position = " + std::to_string(arg->position_) + " ";
            }
            if (arg->default_value_) {
                result +=
                    "default = " + std::to_string(arg->default_value_) + " ";
            }
            result += "]";
        }
        result += "\n";
    }
    for (auto arg : flag_arguments_) {
        if (arg->short_name_ != "") {
            result += "  -" + arg->short_name_ + ", ";
        }
        if (arg->long_name_ != "") {
            result += "--" + arg->long_name_ + ", ";
        }
        result += arg->description_ + " ";
        if (arg->multi_value_ || arg->position_ || arg->default_value_) {
            result += "[ ";
            if (arg->multi_value_) {
                result += "repeated, min args = " +
                          std::to_string(arg->min_args_count_) + " ";
            }
            if (arg->position_) {
                result += "position = " + std::to_string(arg->position_) + " ";
            }
            if (arg->default_value_) {
                result +=
                    "default = " + std::to_string(arg->default_value_) + " ";
            }
            result += "]";
        }
        result += "\n";
    }
    result += "\n";
    result += short_name_help_argument_ + "  " + long_name_help_argument_ +
              " " + description_help_argument_ + "\n";
    return result;
}