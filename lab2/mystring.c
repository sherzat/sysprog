#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 20

int main(int argc, const char *argv[])
{
	char first[MAX];
	char second[MAX];
	char str[MAX];
	char name[MAX*2];
	int year;

	printf("Please enter your first name and last name\n");
	scanf("%s%s", first, second);
	printf("first name: %s, second name: %s\n", first, second);

	printf("length of second name : %lu\n", strlen(second));
	for (int i = 0 ; i <= strlen(second) ; i++)
	{
		str[i] = toupper(second[i]);
	}
	printf("length of str name : %lu\n", strlen(str));
	printf("upper case second name: %s\n", str);
	
	int n = 0;
	if ((n = strcmp(second, str)) == 0)
		printf("same\n");
	else 
		printf("not same %d \n", n);

	strcpy(name, first);
	strcat(name, second);
	printf("name is: %s\n", name);

	printf("Please enter your birth year\n");
	scanf("%d", &year);
	printf("birth year: %d\n", year);

	snprintf(name, MAX, "%s %s %d",first, second,year);
	printf("name is: %s\n", name);
//	int i;

//  sscanf("hello 1 2 3 4 5", "%s%d", str, &i);
//  printf("%s %d", str, i);
	sscanf(name, "%s%s%d",first, second, &year);
	printf("first name is: %s. second name is: %s. birth year is :%d\n",first, second, year);

	return 0;
}
