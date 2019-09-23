#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER 13

struct contact
{
    char name[40];
    char phone[5][13];
    char address[100];
    char email [5][50];
    char group [20];
    struct contact *next;
}*head;

void load();
void menu();
void add_contact();
void show_contact();
void search();
void user_exit();
void search_by_name();
void search_by_group();
void delete_contact();
void deleteelement(char str[]);

int main()
{
    head = NULL;
    load();
    menu();
}

void load()
{

    FILE *fp;
    fp = fopen("contacts.txt", "rb");

    if(fp == NULL)
    {
        printf("Error reading file\n");
        return;
    }
    struct contact *current_read, *current_node;
    while(1)
    {
        current_read = malloc(sizeof(struct contact));
        if(fread(current_read, sizeof(struct contact), 1, fp) != 1)
        {
            if(feof(fp))
            {
                return;
            }
            printf("No contacts to show");
            return;
        }
        // putting NULL to mark the end of list(current contact to be read will be the last node)
        // next contact to be read will be put after last node in 'else' block
        current_read -> next = 0;

        if(head == NULL)
        {
            head = current_read;
        }
        else
        {
            current_node = head;
            while(current_node -> next != NULL)
            {
                current_node = current_node -> next;
            }
            current_node -> next = current_read;
        }
    }
    return;
}

void menu()
{
    char key;
    char arrow[] = "==>>";
    char menu_option_lower[5][100];
    char menu_option_upper[5][100];
    strcpy(menu_option_lower[0],"Add a contact");
    strcpy(menu_option_lower[1],"View all contacts");
    strcpy(menu_option_lower[2],"search contacts");
    strcpy(menu_option_lower[3],"delete contacts");
    strcpy(menu_option_lower[4],"Exit");

    strcpy(menu_option_upper[0],"ADD A CONTACT");
    strcpy(menu_option_upper[1],"VIEW ALL CONTACTS");
    strcpy(menu_option_upper[2],"SEARCH CONTACTS");
    strcpy(menu_option_upper[3],"DELETE CONTACTS");
    strcpy(menu_option_upper[4],"EXIT");

    printf("%s\t%s\n",arrow,menu_option_upper[0]);
    printf("\t%s\n",menu_option_lower[1]);
    printf("\t%s\n",menu_option_lower[2]);
    printf("\t%s\n",menu_option_lower[3]);
    printf("\t%s\n",menu_option_lower[4]);
    printf("press UP and DOWN key to navigate the menu");
    int i = 0;
    int j;
    while(1)
    {
        key = getche();
        if(key == DOWN_ARROW)
        {
            if(i == 4)
                i = 0;
            else
                i++;
        }
        else if(key == UP_ARROW)
        {
            if(i == 0)
                i = 4;
            else
                i--;
        }
        if(key == ENTER)
        {
            if(i == 0)
            {
                system("cls");
                add_contact();
            }
            else if(i == 1)
            {
                system("cls");
                show_contact();
            }
            else if(i==2){
                system("cls");
                search();
            }
            else if(i == 3){
                    system("cls");
                delete_contact();

            }

            else if(i == 4)
                user_exit();
        }
        system("cls");
        for(j = 0; j < 5; j++)
        {
            if(j == i)
                printf("%s%s\n",arrow,menu_option_upper[j]);
            else
                printf("\t%s\n",menu_option_lower[j]);
        }
        printf("press UP and DOWN key to navigate the menu");
    }
}

void add_contact()
{
    struct contact *n, *current, *previous;
    n = malloc(sizeof(struct contact));
    printf("Enter name: ");
    gets(n -> name);

    char ch;

    int i = 0;
    do
    {
        printf("Enter phone number: ");
        gets(n -> phone[i++]);
        if(i == 5)
        {
            printf("Maximum contact number reached\n");
            break;
        }
        printf("Want to add another number?(y/n) ");
        ch = getche();
        printf("\n");
    }
    while(ch == 'y');
    strcpy(n -> phone[i], "\0");

    printf("Enter address: ");
    gets(n -> address);

    int j = 0;
    do
    {
        printf("Enter email address: ");
        gets(n -> email[j++]);
        if(j == 5)
        {
            printf("Maximum number of email address reached\n");
            break;
        }
        printf("Want to add another address?(y/n) ");
        ch = getche();
        printf("\n");
    }
    while(ch == 'y');
    strcpy(n -> email[j], "\0");

    printf("Enter group: ");
    gets(n -> group);

    n -> next = NULL;

    // add contact alphabetically
    if(head == NULL)
        head = n;
    else
    {
        // checks if new name is before the first contact
        if(stricmp(n -> name, head -> name) < 0 )
        {
            n -> next = head;
            head = n;
        }
        else
        {
            previous = head;
            current = head -> next;

            while(current != NULL)
            {
                // if new name is alphabetically after previous name and before current name
                if(stricmp(n -> name, previous -> name) > 0 && stricmp(n -> name, current -> name) < 0)
                {
                    n -> next = previous -> next;
                    previous -> next = n;
                    break;
                }
                // otherwise move forward
                previous = current;
                current = current -> next;
            }
            // if end of list is reached without adding new name then add new name at the end of list
            if(current == NULL)
            {
                previous -> next = n;
            }
        }
    }
    printf("Press any key to continue or 'e' to exit");
    char action_key = getche();
    if(action_key == 'e')
        user_exit();

    return;
}

void show_contact()
{
    struct contact *current;
    current = head;
    int i, contact_no = 1;
    char action_key;
    if(head == NULL)
    {
        printf("There are no contacts to show. Press any key to continue or 'e' to exit");
        action_key = getche();
        if(action_key == 'e')
            user_exit();
        return;
    }
    while(current != NULL)
    {
        printf("Contact No:%d\n\n", contact_no);
        printf("Name: %s\n", current -> name);
        for(i = 0; strcmp(current -> phone[i], "\0"); i++)
            printf("Phone-0%d: %s\n", i+1, current -> phone[i]);
        printf("Address: %s\n", current -> address);
        for(i = 0; strcmp(current -> email[i], "\0"); i++)
            printf("Email-0%d: %s\n", i+1, current -> email[i]);
        printf("Group: %s\n", current -> group);
        current = current -> next;
        contact_no++;
        printf("\n\n");
    }
    printf("Press any key to continue or 'e' to exit ");
    action_key = getche();
    if(action_key == 'e')
        user_exit();
}

void user_exit()
{
    FILE *fp;
    fp = fopen("contacts.txt", "wb");
    if(fp == NULL)
    {
        printf("Error reading file");
        return;
    }

    struct contact *current;
    current = head;
    while(current != NULL)
    {
        if(fwrite(current, sizeof(struct contact), 1, fp) != 1)
        {
            printf("Error writing file");
            exit(1);
        }
        current = current -> next;
    }
    exit(1);
}
void search()
{
    char menu_option_lower[3][100];
    char menu_option_upper[3][100];
    strcpy(menu_option_lower[0],"search by name");
    strcpy(menu_option_lower[1],"search by group");
    strcpy(menu_option_lower[2],"back");

    strcpy(menu_option_upper[0],"SEARCH BY NAME");
    strcpy(menu_option_upper[1],"SEARCH BY GROUP");
    strcpy(menu_option_upper[2],"BACK");

    char key;
    char arrow[] = "==>>";

    printf("%s\t%s\n",arrow,menu_option_upper[0]);
    printf("\t%s\n",menu_option_lower[1]);
    printf("\t%s\n",menu_option_lower[2]);
    printf("press UP and DOWN key to navigate the menu");
    int i = 0;
    int j;
    while(1)
    {
        key = getche();
        if(key == DOWN_ARROW)
        {
            if(i == 2)
                i = 0;
            else
                i++;
        }
        else if(key == UP_ARROW)
        {
            if(i == 0)
                i = 2;
            else
                i--;
        }
        if(key == ENTER)
        {
            if(i == 0)
            {
                system("cls");
                search_by_name();
            }
            else if(i == 1)
            {
                system("cls");
                search_by_group();
            }
            else if(i==2)
                return;
        }
        system("cls");
        for(j = 0; j < 3; j++)
        {
            if(j == i)
                printf("%s\t%s\n",arrow,menu_option_upper[j]);
            else
                printf("\t%s\n",menu_option_lower[j]);
        }
        printf("press UP and DOWN key to navigate the menu");
    }

}
void search_by_name()
{
    int flag=0;
    printf("Name: ");
    char str[100];
    gets(str);
    struct contact *current;
    current = head;
    int i;
    char action_key;
    if(head == NULL)
    {
        printf("There are no contacts. Press any key to continue or 'e' to exit");
        action_key = getche();
        if(action_key == 'e')
            user_exit();
        return;
    }
    while(current != NULL)
    {
        if(strcmp(current->name,str)==0){
        printf("Name: %s\n", current -> name);
        for(i = 0; strcmp(current -> phone[i], "\0"); i++)
            printf("Phone-0%d: %s\n", i+1, current -> phone[i]);
        printf("Address: %s\n", current -> address);
        for(i = 0; strcmp(current -> email[i], "\0"); i++)
            printf("Email-0%d: %s\n", i+1, current -> email[i]);
        printf("Group: %s\n", current -> group);
        flag=1;
        break;
        }
        current = current -> next;
    }
    if(flag==0) printf("contact not found\n");
    printf("press any key to continue");
    action_key=getche();
    return;

}

void search_by_group()
{
    int flag=0;
    printf("Group: ");
    char str[100];
    gets(str);
    struct contact *current;
    current = head;
    int i;
    char action_key;
    if(head == NULL)
    {
        printf("There are no contacts. Press any key to continue or 'e' to exit");
        action_key = getche();
        if(action_key == 'e')
            user_exit();
        return;
    }
    while(current != NULL)
    {
        if(strcmp(current->group,str)==0){
                printf("\n**********\n");
        printf("Name: %s\n", current -> name);
        for(i = 0; strcmp(current -> phone[i], "\0"); i++)
            printf("Phone-0%d: %s\n", i+1, current -> phone[i]);
        printf("Address: %s\n", current -> address);
        for(i = 0; strcmp(current -> email[i], "\0"); i++)
            printf("Email-0%d: %s\n", i+1, current -> email[i]);
        printf("Group: %s\n", current -> group);
        flag=1;
        }
        current = current -> next;
        printf("\n\n");
    }
    if(flag==0) printf("group not found\n");
    printf("press any key to continue");
    action_key=getche();
    return;

}

void delete_contact()
{
    int flag=0;
    printf("Name: ");
    char str[100],ch;
    gets(str);
    struct contact *current;
    current = head;
    int i;
    char action_key;
    if(head == NULL)
    {
        printf("There are no contacts. Press any key to continue or 'e' to exit");
        action_key = getche();
        if(action_key == 'e')
            user_exit();
        return;
    }
    while(current != NULL)
    {
        if(strcmp(current->name,str)==0){
        printf("Name: %s\n", current -> name);
        for(i = 0; strcmp(current -> phone[i], "\0"); i++)
            printf("Phone-0%d: %s\n", i+1, current -> phone[i]);
        printf("Address: %s\n", current -> address);
        for(i = 0; strcmp(current -> email[i], "\0"); i++)
            printf("Email-0%d: %s\n", i+1, current -> email[i]);
        printf("Group: %s\n", current -> group);
        flag=1;
        break;
        }
        current = current -> next;
    }
    if(flag==0) printf("contact not found\n");
    else if(flag==1){
        puts("Do you want to delete this contact?(y/n)");
        ch=getche();
        if(ch=='y') deleteelement(str);
    }
    printf("press any key to continue");
    action_key=getche();
    return;
}
void deleteelement(char str[])
{
    struct contact *current,*previous;
    current = head;
    previous = NULL;
    char action_key;
    if(head == NULL)
    {
        printf("There are no contacts. Press any key to continue or 'e' to exit");
        action_key = getche();
        if(action_key == 'e')
            user_exit();
        return;
    }
    while(current != NULL)
    {

        if(strcmp(current->name,str)==0){
                if(previous==NULL) head=head->next;
                else if(current==NULL) previous->next=NULL;
                else previous->next=current->next;
                break;
        }
        else{
                previous=current;
                current=current->next;
        }
    }
    printf("Press any key to continue or 'e' to exit ");
    action_key = getche();
    system("cls");
    if(action_key == 'e')
        user_exit();
}
