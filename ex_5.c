/***********************
* Yarin Sason
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YES 'y'
#define TOK_COMMA ","
#define COMMA ','
#define COLON ':'
#define Ave_Size 25
#define MAX 20

typedef struct Word {
    char** translations;
    struct Word* next;
} Word;

typedef struct {
    char** languages;
    int numOfLanguages;
    Word* wordList;
} Dictionary;

//function definition
int menu();

/******************
* Function Name: main
* Input: void
* Output: int
* Function Operation: greeting and starting the program.
******************/
int main(){
    menu();
    return 0;
}
/******************
* Function Name: scanString
* Input: void
* Output: char*
* Function Operation: the function scan a unknown size of string from the user,
 * and return the string in fit array (located on heap).
******************/
char * scanString(){
    // I am using in this function in constant for the sake of not do to many reallocates.
    char *tempString = malloc(sizeof (char) * Ave_Size);
    if (tempString == NULL){
        return tempString;
    }
    char temp = '\0';
    int letters = 0;

    //cleaning the buffer
    while (getchar() != '\n' );
    while (temp != '\n'){
        scanf("%c",&temp);
        if (temp != '\n'){
            // if the place that in the temp string is about to over increases the allocation.
            if (letters > MAX){
                tempString = (char*) realloc(tempString,sizeof(char)*(letters+MAX));
            }
            if (tempString == NULL){
                return tempString;
            }
            tempString[letters] = temp;
            letters++;
        }
    }
    tempString[letters] = '\0';
    return tempString;
}
/******************
* Function Name:newDict
* Input: Dictionary * , int
* Output: int
* Function Operation: the function get a pointer to the start of dictionaries array and the
 * number of the dictionaries in this array, and adding a new dictionary to this array.
 * if in any of the allocation point the allocation has failed ,the function automatically
 * free all the memory that created for that memory till the failure point.
******************/
Dictionary * newDict(Dictionary * dicts,int *num){
    char *string = NULL,*token = NULL;
    //char * backUpStrPtr = NULL;
    int wordsCounter=1;
    Dictionary * backUpDictPtr = NULL;

    printf("Define a new dictionary:\n");
    backUpDictPtr = dicts;
    dicts = (Dictionary *) realloc(dicts,(*num+1)*sizeof(Dictionary));
    if (dicts == NULL){
        printf("The creation of the dictionary has failed!\n");
        dicts = backUpDictPtr;
        return backUpDictPtr;
    }
    string = scanString();
    if (string ==NULL){
        printf("The creation of the dictionary has failed!\n");
        dicts = (Dictionary *) realloc(dicts,(*num)*sizeof(Dictionary));
        if (dicts == NULL) {
            dicts = backUpDictPtr;
        }
        return backUpDictPtr;

    }
    //check how many words there's in the string with the help of the commas.
    for (int i=0 ;i< strlen(string);i++){
        if (string[i] == COMMA){
            wordsCounter++;
        }
    }
    dicts[*num].languages = (char **) malloc(sizeof (char *)*wordsCounter);
    if (dicts[*num].languages == NULL){
        printf("The creation of the dictionary has failed!\n");
        backUpDictPtr = dicts;
        dicts = (Dictionary *) realloc(dicts,(*num)*sizeof(Dictionary));
        if (dicts == NULL) {
            dicts = backUpDictPtr;
        }
        return backUpDictPtr;
    }
    for(int i=0;i<wordsCounter;i++){
        if(i==0){
            token = strtok(string,TOK_COMMA);
        } else{
            token = strtok(NULL,TOK_COMMA);
        }
        dicts[*num].languages[i] = (char *) malloc(sizeof (char)*(strlen(token)+1));
        //in case of an allocation fail, freeing all the un needed memory
        if (dicts[*num].languages[i] == NULL){
            i--;
            for (;i>=0 ;i--){
                free(dicts[*num].languages[i]);
            }
            free(dicts[*num].languages);
            backUpDictPtr = dicts;
            dicts = (Dictionary *) realloc(dicts,(*num)*sizeof(Dictionary));
            if (dicts == NULL) {
                dicts = backUpDictPtr;
            }
            return backUpDictPtr;
        }
        strcpy(dicts[*num].languages[i],token);
    }
    free(string);
    printf("The dictionary has been created successfully!\n");
    dicts[*num].numOfLanguages = wordsCounter;
    dicts[*num].wordList = NULL;
    (*num)++;
    return dicts;
}
/******************
* Function Name: dictsPrint
* Input: Dictionary * ,const int *
* Output: void
* Function Operation:the function get an array of dictionaries and prints a list
 * of all the dictionaries inside
******************/
void dictsPrint(Dictionary * dicts,const int * num ){
    printf("Choose a dictionary:\n");
    for (int i=0;i<*num;i++){
        printf("%d. ",i+1);
        for(int word=0;word < dicts[i].numOfLanguages;word++){
            printf("%s",dicts[i].languages[word]);
            if(word != dicts[i].numOfLanguages - 1){
                printf("%c",COMMA);
            }
        }
        printf("\n");
    }
}
/******************
* Function Name: dictPrint
* Input: Dictionary * ,const int
* Output: void
* Function Operation: the function gets an array of dictionaries and a number
 * and print the dictionary tittle in that index.
******************/
void dictPrint(Dictionary * dicts,const int num){
        for(int word=0;word < dicts[num].numOfLanguages;word++){
            printf("%s",dicts[num].languages[word]);
            if(word != dicts[num].numOfLanguages - 1){
                printf("%c",COMMA);
            }
        }
        printf(":\n");
}
/******************
* Function Name: newTrans
* Input: Dictionary ,char *
* Output: char **
* Function Operation: the function gets a dictionary and a string and return a organized
 * two dimension array (pointer for a array of pointers) of the word in the string.
******************/
char ** newTrans(Dictionary dict,char * string){
    char *token;
    char ** translates;
    translates = (char **) malloc(sizeof (char *) * dict.numOfLanguages);
    if (translates == NULL){
        return NULL;
    }
    for(int i=0;i<dict.numOfLanguages;i++){
        if(i==0){
            token = strtok(string,TOK_COMMA);
        } else{
            token = strtok(NULL,TOK_COMMA);
        }
        translates[i] = (char *) malloc(sizeof (char )*(strlen(token) + 1));
        // in case of memory fail freeing all the un necessary allocations.
        if (translates[i] == NULL){
            for(int j =i-1;j>=0;j--){
                free(translates[i]);
            }
            free(translates);
            translates == NULL;
            return NULL;
        }
        strcpy(translates[i],token);
    }
    return translates;
}
/******************
* Function Name: addWord
* Input:Dictionary ,Word **,char *
* Output: void
* Function Operation: the function receive a dictionary and creat a pointer to a Word
 * object and allocate memory for him,than the function go throw the word list and
 * insert the new word in the first empty position.
 *
******************/
void addWord(Dictionary dict,Word **head,char *string){
    Word *newWord;
    newWord = malloc(sizeof (Word));
    newWord->translations = newTrans(dict,string);
    if(newWord->translations == NULL){
        printf("The addition of the word has failed!\n");
        free(newWord);
        return;
    }
    newWord->next = NULL;
    Word *currentPtr;
    currentPtr = *head;
    if (newWord != NULL){
        if (currentPtr == NULL){
            *head = newWord;
            printf("The word has been added successfully!\n");
        } else{
            while (currentPtr->next !=NULL){
                currentPtr = currentPtr->next;
            }
            currentPtr->next = newWord;
            printf("The word has been added successfully!\n");
        }

    } else{
        free(newWord);
        return;
    }
}
/******************
* Function Name: newWord
* Input: Dictionary * , int
* Output: Dictionary *
* Function Operation: the function ask the user in which dictionary he want to add
 * a word,than receive from him a string of word and with the help of addWord function
 * insert the translation of his word to the chosen dictionary.
******************/
Dictionary * newWord(Dictionary * dictionaries, int *nPtr){
    int choice;
    char *string = NULL;
    dictsPrint(dictionaries,nPtr);
    scanf("%d",&choice);
    while(choice > *nPtr  || choice < 1){
        printf("Wrong option, try again:\n");
        scanf("%d",&choice);
    }
    printf("Enter a word in ");
    dictPrint(dictionaries,choice-1);
    string = scanString();
    if (string == NULL){
        printf("The addition of the word has failed!\n");
        return dictionaries;
    }
    addWord(dictionaries[choice-1],&(dictionaries[choice-1].wordList),string);
    free(string);
    return dictionaries;

}
/******************
* Function Name: freeWord
* Input: Word * ,int
* Output: void
* Function Operation: the function are responsible for releasing allocated memory,in
 * a word ,the function get a pointer for a word object,and a number of languages in this word object
 * and free al the memory that allocated in this object.
******************/
void freeWord(Word ** word,int numOfLanguages){
    if ((*word) !=NULL){
        for (int i=0;i<numOfLanguages;i++){
            free((*word)->translations[i]);
            (*word)->translations[i] = NULL;
        }
        free((*word)->translations);
        (*word)->translations = NULL;
        free(*word);
        (*word) =NULL;

    }

}
/******************
* Function Name: eraseWord
* Input: Dictionary ,Word **,char*
* Output: void
* Function Operation: the function gets a dictionary and a pointer to a pointer of a Word ,with
 * a string that indicate the word that needs to be deleted ,she run throw the word list
 * and search for this word ,when she find it, she delete it ,if the word isn't exists she
 * print it to the user.
******************/
void eraseWord(Dictionary dict,Word **head,char* str){
    Word *current= *head;
    Word *previous = *head;

    if(current == NULL ){
        printf("The deletion of the word has failed!\n");
        return;
    }else if(!strcmp(current->translations[0],str)) {
        *head = current->next;
        freeWord(&current,dict.numOfLanguages);
        current = NULL;
        printf("The word has been deleted successfully!\n");
        return;
    }else{
        while (current != NULL){
            if (!(strcmp(current->translations[0],str))) {
                // Mediating  between the word that before of the deleted word and the rest of the word list
                previous->next = current->next;
                freeWord(&current,dict.numOfLanguages);
                printf("The word has been deleted successfully!\n");
                return;
            }
            previous = current;
            current = current->next;
        }
        printf("The deletion of the word has failed!\n");
        return;
    }
}
/******************
* Function Name: delWord
* Input: Dictionary *, int *
* Output: void
* Function Operation:the interface function of word deleting, the function ask the user which,
 * from which dictionary he wants to delete a word and get a string for this word
 * that with the help of eraseWord function deleting that word.
******************/
void delWord(Dictionary * dictionaries, int *nPtr){
    int choice;
    char pick,*string;
    dictsPrint(dictionaries,nPtr);
    scanf("%d",&choice);
    while(choice > *nPtr  || choice < 1){
        printf("Wrong option, try again:\n");
        scanf("%d",&choice);
    }
    printf("Enter a word in ");
    printf("%s%c\n",dictionaries[choice-1].languages[0],COLON);
    string = scanString();
    if (string == NULL){
        printf("The deletion of the word has failed!");
        return;
    }
    printf("Are you sure? (y/n)\n");
    scanf("%c",&pick);
    if (pick == 'y'){
        eraseWord(dictionaries[choice-1],&dictionaries[choice-1].wordList,string);
    }
    else{
        printf("The deletion of the word has been canceled.\n");
    }
    free(string);
}
/******************
* Function Name: findWord
* Input: Dictionary *,int *
* Output: void
* Function Operation: the function ask the user for the dictionary he want to search word in ,
 * and than get a string for this word from the user ,after that she search in the chosen dictionary for this word,
 * if she finds the word she prints the translation ,if the word isn't exists she print an appropriate
 * message to the user.
******************/
void findWord(Dictionary * dictionary,int *dictNum){
    int choice = 0,flag=0;
    Word * current;
    char *string;

    dictsPrint(dictionary,dictNum);
    scanf("%d",&choice);
    while(choice > *dictNum  || choice < 1){
        printf("Wrong option, try again:\n");
        scanf("%d",&choice);
    }
    printf("Enter a word in ");
    printf("%s:\n",dictionary[choice-1].languages[0]);
    string = scanString();
    if (string == NULL){
        return;
    }

    current = dictionary[choice-1].wordList;
    while (current != NULL){
        if(!strcmp(current->translations[0],string)){
            flag = 1;
            printf("The translations are:\n");
            for (int i=1;i<dictionary[choice-1].numOfLanguages;i++){
                printf("%s: ",dictionary[choice-1].languages[i]);
                printf("%s",current->translations[i]);
                if (i<dictionary[choice-1].numOfLanguages-1){
                    printf("%c ",COMMA);
                }
            }

            printf("\n");
            break;
        }
        current = current->next;
    }
    if(!flag){
        printf("There are no translations for \"%s\" in this dictionary.\n",string);
    }
    free(string);
}
/******************
* Function Name: freeWordList
* Input: Word * ,int
* Output: void
* Function Operation: the function get a pointer to a list of words and free all of them allocated memory.
******************/
void freeWordList(Word * current,int numOfLanguages){
    //if the list is empty.
    if(current == NULL){
        return;
    }
    //stop condition
    if (current->next == NULL){
        freeWord(&current,numOfLanguages);
        current =NULL;
        return;
    }
    freeWordList(current->next,numOfLanguages);
    freeWord(&current,numOfLanguages);
    current =NULL;
}
/******************
* Function Name: freeArray
* Input: char ** , int
* Output: void
* Function Operation: the function gets an array of word and freeing his allocated memory.
******************/
void freeArray(char ** languages, int numOfLanguages){
    for (int i=0;i<numOfLanguages;i++){
        free(languages[i]);
        languages[i] = NULL;
    }
}
/******************
* Function Name: delDict
* Input: Dictionary * ,int*
* Output: void
* Function Operation: the function gets an array of dictionary and ask for the user which
 * dictionary he wants to delete ,than she delete it and remove it from the array of dictionaries.
******************/
void delDict(Dictionary ** dictionary,int* num){
    int choice;
    char pick = '\0';
    dictsPrint(*dictionary,num);
    scanf("%d",&choice);
    while(choice > *num  || choice < 1){
        printf("Wrong option, try again:\n");
        scanf("%d",&choice);
    }
    printf("Are you sure? (y/n)\n");
    scanf(" %c",&pick);
    if (pick == YES){

        freeWordList(((*dictionary)[choice-1]).wordList,(*dictionary)[choice-1].numOfLanguages);
        freeArray((*dictionary)[choice-1].languages,(*dictionary)[choice-1].numOfLanguages);
        free((*dictionary)[choice-1].languages);
        for(int i= choice-1;i< (*num)-1;i++){
            (*dictionary)[i]=(*dictionary)[i+1];
        }
        (*num)--;
        if (*num == 0){
            free(*dictionary);
            *dictionary = NULL;
        }else{
            (*dictionary) = realloc((*dictionary),sizeof (Dictionary) * (*num));
            if ((*dictionary) ==NULL){
                printf("The deletion of the dictionary has failed!\n");
                return;
        }
        }

        printf("The dictionary has been deleted successfully!\n");
    }else{
        printf("The deletion of the dictionary has been canceled.\n");
    }
}
/******************
* Function Name: delAll
* Input: Dictionary * ,const int *
* Output: void
* Function Operation: the function goes over the all the dictionary array and free all the memory that
 * use fot them.
******************/
void delAll(Dictionary ** dictionary,int * nPtr){
    for (int i = 0;i<*nPtr;i++){
        freeWordList((*dictionary)[i].wordList,(*dictionary)[i].numOfLanguages);
        freeArray((*dictionary)[i].languages,(*dictionary)[i].numOfLanguages);
        free((*dictionary)[i].languages);
    }
    free(*dictionary);
    *nPtr = 0;
    *dictionary =NULL;
}
/******************
* Function Name: menu
* Input: int
* Output: int
* Function Operation: the function get a pointer to an array of dictionaries,and operate
 * on those dictionaries several editing actions.
******************/
int menu() {
    Dictionary *dictionaries =NULL;
    int numOfDictionaries = 0,choice = 0;
    int *nPtr = &numOfDictionaries;
    int flag = 0;

    do {
        if(!flag) {
            printf("Welcome to the dictionaries manager!\n");
            printf("Choose an option:\n"
                   "1. Create a new dictionary.\n"
                   "2. Add a word to a dictionary.\n"
                   "3. Delete a word from a dictionary.\n"
                   "4. Find a word in a dictionary.\n"
                   "5. Delete a dictionary.\n"
                   "6. Exit.\n");
        }
        scanf("%d", &choice);
        if (choice == 1) {
            dictionaries = newDict(dictionaries, nPtr);
            flag = 0;
        }else if (numOfDictionaries == 0 && choice < 6) {
            printf("This option is not available right now, try again:\n");
            flag =1;
        } else if (choice == 2) {
            newWord(dictionaries, nPtr);
            flag = 0;
        } else if (choice == 3) {
            delWord(dictionaries, nPtr);
            flag = 0;
        } else if (choice == 4) {
            findWord(dictionaries, nPtr);
            flag=0;
        } else if (choice == 5) {
            delDict(&dictionaries, nPtr);
            flag=0;
        } else if (choice == 6) {
            delAll(&dictionaries, nPtr);
            flag = 0;
            printf("Bye!\n");
        }
        else{
            printf("Wrong option, try again:\n");
            flag = 1;
        }
    } while (choice != 6);

    return 0;
}
