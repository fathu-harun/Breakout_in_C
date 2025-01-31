#ifndef SCORES_H
#define SCORES_H

#include <time.h>

typedef struct {
    char *name;
    int score;
    time_t timestamp;
    char* hash;
} entry;

#define NUM_SCORES 3

extern entry localentries[NUM_SCORES];

char * computeHash(entry *new_entry);
entry createEntry(char *name, int score);
void LoadDummyEntries(void);
entry *returnRecords(void);
entry* mockReturnRecords(void);
void freeEntryHash(entry *e);
void swap(entry *a, entry *b);
int compareScores(const void *a, const void *b);
void loadEntries(void);
void saveEntriesToFile(void);
void updateHighScores(entry *new_entry);

#endif
