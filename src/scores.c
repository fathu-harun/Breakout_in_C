#include <openssl/evp.h>
#include <openssl/sha.h>
#include "scores.h"
#include <string.h>
#include "constants.h"

int countlines(void) {
    char buffer[256];
    int count;
    FILE *fp ;
    
    fp= fopen("resources/game_load_files/highscores.txt", "r");
    if (fp == NULL) {
        printf("Could not open file\n");
        return 1;
    }
    
    count = 0;
    
  /*    Count the lines */
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }
    
   /*   Close the file */
    fclose(fp);
    return count;
}

char * computeHash(entry *new_entry) {
    int i;
    char *hash_str;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hash_len;
    /*  Initialize the context for SHA-256 hash */
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

   /*   Initialize the hash computation */
    EVP_DigestInit_ex(mdctx, md, NULL);

/*      Update the hash computation with name, score, and timestamp */
    EVP_DigestUpdate(mdctx, new_entry->name, strlen(new_entry->name));
    EVP_DigestUpdate(mdctx, &new_entry->score, sizeof(new_entry->score));
    EVP_DigestUpdate(mdctx, &new_entry->timestamp, sizeof(new_entry->timestamp));

   /*   Finalize the hash computation and store the result in a buffer */

    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

   /*   Allocate memory for the hash string (in hexadecimal representation) */
    hash_str = (char *)malloc((2 * SHA256_DIGEST_LENGTH + 1) * sizeof(char));/*  // +1 for null terminator */

/*      Convert each byte of the hash to hexadecimal and store it in hash_str */
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_str[i * 2], "%02x", hash[i]);
    }
    hash_str[2 * SHA256_DIGEST_LENGTH] = '\0'; /* // Null-terminate the string */

    EVP_MD_CTX_free(mdctx);

    return hash_str;
}

char *replaceSpacesWithUnderscores(const char *name) {
    int i;
    char *modified_name = strdup(name);
    if (modified_name == NULL) {
        /* Handle memory allocation failure */
        return NULL;
    }

    /* Replace spaces with underscores*/
    for (i = 0; modified_name[i] != '\0'; i++) {
        if (modified_name[i] == ' ') {
            modified_name[i] = '_';
        }
    }
    return modified_name;
}

entry createEntry(char *name, int score) {

    entry new_entry;
    char *modified_name ;

    if (name == NULL) {
        name = (char *)malloc(8 * sizeof(char));
        strcpy(name, "Unknown");
    }

    modified_name = replaceSpacesWithUnderscores(name);
    if (modified_name == NULL) {

        exit(EXIT_FAILURE);
    }

    new_entry.name = modified_name;
    new_entry.score = score;
    new_entry.timestamp = time(NULL);
    new_entry.hash = computeHash(&new_entry);
    return new_entry;
}

void LoadDummyEntries(void){
    char buffer[10];
    entry temp;
    int i;
    highscoresptr = fopen("resources/game_load_files/highscores.txt", "a+");
    if(highscoresptr==NULL){
        printf("Error in opening file");
        return;
    }
    else {printf("File opened successfully\n");}
    srand(time(NULL)); /*seeding the random function */
    for (i=0; i<3; i++){
        
        sprintf(buffer, "Player%d",1+rand()%100);
        temp = createEntry(buffer,10000+rand()%5000);
        fprintf(highscoresptr, "%s %d %ld %s\n", temp.name, temp.score, (long int)temp.timestamp, temp.hash);
    }

    fclose(highscoresptr);
}

void loadEntries(void) {

    int entryCount;
    entry temp;
    char line[512];

    /*char *hash_str;*/
    char name[256];
    char hash[HASH_SIZE];
    int score;
    time_t timestamp;

    highscoresptr= fopen("resources/game_load_files/highscores.txt", "r");

    if (highscoresptr == NULL) {
        printf("Error in opening file");
        return;
    } else {
        printf("File opened successfully returnRecords\n");
    }
    

    entryCount = 0;

    while (fgets(line, sizeof(line), highscoresptr)) {


        int result = sscanf(line, "%255s %d %ld %255s\n", name, &score, (long int *)&timestamp, hash);
        if (result != 4) {
            printf("line is %s\n",line);
            printf("Fix this in the future, we should do something here\n");
            continue;
        }

        
        temp.name = strdup(name);
        if (temp.name == NULL) {
            printf("Error: Memory allocation failed\n");
            fclose(highscoresptr);
            return;
        }

        temp.score = score;
        temp.timestamp = timestamp;

        temp.hash = strdup(hash);
        if (temp.hash == NULL) {
            printf("Error: Memory allocation failed\n");
            fclose(highscoresptr);
            free(temp.name);
            return;
        }

        
        /*hash_str = computeHash(&temp);  Assuming computeHash is defined somewhere */

/*       if (strcmp(hash_str, temp.hash) != 0) {
             printf("Hash verification failed for entry: %s\n", temp.name);
             free(temp.name);
             free(temp.hash);
            continue;
         } */
        if (entryCount < NUM_SCORES) {
            localentries[entryCount++] = temp;
        } else {
            printf("Localentries array is full, skipping additional entries\n");
            free(temp.name);
            free(temp.hash);
            break;
        }
    }
    fclose(highscoresptr);
}

entry *returnRecords(void) {
    FILE *highscoresptr;
    int entryCount;
    char line[512];
    entry *entries;
    char name[256];
    char hash[HASH_SIZE];
    char *hash_str;
    entry temp;        
    int score;
    time_t timestamp;
    int result;

    highscoresptr = fopen("resources/game_load_files/highscores.txt", "r");

    if (highscoresptr == NULL) {
        printf("Error in opening file");
        return NULL;
    } else {
        printf("File opened successfully returnRecords\n");
    }
    entries = malloc((countlines()+1) * sizeof(entry));
    if (entries == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(highscoresptr);
        return NULL;
    }

    entryCount = 0;

    while (fgets(line, sizeof(line), highscoresptr)) {
        
        result = sscanf(line, "%255s %d %ld %255s\n", name, &score, (long int *)&timestamp, hash);
        if (result != 4) {
            printf("Fix this in the future, we should do something here\n");
            continue;
        }

        temp.name = strdup(name);
        if (temp.name == NULL) {
            printf("Error: Memory allocation failed\n");
            fclose(highscoresptr);
            free(entries);
            return NULL;
        }

        temp.score = score;
        temp.timestamp = timestamp;

        temp.hash = strdup(hash);
        if (temp.hash == NULL) {
            printf("Error: Memory allocation failed\n");
            fclose(highscoresptr);
            free(temp.name);
            free(entries);
            return NULL;
        }

        
        hash_str= computeHash(&temp);

        if (strcmp(hash_str, temp.hash) != 0) {
            printf("Hash verification failed for entry: %s\n", temp.name);
            free(temp.name);
            free(temp.hash);
            continue;
        }

        entries[entryCount++] = temp;
    }

    entries[entryCount].name =NULL;
    entries[entryCount].score = 0;
    entries[entryCount].timestamp = 0;
    entries[entryCount].hash = NULL;


    fclose(highscoresptr);

    return entries;
}

void freeEntryHash(entry *e) {
    free(e->hash);
}

void swap(entry *a, entry *b) {
    entry temp = *a;
    *a = *b;
    *b = temp;
}

/*  Comparison function for qsort */
int compareScores(const void *a, const void *b) {
    entry *entryA = (entry *)a;
    entry *entryB = (entry *)b;
    return entryB->score - entryA->score;
}

void saveEntriesToFile(void) {
    int i;
    highscoresptr = fopen("resources/game_load_files/highscores.txt", "w");

    if (highscoresptr == NULL) {
        printf("Error in opening file");
        return;
    } else {
        printf("File opened successfully saveEntriesToFile\n");
    }

    qsort(localentries, NUM_SCORES, sizeof(entry), compareScores);

    for (i = 0; i < NUM_SCORES; i++) {
        if (localentries[i].name != NULL) {
            fprintf(highscoresptr, "%s %d %ld %s\n", localentries[i].name, localentries[i].score, (long int)localentries[i].timestamp, localentries[i].hash);
        }
    }

    fclose(highscoresptr);
}

void updateHighScores(entry *new_entry) {
    int i;
    entry temp[NUM_SCORES+1];
    for ( i = 0; i < NUM_SCORES; i++) {
        temp[i] = localentries[i];
    }
    temp[NUM_SCORES] = *new_entry;
    /* Add the new entry to the local entries*/
    qsort(temp, NUM_SCORES + 1, sizeof(entry), compareScores);
    for (i = 0; i < NUM_SCORES; i++) {
        localentries[i] = temp[i];
    }
}
