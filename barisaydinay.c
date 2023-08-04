// BARIS AYDINAY 2452977 //

// Actually, my code works for many patterns such as ; won (also WON), credit, bet , b (letter). However, some patterns it founds more than
// one and it seems weird. I don't understand, we spoke with Sukru Hoca and I couldn't found any solution. Anyways, it seems applicable code for me.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct myData
{
    int mailID;
    char sender[50];
    char receipt[50];
    int day;
    char content[200];
    int noOfWords;
    char type[8];
};


struct spamMails
{
    int mailID;
    char sender[50];
    char receipt[50];
    int day;
    char content[200];
    int noOfWords;
    char type[8];
    struct spamMails *nextSpam;
};

struct nonSpamMails
{
    int mailID;
    char sender[50];
    char receipt[50];
    int day;
    char content[200];
    int noOfWords;
    char type[8];
    struct nonSpamMails *nextnonSpam;

};



void readMail(char *,int,struct spamMails*,struct nonSpamMails*);
int searchPattern(char *,char *,int *);
char * insertChar(char *,int,int);


int main()
{
    int number=0;
    char filePath[50];
    char pattern[50];
    printf("Enter data path: ");
    scanf("%s",filePath);
    fflush(stdin);
    printf("How many files: ");
    scanf("%d",&number);
    fflush(stdin);
    printf("Enter the pattern: ");
    scanf("%s",pattern);
    strcat(filePath,"/");

    struct spamMails *spamHead = (struct spamMails *) malloc(sizeof(struct spamMails));
    spamHead->mailID = 0;
    spamHead->day = 0;
    spamHead->noOfWords = 0;
    strcpy(spamHead->content,"dummy");
    strcpy(spamHead->receipt,"dummy");
    strcpy(spamHead->sender,"dummy");
    strcpy(spamHead->type,"Spam");
    spamHead->nextSpam = NULL;

    struct nonSpamMails *nonspamHead = (struct nonSpamMails *) malloc(sizeof(struct nonSpamMails));
    nonspamHead->mailID = 0;
    nonspamHead->day = 0;
    nonspamHead->noOfWords = 0;
    strcpy(nonspamHead->content,"dummy");
    strcpy(nonspamHead->receipt,"dummy");
    strcpy(nonspamHead->sender,"dummy");
    strcpy(nonspamHead->type,"Nonspam");
    nonspamHead->nextnonSpam = NULL;

    readMail(filePath,number,spamHead,nonspamHead); // Mails have been read and related linked lists created.

    int b;
    int countPattern = 0;
    int countspamMails = 0;
    int count = 1;

    struct spamMails *spamDummy = spamHead->nextSpam;
    printf("\nSpam emails containing the pattern:\n");
    while(spamDummy != NULL) {
        b = searchPattern(pattern,spamDummy->content,&count);
        if(b != 0) {
        countspamMails++;
        countPattern = countPattern + b;
        }
        spamDummy = spamDummy->nextSpam;

    }

    printf("%d pattern(s) detected in %d email(s)\n\n",countPattern,countspamMails);




    int a;
    countPattern = 0;
    int countNonspamMails = 0;
    count = 1;

    struct nonSpamMails *nonspamDummy = nonspamHead->nextnonSpam;
    printf("Non-spam emails containing the pattern:\n");
    while(nonspamDummy != NULL) {
        a = searchPattern(pattern,nonspamDummy->content,&count);
        if(a != 0) {
            countNonspamMails++;
            countPattern = countPattern + a;
        }
        nonspamDummy = nonspamDummy->nextnonSpam;

    }

    printf("%d pattern(s) detected in %d email(s)\n",countPattern,countNonspamMails);

}

int searchPattern(char pattern[50],char text[500],int *count) {
    char *update_text = text;
    int m = strlen(pattern);
    int n = strlen(text);
    int d = 128;
    int q = 256;
    int i, j;
    int p = 0;
    int t0 = 0;
    int patternCount = 0;
    int flag = 0;

    int brs = pow(d,(m-1));
    int h = brs % q;

    for (i=0;i<m;i++) {
        p = (d * p + tolower(pattern[i])) % q;  // Since the program should be case insensitive I used 'tolower'
        t0 = (d * t0 + tolower(text[i])) % q;
    }

    //
    int s;
    for (s=0;s<=(n-m);s++) {


        if (p == t0) {

            for (j = 0; j < m; j++) {
                if ( tolower(text[s+j]) != tolower(pattern[j]) ) break;
            }

            if (j == m)
                {
                flag = 1;
                strcpy(update_text,insertChar(update_text,s,m));
                patternCount++;
                }
        }


        if (s < (n-m)) {
            t0 = (d * (t0 - tolower(text[s]) * h) + tolower(text[s + m])) % q;

            if (t0 < 0) t0 = (t0 + q);
        }
    }

    if(flag) {printf("%d. %s",*count,update_text); (*count)++;}


    return patternCount;
}


char *insertChar(char str[200],int index,int patternLeng) {
    int i;
    int leng = strlen(str);
    int new_leng = leng+2;
    char new_str[new_leng];

    int position = index;

    for(i=0;i<position;i++) {
        new_str[i] = str[i];
    }
    new_str[position] = '[';
    for(i=position+1;i<new_leng;i++) {
        new_str[i] = str[i-1];
    }
    new_str[new_leng] = '\0';

    leng = strlen(new_str);
    new_leng = leng+2;
    char last_str[new_leng];

    position = position+patternLeng+1;

    for(i=0;i<position;i++) {
        last_str[i] = new_str[i];
    }
    last_str[position] = ']';
    for(i=position+1;i<new_leng;i++) {
        last_str[i] = new_str[i-1];
    }
    last_str[new_leng] = '\0';


    char *string = malloc(new_leng);
    strcpy(string,last_str);
    return string;

}

void readMail(char filePath[50],int number,struct spamMails *spamHead,struct nonSpamMails *nonspamHead)
{

    FILE *inFile1;
    char fileName[50]="";
    char fileNumber[3] = "";
    int i;
    int validity=0;


    int j=1;
    int count=0;


    for(i=0; i < number; i++)
    {
        struct myData dummy;



        char str[200] = "";
        strcpy(fileName,filePath);

        itoa(i+1, fileNumber, 10);


        strcat(fileName,fileNumber);
        strcat(fileName,".txt");
        inFile1 = fopen(fileName,"r");

        if(inFile1 != NULL)
        {
            validity = 1;
        }

        if(inFile1 == NULL)
        {
            if(validity == 1)
            {
                printf("\n*** You have entered excess number of files, so entire folder have been read ! ***\n");
                i = number-1;
                continue;
            }
            else
            {
                printf("Directory that contains the mails can not be found !");
                exit(0);
            }
        }


        fseek(inFile1, 0, SEEK_SET);

        while(fgets(str,200,inFile1))
        {


            if(j==1)
            {
                int id = atoi(str);
                dummy.mailID = id;
            }

            else if(j==2)
            {
                if(str[0] == ' ') {
                    int subsize = strlen(str) -1;
                    char sub[subsize];
                    strncpy(sub,&str[1],subsize-1);
                    sub[subsize-1] = '\0';
                    strcpy(dummy.sender,sub);
                }
                else {
                        int subsize = strlen(str);
                        char sub[subsize];
                        strncpy(sub,&str[0],subsize-1);
                        sub[subsize-1] = '\0';
                        strcpy(dummy.sender,sub);
                }



            }
            else if(j==3)
            {

                if(str[0] == ' ') {
                    int subsize = strlen(str) -1;
                    char sub[subsize];
                    strncpy(sub,&str[1],subsize-1);
                    sub[subsize-1] = '\0';
                    strcpy(dummy.receipt,sub);
                }
                else {
                        int subsize = strlen(str);
                        char sub[subsize];
                        strncpy(sub,&str[0],subsize-1);
                        sub[subsize-1] = '\0';
                        strcpy(dummy.receipt,sub);
                }

            }

            else if(j==4)
            {
                int dayNo = atoi(str);
                dummy.day = dayNo;
            }
            else if(j==6)
            {
                strcpy(dummy.content,str);
                int k,words=0;

                for(k=0; k<strlen(str); k++)
                {
                    if(str[k] == ' ') words++;
                }
                dummy.noOfWords = words+1;

            }
            else if(j==5) {

                    if(str[0] == ' ') {
                    int subsize = strlen(str) -1;
                    char sub[subsize];
                    strncpy(sub,&str[1],subsize-1);
                    sub[subsize-1] = '\0';
                    strcpy(dummy.type,sub);
                }
                else {
                        int subsize = strlen(str);
                        char sub[subsize];
                        strncpy(sub,&str[0],subsize-1);
                        sub[subsize-1] = '\0';
                        strcpy(dummy.type,sub);
                }



            }


            j++;
            if(j==4) fseek(inFile1, 6, SEEK_CUR);
            if(j==2) fseek(inFile1, 6,SEEK_CUR);
            if(j==3) fseek(inFile1,4,SEEK_CUR);
            if(j==5) fseek(inFile1,6,SEEK_CUR);
            count++;

        }



        if(strcmp(dummy.type,"Spam") == 0) {

            if(spamHead->nextSpam == NULL) {
                struct spamMails *dummySpam = ( struct spamMails  *) malloc(sizeof (struct spamMails));
                strcpy(dummySpam->sender,dummy.sender);
                strcpy(dummySpam->receipt,dummy.receipt);
                strcpy(dummySpam->content,dummy.content);
                strcpy(dummySpam->type,dummy.type);
                dummySpam->nextSpam = NULL;
                spamHead->nextSpam = dummySpam;
            }
            else {
                struct spamMails *dummySpam = ( struct spamMails  *) malloc(sizeof (struct spamMails));
                strcpy(dummySpam->sender,dummy.sender);
                strcpy(dummySpam->receipt,dummy.receipt);
                strcpy(dummySpam->content,dummy.content);
                strcpy(dummySpam->type,dummy.type);
                dummySpam->nextSpam = spamHead->nextSpam;
                spamHead->nextSpam = dummySpam;

            }

        }

        if(strcmp(dummy.type,"Nonspam") == 0) {

            if(nonspamHead->nextnonSpam == NULL) {
                struct nonSpamMails *dummynonSpam = ( struct nonSpamMails  *) malloc(sizeof (struct nonSpamMails));
                strcpy(dummynonSpam->sender,dummy.sender);
                strcpy(dummynonSpam->receipt,dummy.receipt);
                strcpy(dummynonSpam->content,dummy.content);
                strcpy(dummynonSpam->type,dummy.type);
                dummynonSpam->nextnonSpam = NULL;
                nonspamHead->nextnonSpam = dummynonSpam;
            }
            else {
                struct nonSpamMails *dummynonSpam = ( struct nonSpamMails  *) malloc(sizeof (struct nonSpamMails));
                strcpy(dummynonSpam->sender,dummy.sender);
                strcpy(dummynonSpam->receipt,dummy.receipt);
                strcpy(dummynonSpam->content,dummy.content);
                strcpy(dummynonSpam->type,dummy.type);
                dummynonSpam->nextnonSpam = nonspamHead->nextnonSpam;
                nonspamHead->nextnonSpam = dummynonSpam;

            }

        }


        j=1;
        strcpy(fileName,"");
        fclose(inFile1);
    }

}


