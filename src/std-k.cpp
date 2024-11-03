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

k::config::Config& k::config::Config::getInstance() {
    static Config instance;
    return instance;
}

bool k::config::Config::load(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open())
        return false;

    std::string line;
    std::string current_section;
    std::string pending_key;
    std::string pending_value;
    bool is_multiline_array = false;

    while (std::getline(infile, line)) {
        // Remove comments
        auto comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
            line = line.substr(0, comment_pos);

        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty())
            continue;

        // Handle sections
        if (!is_multiline_array && line.front() == '[' && line.back() == ']') {
            current_section = line.substr(1, line.size() - 2);
            continue;
        }

        // Handle multi-line array
        if (is_multiline_array) {
            pending_value += line + "\n"; // Preserve newlines
            if (line.find(']') != std::string::npos) {
                // End of multi-line array
                is_multiline_array = false;
                std::string full_key = current_section.empty() ? pending_key : current_section + "." + pending_key;
                data[full_key] = pending_value;
                pending_key.clear();
                pending_value.clear();
            }
            continue;
        }

        // Parse key-value pair
        auto equal_pos = line.find('=');
        if (equal_pos == std::string::npos) {
            // Invalid line, skip
            continue;
        }

        std::string key = line.substr(0, equal_pos);
        std::string value_str = line.substr(equal_pos + 1);

        // Trim whitespace from key and value
        key.erase(0, key.find_first_not_of(" \t\n\r"));
        key.erase(key.find_last_not_of(" \t\n\r") + 1);

        value_str.erase(0, value_str.find_first_not_of(" \t\n\r"));
        value_str.erase(value_str.find_last_not_of(" \t\n\r") + 1);

        // Check if value starts a multi-line array
        if (value_str.front() == '[' && value_str.back() != ']') {
            is_multiline_array = true;
            pending_key = key;
            pending_value = value_str + "\n";
        } else {
            // Single-line value
            std::string full_key = current_section.empty() ? key : current_section + "." + key;
            data[full_key] = value_str;
        }
    }

    return true;
}

bool k::config::Config::contains(const std::string& key) const {
    return data.find(key) != data.end();
}

// TODO added colour printing functions

