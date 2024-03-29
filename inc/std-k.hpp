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
