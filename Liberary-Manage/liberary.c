#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liberary.h"

struct Bookitem
{
	int id;
	char name[20];
	char author[10];
	char publish[20];
	char date[8];
	float price;
	int class;
	int totalnum;
	int nownum;
	int borrowed;
}bookitem;

int searchitem(int symbol,int id)    //查找并返回图书的位置
{
	FILE *fp;
	struct Bookitem ifexist;
	int pos = 1;  //标记该编号的位置
	if (symbol == 1)
		fp = fopen("books.txt", "rb");
	else if (symbol == 2)
		fp = fopen("papers.txt", "rb");
	else
		fp = fopen("newspaper.txt", "rb");
	while (1)
	{
		int n = 0;
		n = fread(&ifexist, sizeof(struct Bookitem), 1, fp);
		if (n == 0)
		{
			pos = 0;
			break;
		}
		else if (ifexist.id == id)
			break;
		pos++;
	}
	fclose(fp);

	return pos;
}

void additem(int symbol)      //添加图书
{
	printf("编号： ");
	scanf("%d", &bookitem.id);
	printf("书名： ");
	scanf("%s", &bookitem.name);
	printf("作者： ");
	scanf("%s", &bookitem.author);
	printf("出版社： ");
	scanf("%s", &bookitem.publish);
	printf("出版时间： ");
	scanf("%s", &bookitem.date);
	printf("价格： ");
	scanf("%f", &bookitem.price);
	printf("总入库数量： ");
	scanf("%d", &bookitem.totalnum);
	printf("当前库存： ");
	scanf("%d", &bookitem.nownum);
	printf("已借出数量： ");
	scanf("%d", &bookitem.borrowed);
	bookitem.class = symbol;

	if (searchitem(symbol, bookitem.id) == 0)
	{
		if (symbol == 1)
		{
			FILE *fp = fopen("books.txt", "ab");    //文件必须和源程序在同一子目录
			if (fp != NULL)
			{
				fwrite(&bookitem, sizeof(struct Bookitem), 1, fp);
				fclose(fp);
			}
			else
				printf("error\n");
		}
		else if (symbol == 2)
		{
			FILE *fp = fopen("papers.txt", "ab");
			fwrite(&bookitem, sizeof(struct Bookitem), 1, fp);
			fclose(fp);
		}
		else
		{
			FILE *fp = fopen("newspaper.txt", "ab");
			fwrite(&bookitem, sizeof(struct Bookitem), 1, fp);
			fclose(fp);
		}
		printf("-----添加成功-----\n");
	}
	else
		printf("-----------编号重复，请重新输入！\n");

}

void getitem(int no)     //获取图书
{
	FILE *fp;
	printf("编号  书名      作者  出版社  出版时间  价格  （书籍1/期刊2/报刊3）  入库总数  当前库存  已借出数量\n");
	if (no == 1)
		fp = fopen("books.txt", "rb");
	else if (no == 2)
		fp = fopen("papers.txt", "rb");
	else
		fp = fopen("newspaper.txt", "rb");
	while (1)
	{
		int n=0;
		n = fread(&bookitem, sizeof(struct Bookitem), 1, fp);
		if (n == 0)
			break;

		printf("%03d\t%s\t%s\t%s\t%s\t%.2f\t%d\t %d\t %d\t %d\n", bookitem.id, bookitem.name, bookitem.author, bookitem.publish, bookitem.date,
			bookitem.price, bookitem.class, bookitem.totalnum, bookitem.nownum, bookitem.borrowed);
	}
	fclose(fp);
	printf("\n");
}

int countitem(int symbol)     //返回文件中图书的数量
{
	FILE *fp;
	int num = 0;  
	if (symbol == 1)
		fp = fopen("books.txt", "rb");
	else if (symbol == 2)
		fp = fopen("papers.txt", "rb");
	else
		fp = fopen("newspaper.txt", "rb");
	while (1)
	{
		int n = 0;
		n = fread(&bookitem, sizeof(struct Bookitem), 1, fp);
		if (n == 0)
			break;
		num++;
	}
	fclose(fp);

	return num;
}

void removeitem(int symbol)    //删除图书记录
{
	printf("输入你想删除的书籍编号： ");
	int id;
	scanf("%d", &id);
	int n = searchitem(symbol, id),i,cho;
	if (n != 0)
	{
		struct Bookitem removeit[10];
		FILE *fp;
		if (symbol == 1)
			fp = fopen("books.txt", "rb");
		else if (symbol == 2)
			fp = fopen("papers.txt", "rb");
		else
			fp = fopen("newspaper.txt", "rb");
		fseek(fp, (n-1)*sizeof(struct Bookitem),0);     //将文件指针定位到要删除的那条记录
		fread(&bookitem, sizeof(struct Bookitem), 1, fp);
		printf("%03d\t%s\t%s\t%s\t%s\t%.2f\t%d\t%d\t%d\t%d\n", bookitem.id, bookitem.name, bookitem.author, bookitem.publish, bookitem.date,
			bookitem.price, bookitem.class, bookitem.totalnum, bookitem.nownum, bookitem.borrowed);
		rewind(fp);
		printf("[1]继续删除\t[2]取消并退出\n");
		scanf("%d", &cho);
		if (cho == 1)
		{
			int m = countitem(symbol);
			fread(&removeit, sizeof(struct Bookitem), m, fp);

			if (symbol == 1)
				fp = fopen("books.txt", "wb");
			else if (symbol == 2)
				fp = fopen("papers.txt", "wb");
			else
				fp = fopen("newspaper.txt", "wb");
			for (i = 0; i < m; i++)
			{
				if (i + 1 != n)
					fwrite(&removeit[i], sizeof(struct Bookitem), 1, fp);
			}
			printf("-----删除成功-----\n");
		}
		fclose(fp);
	}
	else
		printf("-----不存在编号为%03d的书籍！-----\n", id);
}

void updateitem(int symbol)     //修改信息，与删除信息相似
{
	printf("输入你想修改的书籍编号： ");
	int id;
	scanf("%d", &id);
	int n = searchitem(symbol, id),cho,i;
	if (n != 0)
	{
		struct Bookitem removeit[10],tempitem;
		FILE *fp;
		if (symbol == 1)
			fp = fopen("books.txt", "rb");
		else if (symbol == 2)
			fp = fopen("papers.txt", "rb");
		else
			fp = fopen("newspaper.txt", "rb");
		fseek(fp, (n-1) * sizeof(struct Bookitem), 0);
		fread(&bookitem, sizeof(struct Bookitem), 1, fp);
		printf("%03d\t%s\t%s\t%s\t%s\t%.2f\t%d\t%d\t%d\t%d\n", bookitem.id, bookitem.name, bookitem.author, bookitem.publish, bookitem.date,
			bookitem.price, bookitem.class, bookitem.totalnum, bookitem.nownum, bookitem.borrowed);
		rewind(fp);

		printf("编号： ");
		scanf("%d", &tempitem.id);
		printf("书名： ");
		scanf("%s", &tempitem.name);
		printf("作者： ");
		scanf("%s", &tempitem.author);
		printf("出版社： ");
		scanf("%s", &tempitem.publish);
		printf("出版时间： ");
		scanf("%s", &tempitem.date);
		printf("价格： ");
		scanf("%f", &tempitem.price);
		printf("总入库数量： ");
		scanf("%d", &tempitem.totalnum);
		printf("当前库存： ");
		scanf("%d", &tempitem.nownum);
		printf("已借出数量： ");
		scanf("%d", &tempitem.borrowed);
		tempitem.class = symbol;
		printf("[1]继续修改\t[2]取消并退出\n");
		scanf("%d", &cho);
		if (cho == 1)
		{
			int m = countitem(symbol);
			fread(&removeit, sizeof(struct Bookitem), m, fp);

			if (symbol == 1)
				fp = fopen("books.txt", "wb");
			else if (symbol == 2)
				fp = fopen("papers.txt", "wb");
			else
				fp = fopen("newspaper.txt", "wb");
			for (i = 0; i < m; i++)
			{
				if (i + 1 != n)
					fwrite(&removeit[i], sizeof(struct Bookitem), 1, fp);
				else
					fwrite(&tempitem, sizeof(struct Bookitem), 1, fp);
			}
			printf("-----修改成功-----\n");
		}
		fclose(fp);
	}
	else
		printf("-----不存在编号为%03d的书籍！-----\n", id);
}

int udbrrowed(int symbol,int id,int aord)    //每次借书或还书后更新图书库存
{
	int n = searchitem(symbol, id), m = countitem(symbol),i;
	//struct Bookitem *removeit = malloc(m*sizeof(struct Bookitem));
	struct Bookitem removeit[10];
	FILE *fp;
	if (symbol == 1)
		fp = fopen("books.txt", "rb");
	else if (symbol == 2)
		fp = fopen("papers.txt", "rb");
	else
		fp = fopen("newspaper.txt", "rb");
	fseek(fp, (n - 1) * sizeof(struct Bookitem), 0);
	fread(&bookitem, sizeof(struct Bookitem), 1, fp);
	rewind(fp);
	fread(&removeit, sizeof(struct Bookitem), m, fp);
	if (aord == 1 && bookitem.nownum > 0)    //借书，当前数量不能为0
	{
		bookitem.nownum -= 1;
		bookitem.borrowed += 1;
		if (symbol == 1)
			fp = fopen("books.txt", "wb");
		else if (symbol == 2)
			fp = fopen("papers.txt", "wb");
		else
			fp = fopen("newspaper.txt", "wb");
		for (i = 0; i < m; i++)
		{
			if (i + 1 != n)
				fwrite(&removeit[i], sizeof(struct Bookitem), 1, fp);
			else
				fwrite(&bookitem, sizeof(struct Bookitem), 1, fp);
		}
		fclose(fp);
		return 1;
	}
	else if (aord == -1 && bookitem.borrowed > 0)    //还书，已借出数量不能为0
	{
		bookitem.nownum += 1;
		bookitem.borrowed -= 1;
		if (symbol == 1)
			fp = fopen("books.txt", "wb");
		else if (symbol == 2)
			fp = fopen("papers.txt", "wb");
		else
			fp = fopen("newspaper.txt", "wb");
		for (i = 0; i < m; i++)
		{
			if (i + 1 != n)
				fwrite(&removeit[i], sizeof(struct Bookitem), 1, fp);
			else
				fwrite(&bookitem, sizeof(struct Bookitem), 1, fp);
		}
		fclose(fp);
		return 1;
	}
	else
	{
		if(aord == 1)
			printf("-----该书当前没有库存-----\n");
		else
			printf("-----操作非法，没有借阅记录-----\n");
		fclose(fp);
		return 0;
	}
	
}