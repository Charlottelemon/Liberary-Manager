#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <string.h>
#include "borrow.h"
int Maxstubw = 10;    //学生最多可借
int Maxteabw = 15;    //教师最多可借
float Delaytopay = 1;     //每逾期一天支付罚款
int Applyforday = 30;    //初次借阅以及续借的日数

struct Borrowitem
{
	int id;
	char name[18];
	float price;
	int no;
	int symbol;
	int level;
	int isreturn;
	time_t bowdate;   //time_t类型以long类型存储1970年迄今的秒数
	int retdate; 
	float payfor;
}bowitem;

int toMax(int level,int no)   //判断读者借书次数
{
	int num;
	if (level == 1)
		num = Maxstubw;
	else
		num = Maxteabw;
	FILE *fp = fopen("borrowed.txt", "rb");
	while (1)
	{
		int i = 1;
		i = fread(&bowitem, sizeof(struct Borrowitem), 1, fp);
		if (bowitem.level == level && bowitem.no == no && bowitem.isreturn == 0)
			num--;
		if (i == 0)
			break;
	}
	fclose(fp);

	return num;
}

int countbow()   //返回借阅表的记录总数
{
	FILE *fp;
	fp = fopen("borrowed.txt", "rb");
	struct Borrowitem count;
	int num = 0;
	while (1)
	{
		int n = 1;
		n = fread(&count, sizeof(struct Borrowitem), 1, fp);
		if (n == 0)
			break;
		num++;
	}
	fclose(fp);

	return num;
}

void tranDate(time_t t)   //将time_t类型转换成日历时间
{
	//t = time(NULL);
	char *time;
	time = ctime(&t);
	char str[10];  //将默认的时间格式截取成年-月-日模式
	str[0] = time[20];
	str[1] = time[21];
	str[2] = time[22];
	str[3] = time[23];
	str[4] = time[4];
	str[5] = time[5];
	str[6] = time[6];
	str[7] = time[8];
	str[8] = time[9];
	str[9] = '\0';

	printf("%s\t", str);
}

void borrowed(int level, int symbol, int id, int no)    //借书
{
	if (toMax(level, no) > 0)
	{
		printf("输入书名:");
		scanf("%s", &bowitem.name);
		printf("输入价格:");
		scanf("%f", &bowitem.price);
		bowitem.id = id;
		bowitem.no = no;
		bowitem.level = level;
		bowitem.symbol = symbol;
		bowitem.isreturn = 0;
		bowitem.bowdate = time(NULL);
		bowitem.retdate = 1;   //这里的1代表1*30*24*3600秒
		bowitem.payfor = 0;
		FILE *fp = fopen("borrowed.txt", "ab");
		fwrite(&bowitem, sizeof(struct Borrowitem), 1, fp);
		fclose(fp);
		printf("-----操作成功-----\n");
	}
	else
		printf("-----当前无借阅次数，请还书后再来-----\n");
}

void display()
{
	FILE *fp;
	fp = fopen("borrowed.txt", "rb");
	printf("书籍编号|借书证号|书籍类别|读者类别|\t书名\t| 价格 |  借书日期  |  还书日期  |是否归还|罚款金额\n");
	while (1)
	{
		int n = 0;
		n = fread(&bowitem, sizeof(struct Borrowitem), 1, fp);
		if (n == 0)
			break;

		printf("%03d\t%03d\t",bowitem.id,bowitem.no);
		if (bowitem.symbol == 1)
			printf("书籍");
		else if (bowitem.symbol == 2)
			printf("期刊");
		else
			printf("报刊");
		if (bowitem.level == 1)
			printf("\t学生");
		else
			printf("\t老师");
		printf("\t%s\t%.2f\t",bowitem.name,bowitem.price);
		tranDate(bowitem.bowdate);
		tranDate(bowitem.bowdate + bowitem.retdate*Applyforday*24*3600);
		if (bowitem.isreturn == 0)
			printf("否");
		else
			printf("是");
		printf("\t%.2f\n", bowitem.payfor);
	}
	fclose(fp);
	printf("\n");
}

int udRecord(int am,int bn,int signal)     //续借或还书
{
	int a[4],pos=1;
	a[0] = am;
	a[1] = bn;
	printf("输入读者类别（[1]学生[2]教师）以及借书证号：");
	scanf("%d%d", &a[2], &a[3]);
	int m = countbow(),j;
	struct Borrowitem udbow[20];    //不支持用变量初始化数组
	FILE *fp;
	fp = fopen("borrowed.txt", "rb");
	while (1)
	{
		int i = 1;
		i = fread(&bowitem, sizeof(struct Borrowitem), 1, fp);
		if (i == 0)
		{
			pos = -1;
			break;
		}
		else if (bowitem.symbol == a[0] && bowitem.id == a[1] && bowitem.level == a[2] && bowitem.no == a[3] && bowitem.isreturn == 0)   //只查找未归还的
		{
			printf("《%s》\t（借书证号）%03d\t（借书日期）", bowitem.name, bowitem.no);
			tranDate(bowitem.bowdate);
			printf("\t(还书日期)");
			tranDate(bowitem.bowdate + bowitem.retdate*Applyforday * 24 * 3600);
			printf("\n");
			break;
		}
		pos++;   //标记位置
		
	}
	
	rewind(fp);    //文件指针回到文件头
	fread(&udbow, sizeof(struct Borrowitem), m, fp);     //批量写入
	fp = fopen("borrowed.txt", "wb");
	if (pos != -1 && signal == 1)
	{
		if (bowitem.retdate == 1)   //判断之前是否续借过
		{
			//struct Borrowitem *udbow = (struct Borrowitem *)malloc(m * sizeof(struct Borrowitem));   
			bowitem.retdate++;
			for (j = 0; j < m; j++)
			{
				if (j + 1 != pos)
					fwrite(&udbow[j], sizeof(struct Borrowitem), 1, fp);
				else
					fwrite(&bowitem, sizeof(struct Borrowitem), 1, fp);
			}
			printf("-----续借成功-----\n");
		}
		else
		{
			printf("-----操作失败，只能续借一次-----\n");
			fwrite(&udbow, sizeof(struct Borrowitem), m, fp);   //续借失败，数据原封不动写回
		}
	}
	else if (pos != -1 && signal == 2)
	{
		time_t now = time(NULL);   //获取当前时间
		//struct Borrowitem *udbow = malloc(m * sizeof(struct Borrowitem));   
		bowitem.isreturn = 1;
		if (now - bowitem.bowdate > bowitem.retdate*Applyforday*24*3600)   //与还书时间做对比
			bowitem.payfor = ((now - bowitem.bowdate) / (24 * 3600)-bowitem.retdate*Applyforday)*Delaytopay;  //计算罚款金额
		for (j = 0; j < m; j++)
		{
			if (j + 1 != pos)
				fwrite(&udbow[j], sizeof(struct Borrowitem), 1, fp);
			else
				fwrite(&bowitem, sizeof(struct Borrowitem), 1, fp);
		}
		showDelay(a[2], a[3]);    //显示该读者逾期未还的书
		printf("-----操作成功-----\n");
	}
	else
	{
		printf("-----没有相关借阅记录-----\n"); 
		fwrite(&udbow, sizeof(struct Borrowitem), m, fp);
	}
	fclose(fp);
	return pos;
}

void showDelay(int level,int no)      //显示到期未归还的书,(-1,-1)显示所有用户信息，其他情况显示单用户信息
{
	FILE *fp;
	fp = fopen("borrowed.txt", "rb");
	time_t now = time(NULL);    //获取当前时间
	struct Borrowitem bwitem;
	if (level == -1 && no == -1)
	{
		printf("书籍编号|  书名\t|借阅证号|  借书日期  |  还书日期  |罚款金额\n");
		while (1)
		{
			int n = 1;
			n = fread(&bwitem, sizeof(struct Borrowitem), 1, fp);
			if (n != 0 && now > bwitem.bowdate + bwitem.retdate*Applyforday * 24 * 3600)
			{
				printf(" %03d  %s   %d\t", bwitem.id, bwitem.name, bwitem.no);
				tranDate(bwitem.bowdate);
				printf("\t");
				tranDate(bwitem.bowdate + bwitem.retdate*Applyforday * 24 * 3600);
				printf("\t%.2f\n", bwitem.payfor);
			}
			if (n == 0)
				break;
		}
	}
	else
	{
		printf("目前你逾期未归还的书--\n书籍编号|   书名\t|借阅证号|  借书日期  |  还书日期  |罚款金额\n");
		while (1)
		{
			int n = 1;
			n = fread(&bwitem, sizeof(struct Borrowitem), 1, fp);
			if (n != 0 && bwitem.no == no && bwitem.level == level && (bwitem.bowdate + bwitem.retdate*Applyforday * 24 * 3600) < now)
			{
				printf(" %03d  %s   %d\t", bwitem.id, bwitem.name, bwitem.no);
				tranDate(bwitem.bowdate);
				printf("\t");
				tranDate(bwitem.bowdate + bwitem.retdate*Applyforday * 24 * 3600);
				printf("\t%.2f\n", bwitem.payfor);
			}
			if (n == 0)
				break;
		}
	}
	printf("\n");
	fclose(fp);
}

/*void ranking(int id, int symbol)    //借阅排行
{
	//遍历整张借阅表，建立一个数组，出现次数最多的排第一
}*/
