#include "second.h"

Node* head;

/*node and trie functions */

//initializes a created node to default values, and a letter to avoid garbage data
void node_initialize(Node* n, char letter){
	int i;
	n->character = letter;
	n->is_word = 0;
	n->word = NULL;
	n->exact_num = n->prefix_num = 0;
	for(i = 0; i<26; i++) n->next[i] = NULL;
}

//Takes a letter, and returns the appropriate index value for a node's children array
int char_index(char a){
	int i = a - 97;
	return i;
}

//reset's the head's children to NULL between files
void reset_head(){
	int i;
	for(i = 0; i<26; i++) head->next[i] = NULL;
}

/*searches for an occourence of the word in the trie. Returns 1 if found*/
int search_word(char* word_point){
	int index;
	Node* vertex = head;

	if(vertex == NULL  || word_point == NULL || *word_point =='\0') return 0;

	while(*word_point != '\0'){
		index = char_index(*word_point);
		if(!(vertex->next[index])) return 0;
		else (vertex = vertex->next[index]);
		word_point++;
	}
	if(vertex->is_word) return 1;
	return 0;
}

/*adds a word to the trie if it does not already exist*/
int add_word(char* word){
	int index;
	Node* vertex = head;
	char *word_point = word;

	if(vertex == NULL  || *word_point == '\0') return 0;
	if(search_word(word_point)) return 0; // words  already in the trie must not be added

	while(*word_point != '\0'){
		index = char_index(*word_point);
		if(!(vertex->next[index])){
			while(*word_point != '\0'){
				index = char_index(*word_point);
				Node* new = (Node*)malloc(sizeof(Node));
				node_initialize(new, *word_point);
				vertex->next[index] = new;
				vertex = vertex->next[index];
				word_point++;
			}
			vertex->is_word = 1;
			vertex->word = malloc(strlen(word)+1);
			strcpy(vertex->word, word);
			return 1;
		}
		else{
			vertex = vertex->next[index];
			word_point++;
		}
		
	}
	vertex->is_word = 1;
	vertex->word =  malloc(strlen(word)+1);
	strcpy(vertex->word, word);
	return 1;
}

// prints the tree and data alphabetically for debug
void print_trie(Node* vertex){
	int i;
	if(!vertex) return;
	printf("%c ", vertex->character);
	if(vertex->is_word){
		 printf("\n word: %s exact matches %d super matches %d\n",vertex->word, vertex->exact_num, vertex->prefix_num);
	}		
	for(i = 0; i<26; i++) print_trie(vertex->next[i]);	
	return;
}

//free's a tree and it's children recursively
void free_trie(Node* vertex){
	int i;
	if(!vertex) return;
	for(i = 0; i<26; i++) free_trie(vertex->next[i]);
	free(vertex->word);
	free(vertex);
	return;
}

/*end of node and trie functions*/

//Iterates through the trie to find the input word to update exact match, then calls prefix_match
void matchStr(char* str){
	int index, i;
	Node* vertex = head;
	if(vertex == NULL  || str == NULL || *str =='\0') return;

	while(*str != '\0'){
		index = char_index(*str);
		if(!(vertex->next[index])) return;
		else (vertex = vertex->next[index]);
		str++;
	}
	if(vertex->is_word) vertex->exact_num++;
	for(i=0; i<26; i++) prefix_match(vertex->next[i]);
	return;
}

//Iterates through a node and its children to update prefix counts
void prefix_match(Node* vertex){
	int i;
	if(!vertex) return;
	for(i = 0; i<26; i++) prefix_match(vertex->next[i]);
	if(vertex->is_word) vertex->prefix_num++;
	return;
}


/*Reads through the data file for valid words, and updates word statistics using matchStr()*/
void readData(FILE *data_file){
	char string_block[1024]; // Max buffersize 1 kilobyte
	char *begin, *end, *p;
	while(fscanf(data_file,"%s", string_block) != EOF){
		begin = string_block;
		while(*begin != '\0'){
			while(!isalpha(*begin) && *begin != '\0') begin++;
			if(*begin == '\0') break;
			end = begin;

			while(isalpha(*(end+1))){
				end++;
			}
			char word_to_check[end - begin + 2];
			word_to_check[end - begin + 1] ='\0';
			strncpy(word_to_check, begin, end - begin + 1 );
			for(p = word_to_check; *p; p++) *p = tolower(*p);
			matchStr(word_to_check);
			begin = ++end; 
		}
	}
	return;
}

/*Reads through the dict_file for valid words, and inserts them into the trie using add_word()*/
void readDict(FILE *dict_file){
	char string_block[1024]; // Max buffersize 1 kilobyte
	char *begin, *end, *p;
	while(fscanf(dict_file,"%s", string_block) != EOF){
		begin = string_block;

		while(*begin != '\0'){
			while(!isalpha(*begin) && *begin != '\0') begin++;
			if(*begin == '\0') break;
			end = begin;

			while(isalpha(*(end+1)) && *end != '\0'){
				end++;
			}
			char word_to_insert[end - begin + 2];
			word_to_insert[end - begin + 1] ='\0';
			strncpy(word_to_insert, begin, end - begin +1);
			for(p = word_to_insert; *p; p++) *p = tolower(*p);
			add_word(word_to_insert);
			begin = ++end; 
			
		}
	}
	return;
}

/*Creates the output of the program. 
Makes a file and prints the line statistics to it using the recursive print funciton print_f*/
void printResult(){
	static int line = 0;
	line++;
	char file_name[50];
	sprintf(file_name,"out%d.txt", line);

	FILE *f = fopen(file_name, "w");
	if (f == NULL){
    	printf("Error opening file!\n");
    	exit(1);
	}
	print_file(head, f);
	fclose(f);
	return;
}


/*Takes in a files stream object and a starting vertex.
Uses recursive tree traversal to alphabetically print the statistics of the three alphabetically*/
void print_file(Node* vertex, FILE *f){
	int i;
	if(!vertex) return;
	if (vertex->is_word) fprintf(f,"%s %d %d\n",vertex->word, vertex->exact_num, vertex->prefix_num);
	for(i = 0; i<26; i++) print_file(vertex->next[i], f);
	return;
}

/*Program main: Orchestrates the file, read, and print operations to create the necessary statistics*/ 
int main(int argc, char** argv){
	FILE *input, *dict_file, *data_file;
	char dict_name[100];
	char data_name[100];

	input = fopen(argv[1], "r");
	if(input == NULL){
		printf("error\n");
		return 0;
	} 

	while(fscanf(input, "%s %s\n", dict_name, data_name) != EOF){
		head = malloc(sizeof(Node));
		node_initialize(head, '.');

		dict_file = fopen(dict_name, "r");
		data_file = fopen(data_name, "r");

		if(dict_file == NULL || data_file == NULL){
			printf("error\n");
			return 0;
		} 
		readDict(dict_file);
		readData(data_file);
		printResult();

		free_trie(head);
		head = NULL;
	}
	free (head);
	return 0;
}

