#include "../inc/std-k.hpp"

#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <math.h>
#include <thread>
#include <filesystem>
#include <sys/stat.h>
#include <sys/wait.h>

// NULL function to attach breakpoint to in gdb
void k::BreakPoint(void) {
    std::cout << "";
}

int k::ExecCmd(const std::string Cmd, std::string &Output) {
    int ExitStatus = 0;
	auto pPipe = ::popen(Cmd.c_str(), "r");
	if(pPipe == nullptr) throw std::runtime_error("Cannot open pipe");	

    std::array<char, 256> buffer;
	while(not feof(pPipe)) {
	    auto bytes = fread(buffer.data(), 1, buffer.size(), pPipe);
	    Output.append(buffer.data(), bytes);
	}

	auto rc = ::pclose(pPipe);
    if(WIFEXITED(rc))
	    ExitStatus = WEXITSTATUS(rc);

	return ExitStatus;
}

int k::ExecCmd(const std::string Cmd) {
    int ExitStatus = 0;
	auto pPipe = ::popen(Cmd.c_str(), "r");
	if(pPipe == nullptr) throw std::runtime_error("Cannot open pipe");	
	auto rc = ::pclose(pPipe);
    if(WIFEXITED(rc))
	    ExitStatus = WEXITSTATUS(rc);

	return ExitStatus;
}

int ExecCmdOrphan(const std::string Cmd) {
    pid_t pid = fork();
    if (pid < 0) { // If the fork failed
        throw std::runtime_error("Fork failed");
    } else if (pid == 0) { // Child process
        // Redirect stdout and stderr to /dev/null
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "w", stderr);
        execl("/bin/sh", "sh", "-c", Cmd.c_str(), (char*)nullptr);
        // If execl returns, it must have failed.
        exit(127); // Indicate error
    } else { // Parent process
        // Wait for child process to complete
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            status = -1; // waitpid() failed
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}

void k::VPrint(std::vector<std::string> Input) {
	for(std::string Output: Input)
	    std::cout << Output << std::endl;
}

void k::WriteFileLines(std::vector<std::string> Lines, std::string File) {
    std::fstream FileToWrite(File, std::ios::out);
	if (!FileToWrite) {
        std::cout << "File not created!" << std::endl;
	}
	else {
        for(std::string Line: Lines)
            FileToWrite << Line << std::endl;
		FileToWrite.close();
	}
}

std::vector<std::string> k::ReadFileLines(std::string File) {
    std::string Line;
    std::vector<std::string> Output;
    std::ifstream FileToRead(File);

    while(getline(FileToRead, Line)) {
        Output.push_back(Line);
    }

    FileToRead.close();
    return Output;
}

void k::Touch(std::string File) {
    std::fstream fs;
    fs.open(File, std::ios::out);
    fs.close();
}

std::string k::StripTrailingNL(std::string Input) {
    Input.erase(std::remove(Input.begin(), Input.end(), '\n'), Input.cend());
    return Input;
}

void k::SplitString(std::string &str, char delim, std::vector<std::string> &out, bool NoWhitespace) { 
    // construct a stream from the string 
    std::stringstream ss(str); 
 
    std::string s; 
    while(std::getline(ss, s, delim)) { 
        if(NoWhitespace)
            out.push_back(RemoveWhitespace(s)); 
        else
            out.push_back(s);
    } 
} 

int k::VGetIndex(std::vector<std::string> v, std::string K) {
    auto it = find(v.begin(), v.end(), K);
    // If element was found
    if (it != v.end()) {
        // calculating the index
        // of K
        int index = it - v.begin();
        return index;
    } else {
        // If the element is not
        // present in the vector
        return -1;
    }
}

long k::Map(long x, long in_min, long in_max, long out_min, long out_max) {
    if(((in_max - in_min) + out_min) == 0)
        return 0;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float k::Average(std::vector<int> &v){
    if(v.empty()) {
        return 0;
    }
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

int k::GetMax(std::vector<int> V) {
    const int Max = *max_element(std::begin(V), std::end(V));
    return Max;
}

int k::GetMin(std::vector<int> V) {
    const int Min = *min_element(std::begin(V), std::end(V));
    return Min;
}

std::string k::Space(std::vector<std::string> Values, int Width) {
    std::string Str;
    int Space = floor(Width/Values.size());
    for(std::string s: Values) {
        if(s.length() > Space)
            return "too small width";
        int Rem = Space - s.length();
        if(Rem % 2 == 0) {
            for(int i=0; i<Rem/2; i++)
                Str = Str + " ";
            Str = Str + s;
            for(int i=0; i<Rem/2; i++)
                Str = Str + " ";
        } else {
            for(int i=0; i<floor(Rem/2); i++)
                Str = Str + " ";
            Str = Str + s;
            for(int i=0; i<floor(Rem/2)+1; i++)
                Str = Str + " ";
        }
    }
    return Str;
}

bool k::IsInteger(std::string str) {
    bool rtn = !str.empty() && str.find_first_not_of("0123456789-.") == std::string::npos;
    return rtn;
}

bool k::IsNegative(int Num) {
    bool Flag;
    if(Num == 0)
            return 0; else (Num > 0) ? Flag = 0: Flag = 1;
    return Flag;
}

void k::ReplaceFirst(std::string &s ,std::string const &ToReplace, std::string const &ReplaceWith) {
    std::size_t pos = s.find(ToReplace);
    if (pos == std::string::npos) return;
        s.replace(pos, ToReplace.length(), ReplaceWith);
}

bool k::Sleep(const long long Milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
    return true;
}

bool k::MkDir(std::string Path) {
    const char *P = Path.c_str();
    if(mkdir(P, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
        return 1;
    return 0;
}

void k::WriteFileLines(std::string Lines, std::string File) {
    std::fstream FileToWrite(File, std::ios::out);
	if (!FileToWrite) {
        std::cout << "File not created!" << std::endl;
	}
	else {
        FileToWrite << Lines << std::endl;
		FileToWrite.close();
	}
}

k::Time::Time() {
    Start = std::chrono::system_clock::now();
}

k::Time::~Time() {
    if(!Closed) {
        Close();
        std::cout << "Exec: " << milliseconds << "ms" << std::endl;
    } else Close();
}

double k::Time::Close() {
    End = std::chrono::system_clock::now();
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count();
    Closed = 1;
    return milliseconds;
}

std::string k::RemoveWhitespace(std::string str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), \
          str.end());
    return str;
}

void k::WriteOnSameLine(std::string Line) {
    std::cout << "\r" << Line << std::flush;
}

// TODO added colour printing functions

