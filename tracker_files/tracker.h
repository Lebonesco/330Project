//
//  tracker.h
//  
//  Header file for tracker.c
//


#ifndef tracker_h
#define tracker_h

void updateTotalList(char **array, int count, const char *s);

void updatePortList(char **array, int count, const char *s);

void updateNameList(char **array, int count, const char *s);

void freeArray(char*** array, int index);

char* encode_int(char* x);

char* encode_str(char* x);

#endif /* tracker_h */
