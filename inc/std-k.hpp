#ifndef Kstd
#define Kstd

#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>

const std::string HOME = getenv("HOME");

namespace k {
    void BreakPoint();
    std::string ExecCmd(const std::string cmd, const int Output, int ExitStatus);
    void WriteFileLines(std::vector<std::string> Lines, std::string File);
    std::vector<std::string> ReadFileLines(std::string File);
    void VPrint(std::vector<std::string> Input);
    std::vector<std::string> SplitStr(std::string String, const char* Delim);
    void Touch(std::string File);
    std::string StripTrailingNL(std::string Input);
    void SplitString(std::string &str, char delim, std::vector<std::string> &out);
    int VGetIndex(std::vector<std::string> v, std::string K);
    int Map(int x, int in_min, int in_max, int out_min, int out_max);
}

#endif
