

// read a line into buf, return -1 if failed
int read_line(int fd, char *buf); 
     
// write a line into buf, append '\n', return -1 if failed
int write_line(int fd, char *buf);