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
    int chunks;
    int port;
}File;

void updatePeerList(char **array, int count, const char *s);

void updateFileList(File f, int uploads, File* array);

File initializeFile(char filename, int chunks_num, int port_num);

void freeFileArray(File* array);

void freePeerArray(char** array, int index);

#endif /* tracker_h */
