#include <iostream>
#include <set>
#include <utility>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <errno.h>

#define MAXKEYS 104

using namespace std;

class LogFileProcessor
{
private:
  FILE *fp;
  int code[3], i;
  char *temp;
  set <char *> hkey;
  ENTRY ep, *search_result;
  struct code2keymap {
    const char *code;
    const char *keyname;
  };

public:
  LogFileProcessor(int argc, char **argv)
  {
    struct code2keymap c2k[] = {
      "0",  "[RESERVED]",
      "1",  "[ESC]",
      "2",  "1",
      "3",  "2",
      "4",  "3",
      "5",  "4",
      "6",  "5",                   
      "7",  "6",                   
      "8",  "7",                   
      "9",  "8",                   
      "10", "9",
      "11", "0",
      "12", "-",
      "13", "=",
      "14", "[BACKSPACE]",
      "15", "[TAB]",
      "16", "q",
      "17", "w",                   
      "18", "e",                   
      "19", "r",                   
      "20", "t",                   
      "21", "y",                   
      "22", "u",                   
      "23", "i",                   
      "24", "o",                   
      "25", "p",                   
      "26", "[",           
      "27", "]",          
      "28", "[ENTER]",               
      "29", "[LEFT_CTRL]",            
      "30", "a",                   
      "31", "s",                   
      "32", "d",                   
      "33", "f",                   
      "34", "g",                   
      "35", "h",                   
      "36", "j",                   
      "37", "k",                   
      "38", "l",                   
      "39", ";",           
      "40", "KEY_APOSTROPHE",          
      "41", "KEY_GRAVE",               
      "42", "[LEFT_SHIFT]",           
      "43", "\\",           
      "44", "z",                   
      "45", "x",                   
      "46", "c",                   
      "47", "v",                   
      "48", "b",                   
      "49", "n",                   
      "50", "m",                   
      "51", ",",               
      "52", ".",                 
      "53", "/",               
      "54", "[RIGHT_SHIFT]",          
      "55", "*",          
      "56", "[LEFT_ALT]",             
      "57", "[SPACE]",               
      "58", "[CAPS_LOCK]",            
      "59", "[F1]",                  
      "60", "[F2]",                  
      "61", "[F3]",                  
      "62", "[F4]",                  
      "63", "[F5]",                  
      "64", "[F6]",                  
      "65", "[F7]",                  
      "66", "[F8]",                  
      "67", "[F9]",                  
      "68", "[F10]",                 
      "69", "[NUMLOCK]",             
      "70", "[SCROLLLOCK]",          
      "71", "[KP7]",                 
      "72", "[KP8]",                 
      "73", "[KP9]",                 
      "74", "[KPMINUS]",             
      "75", "[KP4]",                 
      "76", "[KP5]",                 
      "77", "[KP6]",                 
      "78", "[KPPLUS]",              
      "79", "[KP1]",                 
      "80", "[KP2]",                 
      "81", "[KP3]",                 
      "82", "[KP0]",                 
      "83", "[KPDOT]",               
      "87", "[F11]",                 
      "88", "[F12]",                 
      "96", "[KPENTER]",             
      "97", "[RIGHT_CTRL]",           
      "98", "[KPSLASH]",             
      "99", "[SYSRQ]",               
      "100", "[RIGHTALT]",            
      "101", "[LINEFEED]",            
      "102", "[HOME]",                
      "103", "[UP]",                  
      "104", "[PAGEUP]",              
      "105", "[LEFT]",                
      "106", "[RIGHT]",               
      "107", "[END]",                 
      "108", "[DOWN]",                
      "109", "[PAGEDOWN]",            
      "110", "[INSERT]",              
      "111", "[DELETE]",              
      "117", "[KPEQUAL]",             
      "118", "[KPPLUSMINUS]",         
      "119", "[PAUSE]",               
    };

    if (argc != 2)
      {
	cerr << "The number of arguments < 2. Usage: " << argv[0] << " filename" << endl;
	exit(1);
      }

    if (!(fp = fopen(argv[1], "r")))
      {
	cerr << "FILE: " << strerror(errno) << endl;
	exit(1);
      }

    if (hcreate(MAXKEYS) == 0)
      {
	cerr << "HSEARCH: " << strerror(errno) << endl;
	exit(1);
      }

    for (i = 0; i < MAXKEYS; i++)
      {
	ep.key  = (char *)c2k[i].code;
	ep.data = (void *)c2k[i].keyname;
	if (hsearch(ep, ENTER) == NULL)
	  {
	    cerr << "HSEARCH: " << strerror(errno) << endl;
	    exit(1);
	  }
      }
  }

  void start_processing()
  {
    while(1)
      {
	if (fscanf(fp, "%d\n%d\n%d", &code[0], &code[1], &code[2]) < 0)
	  {
	    cerr << endl << "FSCANF: " << strerror(errno) << endl;
	    exit(1);
	  }

	temp = (char *)malloc(sizeof(int));
   	sprintf(temp, "%d", code[0]);
  	ep.key = temp;
	search_result = hsearch(ep, FIND); 
	free(temp);
	if (search_result)
	  {
	    if (code[1] == 1)
	      {
		cout << (char *)search_result->data;
		if (search_result->data == "[ENTER]" || search_result->data == "[KPENTER]")
		  cout << endl;
		continue;
	      }
	    if (code[1] == 2 || code[1] == 0)
	      {
		if (code[1] == 2)
		  {
		    pair <set <char *>::iterator,bool>::pair key_pair = hkey.insert(search_result->key);
		    set <char *>::iterator iter = key_pair.first;
		    bool added_to_hash = key_pair.second;
		    if (added_to_hash)
		      cout << "<";
		  }
		else
		  {
		    set <char *>::iterator pos = hkey.find(search_result->key);
		    if (pos != hkey.end())
		      {
			cout << ">";
			hkey.erase(pos);
		      }
		  }
		continue;
	      }
	  }
      }
  }

  ~LogFileProcessor()
  {
    hdestroy();
    fclose(fp);    
  }
};

int main(int argc, char **argv)
{
  LogFileProcessor *lp;
  lp = new LogFileProcessor(argc, argv);
  lp->start_processing();
  delete lp;
}
