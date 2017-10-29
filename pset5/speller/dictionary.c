//Implements a dictionary's functionality.
#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include <stdbool.h>
#include<string.h>

#include "dictionary.h"

struct node
{
	struct node *character[27];
	bool isWord;
};
typedef struct node node;
node *root = NULL;

unsigned int word_count = 0;

//Returns true if word is in dictionary else false.
bool check(const char *word)
{
	char check[46];
	strcpy(check,word);
	
	node *ptr = root;
	int index = 0, char_index;
	while(check[index]!='\0')
	{
		check[index] = tolower(check[index]);

		if(check[index]=='\'')
    			char_index = 26;
    		else
    			char_index = check[index] - 'a';

		if( ptr->character[char_index] != NULL)
			ptr = ptr->character[char_index];
		else
			return false;
		index++;
	}
	if(ptr->isWord == true)
    	return true;
    else
    	return false;
}

//Loads dictionary into memory. Returns true if successful else false.
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    if(fp==NULL)
    	return false;
    
    
    root = (node *) malloc(sizeof(node));
    for(int i=0; i<27; i++)
    {
    	root->character[i] = NULL;
    	root->isWord = false;
    }

    char word[LENGTH+1];
    while( fscanf(fp, "%s", word) != EOF )
    {
    	node *ptr = root, *prev_ptr = NULL;

    	//index keep track of position on word & char_index adjust on which position to look for
    	int index = 0, char_index; 

    	//traverse until characters are available
    	while(word[index]!='\0')
    	{
    		word[index] = tolower(word[index]);
    		
    		prev_ptr = ptr;

    		if(word[index]=='\'')
    			char_index = 26;
    		else
    			char_index = word[index] - 'a';

    		ptr = ptr->character[char_index];
    		if(ptr==NULL)
    			break;

    		index++;
    	}

    	ptr = prev_ptr;
    	//create new node on each stage
    	while(word[index]!='\0')
    	{
    		//choose char_index
    		if(word[index]=='\'')
    			char_index = 26;
    		else
    			char_index = word[index] - 'a';

    		//add node & increment ptr
    		ptr->character[char_index] = (node *) malloc(sizeof(node));
		    ptr = ptr->character[char_index];
		    
		    //initialize values
		    for(int i=0; i<27; i++)
		    {
		    	ptr->character[i] = NULL;
		    	ptr->isWord = false;
		    }
		    index++;
    	}
    	ptr->isWord = true;
    	word_count++;
    }
    
    return true;
}

//Returns number of words in dictionary if loaded else 0 if not yet loaded.
unsigned int size(void)
{
    return word_count;
}

//Unloads dictionary from memory. Returns true if successful else false.
bool unload(void)
{
	free(root);
    return true;
}

void free_memory(node *ptr)
{
	for(int i=0; i<27; i++)
	{
		if(ptr->character[i] != NULL)
			free_memory(ptr->character[i]);
	}

	free(ptr);
}
