#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hofnode{
	struct hofnode* left;
	struct hofnode* right;
	struct hofnode* next;
	struct hofnode* prev;
	int count;
	char c;
	char code[27];
}node;

typedef struct linked {
	node* next;
	int number;
}linkedlist;

linkedlist head = { NULL,0 };
char* encodetable[26];
char* decodetable[26];
FILE* output1;

node* createnode(node* left, node* right, int count, char c) {
	node* temp =(node*)malloc(sizeof(node));
	temp->c = c;
	temp->count = count;

	temp->left = left;
	temp->right = right;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}

void insertlist(node* input) {
	if (head.next == NULL) {
		head.next = input;
		head.next->next = input;
		head.next->prev = input;
		head.number++;
	}
	else {
		node* temp = head.next->prev;
		
		temp->next = input;
		head.next->prev = input;
		input->prev = temp;
		input->next = head.next;
		head.number++;
	}
}

void deletelist(node* input) {
	if (head.number == 1) {
		head.next = NULL;
		head.number--;
	}
	else {
		if (head.next == input) {
			head.next = input->next;
		}
		node* temp = input->prev;
		char c = input->c;
		temp->next = input->next;
		temp = input->next;
		temp->prev = input->prev;
		head.number--;
	}
}

void destroytree(node* head) {
	if (head->left != NULL) {
		destroytree(head->left);
	}
	if (head->right != NULL) {
		destroytree(head->right);
	}
	free(head);
}
void encode(node* input, char c, int i) {
	if (i == -1) {
		++i;
		fprintf(output1, "(");
		if (input->left != NULL) {
			encode(input->left, '1', i);
		}
		fprintf(output1, ",");
		if (input->right != NULL) {
			encode(input->right, '0', i);
		}
		fprintf(output1, ")");
	}
	else {
		input->code[i] = c;
		++i;
		if (input->c != '\0') {
			input->code[i] = '\0';
			encodetable[(input->c) - 'a'] = input->code;
			fprintf(output1, "%c", input->c);
			return;
		}
		fprintf(output1, "(");
		if (input->left != NULL) {
			strcpy(input->left->code, input->code);
			encode(input->left, '1', i);
		}
		fprintf(output1, ",");
		if (input->right != NULL) {
			strcpy(input->right->code, input->code);
			encode(input->right, '0', i);
		}
		fprintf(output1, ")");
	}
}

node* decode(FILE* output,char* buffer,int i) {
	char c = fgetc(output);
	if (c == '(') {
		node* nod = (node*)malloc(sizeof(node));
		buffer[i] = '1';
		nod->left = decode(output,buffer,i+1);
		if ((c = fgetc(output)) != ',') {
			printf("error in decoding");
			return NULL;
		}
		buffer[i] = '0';
		nod->right = decode(output,buffer,i+1);
		if ((c = fgetc(output)) != ')') {
			printf("error in decoding");
			return NULL;
		}
		nod->c = '\0';
		return nod;
	}
	else if ('a' <= c && c <= 'z') {
		node* nod = (node*)malloc(sizeof(node));
		nod->left = NULL;
		nod->right = NULL;
		nod->c = c;
		buffer[i] = '\0';
		strcpy(nod->code, buffer);
		decodetable[c - 'a'] = nod->code;
		return nod;
	}	
}

int main() {
	FILE* input = fopen("hw3_input.txt", "r");
	output1 = fopen("hw3_output1.txt", "w");
	if (output1 == NULL) {
		printf("Failed to write file");
		return -1;
	}
	if (input != NULL) {
		char c;
		int count[26] = { 0, };
		do
		{
			c = fgetc(input);
			if ('a' <= c && c <= 'z') {
				count[c - 'a']++;
			}
			else if ('A' <= c && c <= 'Z') {
				count[c - 'A']++;
			}
		} while (c != EOF);

		for (int i = 0;i < 26;i++) {
			if (count[i] != 0) {
				insertlist(createnode(NULL, NULL, count[i], i + 'a'));
			}
		}

		while (head.number > 1) {
			node* min1 = head.next;
			node* min2 = min1->next;
			node* temp = min2->next;
			while (temp != head.next) {
				if (temp->count < min1->count)
					min1 = temp;
				else if (temp->count < min2->count)
					min2 = temp;
				temp = temp->next;
			}
			if (min1->count > min2->count) {
				temp = min1;
				min1 = min2;
				min2 = temp;
			}
			deletelist(min1);
			deletelist(min2);
			insertlist(createnode(min1, min2, (min1->count + min2->count), '\0'));
		}
		
		encode(head.next, '0', -1);
		fprintf(output1, "HEADEREND");
		rewind(input);
		do
		{
			c = fgetc(input);
			if ('a' <= c && c <= 'z') {
				fprintf(output1, "%s", encodetable[c - 'a']);
			}
			else if ('A' <= c && c <= 'Z') {
				fprintf(output1, "%s", encodetable[c - 'A']);
			}
		} while (c != EOF);

		fclose(input);
		fclose(output1);
		destroytree(head.next);
		
		//uncompress
		FILE* output1 = fopen("hw3_output1.txt", "r");
		FILE* output2 = fopen("hw3_output2.txt", "w");
		char getcode[26] = { 0, };
		node* root = decode(output1,getcode,0);

		char check[10] = { 0, };
		fgets(check, 10, output1);
		if (strcmp(check,"HEADEREND")) {
			printf("HEADERERROR");
			return -1;
		}

		for (int i = 0; i < 26; i++) {
			if (decodetable[i]!=NULL)
				fprintf(output2, "%c:%s\n", i + 'a', decodetable[i]);
		}


		node* temp=root;

		c = fgetc(output1);
		while (c != EOF) {
			if (c == '0') {
				temp = temp->right;
			}
			else if(c == '1') {
				temp = temp->left;
			}
			if (temp->c != '\0') {
				fprintf(output2, "%c", temp->c);
				temp = root;
			}
			c = fgetc(output1);
		}

		destroytree(root);
		fclose(output1);
		fclose(output2);
	}
	else {
		printf("Failed to open file");
		return -1;
	}
}
