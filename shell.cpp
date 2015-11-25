#include <unistd.h>
#include <cstdio>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <dirent.h>
#include <windows.h>
#include <iostream>

#include "globals.h"
#include "colors.h"
#include "shell.h"

using namespace std;

Shell::Shell() {
	USERNAME = ENV("USERNAME");
	COMPUTERNAME = ENV("COMPUTERNAME");
	SetConsoleTitle("$ Prompt");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

Shell::Shell(string pa) {
	setCurrentPath(pa);
	USERNAME = ENV("USERNAME");
	COMPUTERNAME = ENV("COMPUTERNAME");
}

Shell::~Shell() {
	//dtor
}

void Shell::run() {

	bool show_userandpath = true;
	string commandLine = "";
	int result = 0;

	printHeader(true);

	while (RUNNING) {
		currentPath = getCwd();
		currentPathFiles = filesindir(currentPath);
		if (show_userandpath)
			cout << dark_green << USERNAME << "@" << COMPUTERNAME << " " << PATH_COLOR << currentPath << endl;

		cout << PROMPT_COLOR << PROMPT << " " << COMMAND_COLOR;

		commandLine = input();

		cout << "\r\n";

		if (commandLine.length()>0) {
			result = process(commandLine);
			if (result>0) cout << ERROR_COLOR << "ERROR " << result << endl;
			cout << endl;
			show_userandpath=true;
		} else {
			show_userandpath=false;
		}
	}
}

string Shell::input() {
	commandLine = "";
	string currentWord = "";
	cursorPosInCommandLine = 0;
	string currentWordCompl = "";
	char ch;

	while ( ((ch=_getch())!='\r') ) {

		refreshCSBI();

		switch (ch) {
			case KEY_UP:
				for (unsigned int c = 0; c < commandLine.length(); c++) cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
				commandLine = history[history_pos];
				cout << commandLine;
				if (history_pos>0) history_pos--;
				cursorPosInCommandLine = commandLine.length();
				break;
			case KEY_DOWN:
				for (unsigned int c = 0; c < commandLine.length(); c++) cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
				commandLine = history[history_pos];
				cout << commandLine;
				if (history_pos<history.size()-1) history_pos++;
				cursorPosInCommandLine = commandLine.length();
				break;
			/*case KEY_LEFT:
				cursorLeft();
				break;*/
			case -32:
				if (_getch()==KEY_LEFT) cursorLeft();
				if (_getch()==KEY_RIGHT) cursorRight();
				break;
			case KEY_RIGHT:
				cursorRight();
				break;
			case KEY_BACKSPACE:
				if (commandLine.size()>0) {
					cout << ch << " " << ch;
					commandLine.erase(commandLine.size() - 1);
				}
				cursorPosInCommandLine = commandLine.length();
				break;
			case KEY_TAB:
				currentWord = getCurrentWord(commandLine);
				for (string fi : currentPathFiles) {
					if (fi.substr(0,currentWord.size())==currentWord) {
						currentWordCompl=fi.substr(currentWord.size());
						cout << currentWordCompl;
						commandLine+=currentWordCompl;
						break;
					}
				}
				cursorPosInCommandLine = commandLine.length();
				break;
			default:
				if (cursorPosInCommandLine == commandLine.length()) {
					commandLine+=ch;
					cout << ch;
				}
				cursorPosInCommandLine = commandLine.length();
		}
	}

	return commandLine;
}

int Shell::process(string cline) {
	int result = 0;
	vector<string> cparts;
	cparts = split(cline, ' ');
	string command = cparts[0];

	cout << OUTPUT_COLOR;

	history.push_back(cline);
	history_pos = history.size()-1;

	if (command=="cd" && cparts.size()>1) {
		setCurrentPathFromCommand(cparts[1]);
	} else if (command=="cls") {
		result = system(cline.c_str());
		printHeader(false);
	} else if (command=="ciao") {
		cout << CUSTOM_COLOR << "Cazzo vuoi?" << endl;
	} else if (command=="exit") {
		RUNNING = false;
	} else {
		result = system(cline.c_str());
	}

	return result;
}

string Shell::ENV(string const & key) {
	char * val = getenv(key.c_str());
	return val == NULL?string(""):string(val);
}

string Shell::getCwd() {
	char buff[PATH_MAX];
	getcwd(buff, PATH_MAX);
	string c(buff);
	return c;
}

string Shell::exec(const char* cmd) {
	shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if (!pipe) return "ERROR";
	char buffer[128];
	string result = "";
	while (!feof(pipe.get())) {
		if (fgets(buffer, 128, pipe.get()) != NULL)
			result += buffer;
	}
	return result;
}

void Shell::printHeader(bool with_endl = true) {
	cout << TITLE_COLOR << "Welcome to " << title << " " << version << " by s.caronia" << endl;
	string ver;
	ver = exec("@ver").erase(0,1);
	cout << dark_white << ver;
	if (with_endl) cout << endl;
}

string Shell::getCurrentWord(string & cline) {
	std::size_t found = cline.find_last_of(" ");
	return cline.substr(found+1);
}

void Shell::setCurrentPath(string pa) {
	currentPath = pa;
	SetCurrentDirectory(currentPath.c_str());
	string ti = title + " - " + currentPath;
	SetConsoleTitle(ti.c_str());
}

void Shell::setCurrentPathFromCommand(string arg) {
	SetCurrentDirectory(arg.c_str());
	currentPath = getCwd();
	string ti = title + " - " + currentPath;
	SetConsoleTitle(ti.c_str());
}

void Shell::setTitle(const char * s) {
	title = string(s);
}

void Shell::setVersion(const char * s) {
	version = string(s);
}

void Shell::setPROMPT(const char * ti) {
	PROMPT = string(ti);
}

bool Shell::loadConfig() {
	ifstream cfile;
	cfile.open(CONFIG_FILENAME);

	if (!cfile.is_open()) return false;

	string key,value,line,f;

	while(getline(cfile, line)) {

		if (line.size()==0) continue;
        f=line.substr(0,1);
        if (f=="#"||f=="/"||f=="["||f=="*") continue;
        vector<string> parts = split(line,'=');
        if (parts.size()>1) config[parts[0]]=parts[1];
    }

	cfile.close();

	try {
		if (config["TITLE_COLOR"]!="") 	TITLE_COLOR = (concol)stoi(config["TITLE_COLOR"]);
		if (config["PROMPT_COLOR"]!="")	PROMPT_COLOR = (concol)stoi(config["PROMPT_COLOR"]);
		if (config["COMMAND_COLOR"]!="")COMMAND_COLOR = (concol)stoi(config["COMMAND_COLOR"]);
		if (config["OUTPUT_COLOR"]!="")	OUTPUT_COLOR = (concol)stoi(config["OUTPUT_COLOR"]);
		if (config["CUSTOM_COLOR"]!="")	CUSTOM_COLOR = (concol)stoi(config["CUSTOM_COLOR"]);
		if (config["ERROR_COLOR"]!="")	ERROR_COLOR = (concol)stoi(config["ERROR_COLOR"]);
		if (config["PATH_COLOR"]!="")	PATH_COLOR = (concol)stoi(config["PATH_COLOR"]);

		if (config["PROMPT"]!="")	PROMPT = config["PROMPT"];

	} catch (...) {
		return false;
	}

	return true;
}

void Shell::refreshCSBI() {
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	cursorPosition = csbi.dwCursorPosition;
}

void Shell::cursorLeft() {
	if (cursorPosInCommandLine==0) return;
	COORD newCoord;
	newCoord.Y=csbi.dwCursorPosition.Y;
	newCoord.X=csbi.dwCursorPosition.X-1;
	if ((cursorPosInCommandLine+PROMPT.length())%csbi.dwMaximumWindowSize.X==1) newCoord.Y--;
	cursorPosInCommandLine--;
	SetConsoleCursorPosition(hConsole,newCoord);

}

void Shell::cursorRight() {
	if (cursorPosInCommandLine==commandLine.length()) return;
	COORD newCoord;
	newCoord.Y=csbi.dwCursorPosition.Y;
	newCoord.X=csbi.dwCursorPosition.X+1;
	SetConsoleCursorPosition(hConsole,newCoord);
	cursorPosInCommandLine++;
}

/* UTILITY FUNCTIONS */

vector<string> split(string text, char delim) {
	vector<string> elements;
	stringstream stream(text);
	string item;
	while (getline(stream, item, delim)) {
		item.erase(0, item.find_first_not_of(" \n\r\t"));
		item.erase(item.find_last_not_of(" \n\r\t")+1);
		elements.push_back(item);
	}
	return elements;
}

vector<string> filesindir(string path) {
	vector<string> files;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			files.push_back(ent->d_name);
		}
		closedir (dir);
	}
	return files;
}
