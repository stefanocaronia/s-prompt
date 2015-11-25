#pragma once
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include "globals.h"

using namespace std;

typedef map<string,string> conf;

class Shell {

	public:

		bool RUNNING = true;

		Shell();
		Shell(string);
		~Shell();

		void run();

		void setCurrentPath(string);
		void setTitle(const char *);
		void setVersion(const char *);
		void setPROMPT(const char *);
		bool loadConfig();

	protected:
	private:

		string USERNAME;
		string COMPUTERNAME;

		string title = "$ Prompt";
		string version = "v1.0";
		string PROMPT = "$";
		const char* CONFIG_FILENAME = "sprompt.ini";

		conf config;

		concol TITLE_COLOR = yellow;
		concol PROMPT_COLOR = yellow;
		concol COMMAND_COLOR = white;
		concol OUTPUT_COLOR = gray;
		concol CUSTOM_COLOR = magenta;
		concol ERROR_COLOR = red;
		concol PATH_COLOR = dark_aqua;

		enum keys {
			KEY_UP = 72,
			KEY_DOWN = 80,
			KEY_LEFT = 75,
			KEY_RIGHT = 77,
			KEY_BACKSPACE = 8,
			KEY_TAB = 9
		};

		string input();
		int process(string);
		string ENV(string const &);
		string getCwd();
		string exec(const char*);
		void printHeader(bool);
		string getCurrentWord(string &);
		void setCurrentPathFromCommand(string);
		void refreshCSBI();
		void cursorLeft();
		void cursorRight();


		vector<string> files;
		vector<string> history;
		unsigned int history_pos = 0;

		HANDLE hConsole;
		COORD cursorPosition;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		unsigned int cursorPosInCommandLine;
		string commandLine;


		string currentPath;
		vector<string> currentPathFiles;

};

/* UTILITY */
vector<string> split(string, char);
vector<string> filesindir(string);
