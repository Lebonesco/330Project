//
//  tracker.h
//  
//
//  Created by Elijah Caluya on 3/9/17.
//
//

#ifndef tracker_h
#define tracker_h

typedef struct {
    char file_name[100];
    char port[4];
}File;

void updateTotalList(char **array, int count, const char *s);

void updatePortList(char **array, int count, const char *s);

void updateNameList(char **array, int count, const char *s);

void freeArray(char** array, int index);

char* encode_int(char* x);

char* encode_str(char* x);

#endif /* tracker_h */
