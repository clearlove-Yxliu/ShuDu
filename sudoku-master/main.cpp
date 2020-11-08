#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

#include"sudoku_generator.h"
#include"sudoku_solve.h"
#include"sudoku_check.h"

//将num转化为int，如果不合法，返回-1 
int transform_into_int(char* num)
{
	int n = 0;
	char n_t[10] = { 0 };
	strcpy_s(n_t, num);
	int len = strlen(n_t);
	for (int i = 0; i < len; i++)//将char数组转化为数字，如果为不合法的数字则退出
	{
		if (!(n_t[i] >= '0'&&n_t[i] <= '9'))
		{
			return -1;
		}
		else
			n = n * 10 + (n_t[i] - '0');
	}

	return n;
}

int main(int argc, char *argv[])
{
	if (argc < 1)
		return 1;

	clock_t start, finish;
	double totaltime;

	//判断接下来将要执行的命令
	char order[10] = { 0 };
	strcpy_s(order, argv[1]);
	switch (order[1])
	{
		case 'c':
		{
			int N = transform_into_int(argv[2]);
			if (N == -1)
			{
				printf("输入了错误的参数\n");
				system("pause");

				return 1;
			}
			else
			{
				start = clock();

				int n = generate_sudoku(N, "sudoku.txt", 0);//生成n个数独终局并打印到sudoku.txt

				printf("生成了 %d 个数独终局\n", n);
			}
		}; break;

		case 's':
		{
			start = clock();

			int n = solve_problem(argv[2], "sudoku.txt");//解决argv[2]文件中的数独题目并输出到sudoku.txt

			printf("解决了 %d 道数独题\n", n);
		}; break;

		case 'x':
		{
			int N = transform_into_int(argv[2]);
			if (N == -1)
			{
				printf("输入了错误的参数\n");
				system("pause");

				return 1;
			}
			else
			{
				start = clock();

				int n = generate_sudoku(N, argv[3], 1);//生成N个数独题并打印到argv[3]

				printf("生成了 %d 道数独题\n", n);
			}
		}; break;

		case 'y':
		{
			start = clock();

			int n = is_right(argv[2]);

			printf("检测了 %d 个数独终局\n", n);		
		}; break;

		default:
		{
			start = clock();

			printf("输入了错误的参数\n");
			system("pause");
			return 1;
		};break;
			
	}


	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;

	printf("此程序的运行时间为 %.3lf 秒\n", totaltime);

	system("pause");
}
