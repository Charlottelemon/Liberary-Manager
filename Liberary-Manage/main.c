#include <stdio.h>
//#include <windows.h>
#include <stdlib.h>
#include "liberary.h"
#include "reader.h"
#include "borrow.h"

int main()
{
	//生成菜单
	while (1)
	{
		int str = 0,s;
		printf("----------%d%20d图书馆管理系统%d%20d----------\n",str,str,str,str);
		printf("\n%31d-输入你的选择-\n%31d[1]图书类别管理\n%31d[2]读者类别管理\n%31d[3]借阅登记管理\n%31d[4]修改系统参数\n%31d[0]退出系统\n",str,str,str,str,str,str);
		printf("input:");
		scanf("%d", &s);
		if (s == 1)
		{
			while (1)
			{
				printf("[1]添加图书\t[2]删除图书\t[3]修改信息\t[4]查看图书\t[0]退出\n");
				scanf("%d", &s);
				if (s == 1)
				{
					printf("选择图书类别： [1]书籍\t[2]期刊\t[3]报刊\n");
					scanf("%d", &s);
					additem(s);
				}
				else if (s == 2)
				{
					printf("选择图书类别： [1]书籍\t[2]期刊\t[3]报刊\n");
					scanf("%d", &s);
					removeitem(s);
				}
				else if (s == 3)
				{
					printf("选择图书类别： [1]书籍\t[2]期刊\t[3]报刊\n");
					scanf("%d", &s);
					updateitem(s);
				}
				else if (s == 4)
				{
					printf("选择图书类别： [1]书籍\t[2]期刊\t[3]报刊\n");
					scanf("%d", &s);
					getitem(s);
				}
				else
					break;
			}
		}
		else if (s == 2)
		{
			while (1)
			{
				printf("[1]信息录入\t[2]注销读者\t[3]修改信息\t[4]查看读者\t[0]退出\n");
				scanf("%d", &s);
				if (s == 1)
				{
					printf("选择读者类别： [1]学生\t[2]教师\n");
					scanf("%d", &s);
					setreader(s);
				}
				else if (s == 2)
				{
					printf("选择读者类别： [1]学生\t[2]教师\n");
					scanf("%d", &s);
					remreader(s);
				}
				else if (s == 3)
				{
					printf("选择读者类别： [1]学生\t[2]教师\n");
					scanf("%d", &s);
					udreader(s);
				}
				else if (s == 4)
				{
					printf("选择读者类别： [1]学生\t[2]教师\n");
					scanf("%d", &s);
					getreader(s);
				}
				else
					break;
			}
		}
		else if (s == 3)
		{
			while (1)
			{
				printf("[1]借书\t[2]续借\t[3]还书\t[4]查看全部信息\t[5]逾期未还信息\t[0]退出\n");
				scanf("%d", &s);
				if (s == 1)
				{
					int a[4];
					printf("输入书的类别（[1]书籍[2]期刊[3]报刊）以及书的编号：");
					scanf("%d%d", &a[0], &a[1]);
					if (searchitem(a[0], a[1]) != 0)
					{
						if (udbrrowed(a[0], a[1], 1) == 1)
						{
							printf("输入读者类别（[1]学生[2]教师）以及借书证号：");
							scanf("%d%d", &a[2], &a[3]);
							if (searchreader(a[2], a[3]) > 0)
							{
								borrowed(a[2], a[0], a[1], a[3]);
							}
							else if (searchreader(a[2], a[3]) == 0)
							{
								printf("-----不存在这个借书证号-----\n");
								udbrrowed(a[0], a[1], -1);   //放弃操作，把文件改写回来
							}
							else
							{
								printf("-----此借书证已被挂失-----\n");
								udbrrowed(a[0], a[1], -1);
							}
						}
					}
					else
						printf("-----不存在编号为%03d的书籍！-----\n", a[1]);
				}
				else if (s == 2)   //最多只能续借一次
				{
					int m, n;
					printf("输入书的类别（[1]书籍[2]期刊[3]报刊）以及书的编号：");
					scanf("%d%d", &m, &n);
					udRecord(m,n,1);
				}
				else if (s == 3)
				{
					int m, n;
					printf("输入书的类别（[1]书籍[2]期刊[3]报刊）以及书的编号：");
					scanf("%d%d", &m, &n);
					if(udRecord(m, n, 2) != -1)
						udbrrowed(m,n,-1);    //修改完借阅表后紧接着修改图书表的数量
				}
				else if (s == 4)
				{
					display();
				}
				else if (s == 5)
				{
					printf("-----显示所有到期未归还的借阅信息-----\n");
					showDelay(-1, -1);
				}
				else
					break;
			}
		}
		else if (s == 4)   //这里采用的是改变全局变量的方式，只能在本次运行改变初始值，要永久改变需要设置文件记录
		{
			while (1)
			{
				printf("[1]学生最多可借本数\t[2]教师最多可借本数\t[3]续借日数\t[4]延期每日罚款\t[0]退出\n");
				scanf("%d", &s);
				if (s == 1)
				{
					printf("Now : %d\tSet :",Maxstubw);
					scanf("%d", &s);
					Maxstubw = s;
				}
				else if (s == 2)
				{
					printf("Now : %d\tSet :",Maxteabw);
					scanf("%d", &s);
					Maxteabw = s;
				}
				else if (s == 3)
				{
					printf("Now : %d\tSet :",Applyforday);
					scanf("%d", &s);
					Applyforday = s;
				}
				else if (s == 4)
				{
					float st;
					printf("Now : %.2f\t  Set :",Delaytopay);
					scanf("%f", &st);
					Delaytopay = st;
				}
				else
					break;
			}
		}
		else
			break;
		
	}

	return 0;
}
