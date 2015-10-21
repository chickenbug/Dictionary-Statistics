#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef FIRST_H
#define FIRST_H

typedef struct Node{
	char character, *word;
	int is_word, exact_num, prefix_num;
	struct Node *next[26];
} Node;

void node_initialize(Node* n, char letter);
int char_index(char a);
void reset_head();
int search_word(char* word_point);
int add_word(char* word);
void print_trie(Node* vertex);
void free_trie(Node* vertex);

void matchStr(char* str);
void prefix_match(Node* vertex);

void readData(FILE *data_file);
void readDict(FILE *dict_file);
void printResult();
void print_file(Node* vertex, FILE *f);

int main(int argc, char** argv);

#endif


