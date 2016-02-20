#include <stdio.h>

typedef struct {
	short day, month;
	unsigned year;
}date_t;
/*
void date_struct(int day, int month, int year, date_t * date)
{
	date_t dummy;
	dummy.day = (short)day;
	dummy.month = (short)month;
	dummy.year = (unsigned)year;
	*date = dummy;
}
*/

	date_t dummy;
date_t * date_struct(int day, int month, int year)
{
	dummy.day = (short)day;
	dummy.month = (short)month;
	dummy.year = (unsigned)year;
	return &dummy;
}

int main(int argc, const char *argv[])
{
	int day,month,year;
//	date_t d;
	date_t *d;
	printf("Give day,month,year:\n");
	scanf("%d%d%d", &day, &month, &year);
//	date_struct(day,month,year,&d);
	d = date_struct(day,month,year);
//	printf("date struct valuse: %d-%d-%d\n", d.day, d.month, d.year);
	printf("date struct valuse: %d-%d-%d\n", d->day, d->month, d->year);
	return 0;
}
