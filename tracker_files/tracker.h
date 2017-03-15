//
//  tracker.h
//  
//  Header file for tracker.c
//


#ifndef tracker_h
#define tracker_h

void updateList(char **array, int count, const char *s);

void extract_port(const char *string, int index, char **array);

void extract_name(const char *string, int index, char **array);

char* encode_list(char **array, int index);

void freeArray(char*** array, int index);

char* encode_int(char* x);

char* encode_str(char* x);

#endif /* tracker_h */
