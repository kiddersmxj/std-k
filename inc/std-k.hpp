#ifndef Kstd
#define Kstd

#include <chrono>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cctype>

const std::string HOME = getenv("HOME");

namespace k {
    void BreakPoint();
    int ExecCmd(const std::string Cmd, std::string &Output);
    int ExecCmd(const std::string Cmd);
    int ExecCmdOrphan(const std::string Cmd);
    void WriteFileLines(std::vector<std::string> Lines, std::string File);
    std::vector<std::string> ReadFileLines(std::string File);
    void VPrint(std::vector<std::string> Input);
    std::vector<std::string> SplitStr(std::string String, const char* Delim);
    void Touch(std::string File);
    std::string StripTrailingNL(std::string Input);
    void SplitString(std::string &str, char delim, std::vector<std::string> &out, bool NoWhitespace);
    int VGetIndex(std::vector<std::string> v, std::string K);
    long Map(long x, long in_min, long in_max, long out_min, long out_max);
    float Average(std::vector<int> &v);
    int GetMax(std::vector<int> V);
    int GetMin(std::vector<int> V);
    std::string Space(std::vector<std::string> Values, int Width);
    bool IsInteger(std::string str);
    bool IsNegative(int Num);
    void ReplaceFirst(std::string &s ,std::string const &ToReplace, std::string const &ReplaceWith);
    bool Sleep(const long long Milliseconds);
    bool MkDir(std::string Path);
    void WriteFileLines(std::string Lines, std::string File);
	std::string RemoveWhitespace(std::string str);
	void WriteOnSameLine(std::string Line);


	typedef struct CurrentTime {
		int Year;
		int Month;
		int Day;
		int Hour;
		int Min;
		int Sec;
		std::string Date;
		std::string Time;
		CurrentTime() {
			// Get current time
			std::time_t currentTime = std::time(nullptr);
			// Convert to struct tm
			std::tm* timeinfo = std::localtime(&currentTime);

			Year = (timeinfo->tm_year+1900);
			Month = (timeinfo->tm_mon+1);
			Day = timeinfo->tm_mday;
			Hour = timeinfo->tm_hour;
			Min = timeinfo->tm_min;
			Sec = timeinfo->tm_sec;

			Date = std::to_string(Year) + "-" + std::to_string(Month) + "-" + std::to_string(Day);
			Time = std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);
		}
	} CurrentTime;

	typedef struct T {
		int Year;
		int Month;
		int Day;
		int Hour;
		int Min;
		int Sec;
		std::string Date;
		std::string Time;

		T(int year, int month, int day, int hour, int min, int sec) : Year(year), Month(month), Day(day), \
																	Hour(hour), Min(min), Sec(sec) {
			Date = std::to_string(Year) + "-" + std::to_string(Month) + "-" + std::to_string(Day);
			Time = std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);
		}

		T(int hour, int min, int sec) : Hour(hour), Min(min), Sec(sec) {
			Time = std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);
		}

		T(int year, int month, int day, bool isDate) : Year(year), Month(month), Day(day) {
			Date = std::to_string(Year) + "-" + std::to_string(Month) + "-" + std::to_string(Day);
		}


		T() {
			// Get current time
			std::time_t currentTime = std::time(nullptr);
			// Convert to struct tm
			std::tm* timeinfo = std::localtime(&currentTime);

			Year = (timeinfo->tm_year+1900);
			Month = (timeinfo->tm_mon+1);
			Day = timeinfo->tm_mday;
			Hour = timeinfo->tm_hour;
			Min = timeinfo->tm_min;
			Sec = timeinfo->tm_sec;

			Date = std::to_string(Year) + "-" + std::to_string(Month) + "-" + std::to_string(Day);
			Time = std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);
		}
	} T;
    
    class Time {
        public:
            Time();
            ~Time();
            double Close();
        private:
            bool Closed = 0;
			double milliseconds;
            std::chrono::system_clock::time_point Start;
            std::chrono::system_clock::time_point End;
    };

    namespace config {
        class Config {
        public:
            // Get the singleton instance
            static Config& getInstance();

            // Load configuration from a file
            bool load(const std::string& filename);

            // Retrieve a value with a given key path
            template<typename T>
            T get(const std::string& key, const T& default_value = T()) const;

            // Retrieve a required value with a given key path
            template<typename T>
            T get_required(const std::string& key) const;

            // Check if a key exists
            bool contains(const std::string& key) const;

        private:
            Config() = default; // Private constructor for singleton
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

            std::unordered_map<std::string, std::string> data;

            // Helper methods
            bool parseLine(const std::string& line, std::string& current_section);
        };

        // Implementation of template methods

        template<typename T>
        T Config::get(const std::string& key, const T& default_value) const {
            auto it = data.find(key);
            if (it == data.end())
                return default_value;

            std::istringstream iss(it->second);
            T value;
            if (!(iss >> std::boolalpha >> value)) {
                return default_value;
            }
            return value;
        }

        template<typename T>
        T Config::get_required(const std::string& key) const {
            auto it = data.find(key);
            if (it == data.end()) {
                throw std::runtime_error("Required configuration key not found: " + key);
            }

            std::istringstream iss(it->second);
            T value;
            if (!(iss >> std::boolalpha >> value)) {
                throw std::runtime_error("Invalid value for key: " + key);
            }
            return value;
        }

        // Specialization for std::string
        template<>
        inline std::string Config::get<std::string>(const std::string& key, const std::string& default_value) const {
            auto it = data.find(key);
            if (it == data.end())
                return default_value;

            std::string value = it->second;

            // Remove surrounding quotes if present
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            return value;
        }

        template<>
        inline std::string Config::get_required<std::string>(const std::string& key) const {
            auto it = data.find(key);
            if (it == data.end()) {
                throw std::runtime_error("Required configuration key not found: " + key);
            }

            std::string value = it->second;

            // Remove surrounding quotes if present
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            return value;
        }

        // Specialization for std::vector<std::string>
        template<>
        inline std::vector<std::string> Config::get<std::vector<std::string>>(const std::string& key, const std::vector<std::string>& default_value) const {
            auto it = data.find(key);
            if (it == data.end())
                return default_value;

            std::string value = it->second;

            // Remove surrounding brackets if present
            if (value.size() >= 2 && value.front() == '[' && value.back() == ']') {
                value = value.substr(1, value.size() - 2); // Remove [ and ]

                std::vector<std::string> result;
                std::string item;
                std::istringstream iss(value);

                while (std::getline(iss, item, ',')) {
                    // Trim whitespace and newlines
                    item.erase(0, item.find_first_not_of(" \t\n\r\""));
                    item.erase(item.find_last_not_of(" \t\n\r\"") + 1);

                    // Remove surrounding quotes if present
                    if (!item.empty() && item.front() == '"' && item.back() == '"') {
                        item = item.substr(1, item.size() - 2);
                    }

                    if (!item.empty()) {
                        result.push_back(item);
                    }
                }
                return result;
            }
            return default_value;
        }

        template<>
        inline std::vector<std::string> Config::get_required<std::vector<std::string>>(const std::string& key) const {
            auto it = data.find(key);
            if (it == data.end()) {
                throw std::runtime_error("Required configuration key not found: " + key);
            }

            std::string value = it->second;

            // Remove surrounding brackets if present
            if (value.size() >= 2 && value.front() == '[' && value.back() == ']') {
                value = value.substr(1, value.size() - 2); // Remove [ and ]

                std::vector<std::string> result;
                std::string item;
                std::istringstream iss(value);

                while (std::getline(iss, item, ',')) {
                    // Trim whitespace and newlines
                    item.erase(0, item.find_first_not_of(" \t\n\r\""));
                    item.erase(item.find_last_not_of(" \t\n\r\"") + 1);

                    // Remove surrounding quotes if present
                    if (!item.empty() && item.front() == '"' && item.back() == '"') {
                        item = item.substr(1, item.size() - 2);
                    }

                    if (!item.empty()) {
                        result.push_back(item);
                    }
                }
                return result;
            } else {
                throw std::runtime_error("Invalid format for key: " + key);
            }
        }

// Macros to initialize configuration variables
#define KCONFIG_VAR(varname, key_path, default_value) \
    varname = k::config::Config::getInstance().get<decltype(varname)>(key_path, default_value);

#define KCONFIG_VAR_REQUIRED(varname, key_path) \
    varname = k::config::Config::getInstance().get_required<decltype(varname)>(key_path);

    } // namespace config
}

#endif
