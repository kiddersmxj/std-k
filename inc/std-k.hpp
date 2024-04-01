#ifndef Kstd
#define Kstd

#include <chrono>
#include <iostream>
#include <array>
#include <vector>

const std::string HOME = getenv("HOME");

namespace k {
    void BreakPoint();
    int ExecCmd(const std::string Cmd, std::string &Output);
    int ExecCmd(const std::string Cmd);
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
}

#endif
