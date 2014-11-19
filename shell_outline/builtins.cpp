#include "builtins.h"

using namespace std;

stack<string> *dirStack = new stack<string>();

int com_ls(vector<string>& tokens) {
  if (tokens.size() < 2) {
    tokens.push_back(".");
  }

  DIR* dir = opendir(tokens[1].c_str());
  
  if (!dir) {
    perror("There are some errors in ls: ");

    return 1;
  }
  
  for (dirent* iter = readdir(dir); iter; iter = readdir(dir)) {
    cout << iter->d_name << endl;
  }

  return 0;
}


int com_cd(vector<string>& tokens) {
  //-----------------------------
    if (IF_DEBUG) {
		d_printf("For every token it's in cd: ");
		for (int i=0; i<tokens.size(); i++) {
			d_printf("%s ", tokens[i].c_str());
		}
		d_printf("\n");
	}
    if (tokens.size() == 2) {
        if (tokens[1][0] == '/') {
			d_printf("changing path to: %s\n", tokens[1].c_str());
			d_printf("pushing pwd into the dir stack");
			dirStack->push(pwd());
            int retValue = chdir(tokens[1].c_str());
			d_printf("return value of change dir: %d\n", retValue);
			if (retValue != NORMAL_EXIT) {
				perror("cd");
			}
            return retValue;
        }
		else if (tokens[1][0] == '-') {
			d_printf("changing to the last dir\n");
			int retValue = chdir(dirStack->top().c_str());
			dirStack->pop();
			if (retValue != NORMAL_EXIT) {
				perror("cd");
			}
			return retValue;
		}
        else {
            string cwDir = getcwd(NULL, 0);
            cwDir += "/";            
            cwDir += tokens[1];           
            if (!opendir(cwDir.c_str())) {
                perror("cd");
                return BAD_FILE_OR_DIR;
            }
            else {
                chdir(cwDir.c_str());
                return NORMAL_EXIT;
            }
        }
    }
	else if (tokens.size() == 1) {
		d_printf("pushing pwDir into the dir stack\n");
		dirStack->push(pwd());
		chdir(getenv("HOME"));
		return NORMAL_EXIT;
	}
    else {
        perror("invalid argument in cd");
        return INVALID_ARGUMENTS;
    }

  //----------------------------- 
  cout << "cd called" << endl; 
  return 0;
}


int com_pwd(vector<string>& tokens) {
  printf("%s\n", pwd().c_str());
  return NORMAL_EXIT;  
  cout << "pwd called" << endl; 
  return 0;
}


int com_alias(vector<string>& tokens) {  
  cout << "alias called" << endl; 
  return 0;
}


int com_unalias(vector<string>& tokens) {
  cout << "unalias called" << endl; 
  return 0;
}


int com_echo(vector<string>& tokens) {  
    if (tokens.size() == 1) {
	printf("\n");
	return NORMAL_EXIT;
    }
	else {		
		for (int i = 1; i < tokens.size(); ++i) {
			string tempToken = tokens[i];
			if (i == tokens.size() - 1) {
				printf("%s\n", tokens[i].c_str());
				return NORMAL_EXIT;
			}
			else {
				printf("%s ", tokens[i].c_str());				
			}
		}
	}	
	return ABNORMAL_EXEC;


  //--------------------------
  cout << "echo called" << endl; 
  return 0;
}


int com_exit(vector<string>& tokens) {
  exit(0);
  return 0;
}


int com_history(vector<string>& tokens) {
	d_printf("history commands:\n");
	if (history_length == 0) {
		return NORMAL_EXIT;
	}
	d_printf("lack of history\n");
    if (tokens.size() > 2) {
		perror("history");
		return TOO_MANY_ARGUMENTS;
	}
	else if (tokens.size() == 2) {
		d_printf("There are too many arguments.\n");
		if (tokens[1][0] == '-') {
			cerr << "Negative arguments are not allowed in history\n";
			return INVALID_ARGUMENTS;
		}

		print_last_amount_history(atoi(tokens[1].c_str()));
	}
	else if (tokens.size() == 1) {
		d_printf("too many arguments: only one token\n");
		print_last_amount_history(history_length);
	}
	else {
		d_printf("Invalid array size.\n");
		perror("history");
		return ABNORMAL_EXEC;
	}
    return NORMAL_EXIT;
}


void print_last_amount_history(int num) {	
	if (num > history_length) {
		num = history_length;
	}
	HIST_ENTRY *tempHistoryEntry = NULL;
	d_printf("Temp entry created\n");
	for (int i = history_length - num; i <= history_length; i++) {
		tempHistoryEntry = history_get(i);
		d_printf("\nGot a new history element at: %d\n", i);
		if (tempHistoryEntry == NULL) {
			d_printf("Element was null!\n");			
			continue;
		}
		else {
			printf("   %d  %s\n", i, tempHistoryEntry->line);
		}
	}
}

string pwd() {  
  return getcwd(NULL, 0);
  //--------
  return NULL;
}

string user() {
	return getenv("USER");
}

string last_command_status(int code) {
	stringstream ss;
	ss << code;
	return ss.str();
}




