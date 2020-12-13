#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
struct Readeritem
{
	int no;
	char name[10];
	char class[12];
	int sno;
	int level;
	int islost;
}reader;

int searchreader(int symbol, int id)
{
	FILE *fp;
	struct Readeritem ifexist;
	int pos = 1;  //标记该编号的位置
	if (symbol == 1)
		fp = fopen("students.txt", "rb");
	else
		fp = fopen("teachers.txt", "rb");
	while (1)
	{
		int n = 0;
		n = fread(&ifexist, sizeof(struct Readeritem), 1, fp);
		if (n == 0)
		{
			pos = 0;
			break;
		}
		else if (ifexist.no == id && ifexist.islost == 0)   
			break;
		else if (ifexist.no == id && ifexist.islost == 1)
		{
			pos = -1;
			break;
		}
		pos++;
	}
	fclose(fp);

	return pos;   //三个状态码：0表示没有此读者、-1表示存在但是已挂失、其他表示存在
}

void setreader(int symbol)
{
	printf("输入借书证号、姓名、班级和学号：\n");
	scanf("%d%s%s%d", &reader.no, &reader.name, &reader.class, &reader.sno);
	reader.level = symbol;
	reader.islost = 0;

	if (searchreader(symbol, reader.no) == 0)
	{
		if (symbol == 1)
		{
			FILE *fp = fopen("students.txt", "ab");
			fwrite(&reader, sizeof(struct Readeritem), 1, fp);
			fclose(fp);
		}
		else
		{
			FILE *fp = fopen("teachers.txt", "ab");
			fwrite(&reader, sizeof(struct Readeritem), 1, fp);
			fclose(fp);
		}
		printf("-----录入成功-----\n");
	}
	else
		printf("----------借书证号已存在，请重新输入！\n");
	
}

void getreader(int symbol)
{
	FILE *fp;
	if (symbol == 1)
		fp = fopen("students.txt", "rb");
	else
		fp = fopen("teachers.txt", "rb");
	printf("借书证号  姓名  班级  学号  （学生1/教师2）  挂失（是1/否0）\n");
	while (1)
	{
		int n = 0;
		n = fread(&reader, sizeof(struct Readeritem), 1, fp);
		if (n == 0)
			break;
		printf("%03d  %s   %s\t  %d\t  %d\t   %d\n", reader.no, reader.name, reader.class, reader.sno, reader.level, reader.islost);
	}
	fclose(fp);
	printf("\n");
}

int countreader(int symbol)
{
	FILE *fp;
	int num = 0;
	if (symbol == 1)
		fp = fopen("students.txt", "rb");
	else
		fp = fopen("teachers.txt", "rb");
	while (1)
	{
		int n = 0;
		n = fread(&reader, sizeof(struct Readeritem), 1, fp);
		if (n == 0)
			break;
		num++;
	}
	fclose(fp);

	return num;
}

void remreader(int symbol)
{
	printf("输入你想删除的读者借书证号： ");
	int id;
	scanf("%d", &id);
	int n = searchreader(symbol, id), i, cho;
	if (n != 0)
	{
		struct Readeritem removeit[10];
		FILE *fp;
		if (symbol == 1)
			fp = fopen("students.txt", "rb");
		else
			fp = fopen("teachers.txt", "rb");
		fseek(fp, (n-1) * sizeof(struct Readeritem), 0);
		fread(&reader, sizeof(struct Readeritem), 1, fp);
		printf("%03d\t%s\t%s\t%d\t%d\t%d\n", reader.no, reader.name, reader.class, reader.sno, reader.level, reader.islost);
		rewind(fp);
		printf("[1]继续删除\t[2]取消并退出\n");
		scanf("%d", &cho);
		if (cho == 1)
		{
			int m = countreader(symbol);
			fread(&removeit, sizeof(struct Readeritem), m, fp);

			if (symbol == 1)
				fp = fopen("students.txt", "wb");
			else
				fp = fopen("teachers.txt", "wb");
			for (i = 0; i < m; i++)
			{
				if (i + 1 != n)
					fwrite(&removeit[i], sizeof(struct Readeritem), 1, fp);
			}
			printf("-----删除成功-----\n");
		}
		fclose(fp);
	}
	else
		printf("-----不存在借书证号为%03d的读者！-----\n", id);
}

void udreader(int symbol)
{
	printf("输入你想修改的读者借书证号： ");
	int id;
	scanf("%d", &id);
	int n = searchreader(symbol, id), cho, i;
	if (n != 0)
	{
		struct Readeritem removeit[10],tempit;
		FILE *fp;
		if (symbol == 1)
			fp = fopen("students.txt", "rb");
		else
			fp = fopen("teachers.txt", "rb");
		fseek(fp, (n-1) * sizeof(struct Readeritem), 0);
		fread(&reader, sizeof(struct Readeritem), 1, fp);
		printf("%03d\t%s\t%s\t%d\t%d\t%d\n", reader.no, reader.name, reader.class, reader.sno, reader.level, reader.islost);
		rewind(fp);

		printf("借书证号： ");
		scanf("%d", &tempit.no);
		printf("姓名： ");
		scanf("%s", &tempit.name);
		printf("班级： ");
		scanf("%s", &tempit.class);
		printf("学号： ");
		scanf("%d", &tempit.sno);
		printf("是否挂失： ");
		scanf("%d", &tempit.islost);
		tempit.level = symbol;
		printf("[1]继续修改\t[2]取消并退出\n");
		scanf("%d", &cho);
		if (cho == 1)
		{
			int m = countreader(symbol);
			fread(&removeit, sizeof(struct Readeritem), m, fp);

			if (symbol == 1)
				fp = fopen("students.txt", "wb");
			else
				fp = fopen("teachers.txt", "wb");
			for (i = 0; i < m; i++)
			{
				if (i + 1 != n)
					fwrite(&removeit[i], sizeof(struct Readeritem), 1, fp);
				else
					fwrite(&tempit, sizeof(struct Readeritem), 1, fp);
			}
			printf("-----修改成功-----\n");
		}
		fclose(fp);
	}
	else
		printf("-----不存在借书证号为%03d的读者！-----\n", id);
}