#include "first.h"
//TODO seperate Trie code from 
Node* head;

/*node and trie functions */
void node_initialize(Node* n, char letter){
	int i;
	n->character = letter;
	n->is_word = 0;
	n->word = NULL;
	n->exact_num = n->super_num = 0;
	for(i = 0; i<26; i++) n->next[i] = NULL;
}

int char_index(char a){
	int i = a - 97;
	return i;
}

void reset_head(){
	int i;
	for(i = 0; i<26; i++) head->next[i] = NULL;
}

/*searches for an occourence of the word in the trie
  Must be given a lowercase string*/
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

/*adds a word to the trie if it does not already exist
  Must be given a lowercase string*/
int add_word(char* word){
	int index;
	Node* vertex = head;
	char *word_point = word;

	if(vertex == NULL  || word_point == NULL) return 0;
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
			vertex->word = word;
			return 1;
		}
		else{
			vertex = vertex->next[index];
			word_point++;
		}
		
	}
	printf("Code should not reach here\n");
	return 0;
}

// prints the tree and data alphabetically 
void print_trie(Node* vertex){
	int i;
	if(!vertex) return;
	for(i = 0; i<26; i++) print_trie(vertex->next[i]);
	if(vertex->is_word) printf("word: %s exact matches %d super matches %d\n",vertex->word, vertex->exact_num, vertex->super_num);
	return;
}

void free_trie(Node* vertex){
	int i;
	if(!vertex) return;
	for(i = 0; i<26; i++) free_trie(vertex->next[i]);
	free(vertex);
	return;
}


void matchStr(char* str){
	int index;
	Node* vertex = head;
	if(vertex == NULL  || str == NULL || *str =='\0') return;

	while(*str != '\0'){
		index = char_index(*str);
		if(vertex->is_word) vertex->super_num++;

		if(!(vertex->next[index])) return;
		else (vertex = vertex->next[index]);
		str++;
	}
	if(vertex->is_word) vertex->exact_num++;
	return;
}


/*end of node and trie functions*/

void readData(FILE *data_file){
	char string_block[1024]; // Max buffersize 1 kilobyte
	char *begin, *end, *p;
	while(fscanf(data_file,"%s", string_block) != EOF){
		/*after the last word is returned the begin index will be placed at the end '\0'*/
		begin = end = string_block;
		while(*begin != '\0'){
			while(isalpha(*(end+1))){
				end++;
			}
			char word_to_check[begin - end + 2];
			word_to_check[begin - end + 1] ='\0';
			for(p = word_to_check; *p; p++) *p = tolower(*p);
			strncpy(word_to_check, begin, begin-end);
			matchStr(word_to_check);
			begin = end; 
		}
	}
	return;
}

void readDict(FILE *dict_file){
	char string_block[1024]; // Max buffersize 1 kilobyte
	char *begin, *end, *p;
	while(fscanf(dict_file,"%s", string_block) != EOF){
		/*after the last word is returned the begin index will be placed at the end '\0'*/
		begin = string_block;

		while(*begin != '\0'){
			while(!isalpha(*begin) && *begin != '\0') begin++;
			end = begin;
			while(isalpha(*(end+1)) && *end != '\0'){
				end++;
			}
			char word_to_insert[begin - end + 2];
			word_to_insert[begin - end + 1] ='\0';
			strncpy(word_to_insert, begin, begin-end);
			if(word_to_insert != NULL){
			for(p = word_to_insert; *p; p++) *p = tolower(*p);
			add_word(word_to_insert);
			begin = end; 
			}
		}
	}
	return;
}

/*Produces the output files of the program*/
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

void print_file(Node* vertex, FILE *f){
	int i;
	if(!vertex) return;
	for(i = 0; i<26; i++) print_trie(vertex->next[i]);
	if (vertex->is_word
		) fprintf(f,"%s  %d  %d\n",vertex->word, vertex->exact_num, vertex->super_num);
	return;
}


int main(int argc, char** argv){
	FILE *input, *dict_file, *data_file;
	char dict_name[100];
	char data_name[100];
	int i;

	head = malloc(sizeof(Node));
	node_initialize(head, '.');

	input = fopen(argv[1], "r");
	if(input == NULL){
		printf("error\n");
		return 0;
	} 

	while(fscanf(input, "%s %s\n", dict_name, data_name) != EOF){
		dict_file = fopen(dict_name, "r");
		data_file = fopen(data_name, "r");

		if(dict_file == NULL || data_file == NULL){
			printf("error\n");
			return 0;
		} 
		readDict(dict_file);
		readData(data_file);
		printResult();

		//free's head's children recursively and reset head
		for(i=0; i<26; i++) free_trie(head->next[i]);
		reset_head();
	}
	free (head);
	return 0;
}

int mains(){
	head = malloc(sizeof(Node));
	node_initialize(head, '.');
	int i;



	print_trie(head);
	for(i=0; i<26; i++) free_trie(head->next[i]);
	reset_head();
	free(head);

	return 0;
}
