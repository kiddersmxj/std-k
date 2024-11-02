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

int k::ExecCmdOrphan(std::string Cmd) {
    // Fork the process
    pid_t pid = fork();

    if (pid < 0) {
        // If fork failed
        std::cerr << "Fork failed." << std::endl;
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        // Parent process
        // Don't wait for child process, simply return to allow the main program to continue
        return EXIT_SUCCESS;
    }

    if (pid == 0) {
        // Child process
        
        // Create a new session, making the process an orphan
        pid_t sid = setsid();
        if (sid < 0) {
            std::cerr << "Failed to create a new session." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Execute the command using execvp
        // Tokenize the command
        std::string executable = "/bin/sh";
        std::string arg1 = "-c";
        const char *argv[] = { executable.c_str(), arg1.c_str(), Cmd.c_str(), nullptr };

        // Close standard file descriptors to detach from the terminal
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        execvp(argv[0], (char * const *)argv);

        // If execvp returns, it must have failed
        std::cerr << "Exec failed." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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

std::string k::config::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

std::unordered_map<std::string, std::string> k::config::parseConfigFile(const std::string& filePath) {
    std::unordered_map<std::string, std::string> config;
    std::ifstream file(filePath);
    std::string line;
    std::string currentSection;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    while (std::getline(file, line)) {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (line.front() == '[' && line.back() == ']') {
            currentSection = trim(line.substr(1, line.size() - 2));
        } else {
            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) {
                throw std::runtime_error("Invalid config line: " + line);
            }

            std::string key = trim(line.substr(0, delimiterPos));
            std::string value = trim(line.substr(delimiterPos + 1));

            if (!currentSection.empty()) {
                key = currentSection + "." + key;
            }

            config[key] = value;
        }
    }

    file.close();
    return config;
}

template <typename T>
T k::config::convertTo(const std::string& str) {
    std::istringstream iss(str);
    T value;
    if (!(iss >> value)) {
        throw std::runtime_error("Conversion error for value: " + str);
    }
    return value;
}

template <>
bool k::config::convertTo<bool>(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "true" || lowerStr == "1") return true;
    if (lowerStr == "false" || lowerStr == "0") return false;
    throw std::runtime_error("Invalid boolean value: " + str);
}

template <typename T>
T k::config::ConfigLoader::get(const std::string& key, T defaultValue) const {
    if (config.count(key)) {
        return convertTo<T>(config.at(key));
    }
    return defaultValue;
}

// TODO added colour printing functions

