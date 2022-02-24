/*

   Word Count using dedicated lists

*/

/*
   Copyright © 2019 University of California, Berkeley

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount *word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64

void myFree(char *str) {
    if (str != NULL) {
        free(str);
    }
}
/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 * Useful functions: fgetc(), isalpha().
 */
int num_words(FILE* infile) {
    int num_words = 0;
    char c;
    if(infile==NULL)
    {
        printf("error\n");
        return 1;

    }

    char big[4096];
    while(fgets(big, 4096,infile) != NULL) {
        char *tok = malloc(sizeof (char) * 4096);
        char *origPtr = tok;

        /* get the first tok */
        tok = strtok(big, " ");

        /* walk through other tokens */
        while( tok != NULL ) {
            if (tok[0] == '\n')
                break;
            num_words++;
            tok = strtok(NULL, " ");
        }

        myFree(origPtr);
    }

    return num_words;
}

/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 * Useful functions: fgetc(), isalpha(), tolower(), add_word().
 */
void count_words(WordCount **wclist, FILE *infile) {
    char big[4096];
    fseek(infile,0,SEEK_SET);
    while(fgets(big,4096,infile)!=NULL){
        char *tok =malloc(sizeof(char)*4096);
        char *origptr=tok;
        tok=strtok(big, " ");
        while(tok!=NULL){
            if(tok[0]=='\n')
                break;
            char *arr = malloc(sizeof(char)*4096);

            int j = 0;
            for(int i=0;i<strlen(tok);i++){
                if(isalpha(tok[i])!=0){
                    arr[j++] = tolower(tok[i]);
                }
            }

            arr[j] = NULL;

            /* printf( "arr= %s, tok = %s\n", arr, tok); */
            add_word(wclist,arr);
            free(arr);
            tok=strtok(NULL," ");
        }
        free(origptr);
    }

}

/*
 * Comparator to sort list by frequency.
 * Useful function: strcmp().
 */

static bool wordcount_less(const WordCount *wc1, const WordCount *wc2) {
    if( wc1->count==wc2->count)
        return  strcmp(wc1->word,wc2->word)<0;
    return wc1->count<wc2->count ;
}

// In trying times, displays a helpful message.
static int display_help(void) {
    printf("Flags:\n"
            "--count (-c): Count the total amount of words in the file, or STDIN if a file is not specified. This is default behavior if no flag is specified.\n"
            "--frequency (-f): Count the frequency of each word in the file, or STDIN if a file is not specified.\n"
            "--help (-h): Displays this help message.\n");
    return 0;
}

/*
 * Handle command line flags and arguments.
 */
int main (int argc, char *argv[]) {

    // Count Mode (default): outputs the total amount of words counted
    bool count_mode = false;
    int total_words = 0;

    // Freq Mode: outputs the frequency of each word
    bool freq_mode = true;

    FILE *infile = NULL;

    // Variables for command line argument parsing
    int i;
    static struct option long_options[] =
    {
        {"count", no_argument, 0, 'c'},
        {"frequency", no_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    // Sets flags
    while ((i = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
        switch (i) {
            case 'c':
                count_mode = true;
                freq_mode = false;
                break;
            case 'f':
                count_mode = false;
                freq_mode = true;
                break;
            case 'h':
                return display_help();
        }
    }

    if (!count_mode && !freq_mode) {
        printf("Please specify a mode.\n");
        return display_help();
    }

    /* Create the empty data structure */
    init_words(&word_counts);
    WordCount **my =&word_counts;

    if ((argc - optind) < 1) {
        // No input file specified, instead, read from STDIN instead.
        infile = stdin;
    } else {
        FILE *file[argc-1];
        // At least one file specified. Useful functions: fopen(), fclose().
        // The first file can be found at argv[optind]. The last file can be
        // found at argv[argc-1].
        for(int i=1;i<argc;i++){
            file[i]=fopen(argv[i],"r");
            int t=num_words(file[i]);
            count_words(my,file[i]);
            total_words =t+total_words;
            fclose(file[i]);
        }
    }

    if (count_mode) {
        printf("The total number of words is: %i\n", total_words);
    } else {
        wordcount_sort(&word_counts, wordcount_less);

        printf("The frequencies of each word are: \n");
        fprint_words(word_counts, stdout);
    }

     free_words(&word_counts);

    return 0;
}
