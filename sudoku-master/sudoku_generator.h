#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>

using namespace std;

//记录移动的方式。每行记录生成一个数独时需要进行的操作。 
//例如，第一行 0,3,6,1,4,7,2,5,8 表示，数独第一行由数列se向右移动0位得到；数独第二行由se向右移动3位得到……
//此数组共有30行，代表通过一个数列可以获得30个数独。
int move_way[30][9] = {
	{ 0,3,6,1,4,7,2,5,8 },
	{ 0,3,6,1,7,4,2,5,8 },
	{ 0,3,6,4,1,7,2,5,8 },
	{ 0,3,6,4,7,1,2,5,8 },
	{ 0,3,6,7,1,4,2,5,8 },


	{ 0,3,6,1,4,7,2,8,5 },
	{ 0,3,6,1,4,7,5,2,8 },
	{ 0,3,6,1,4,7,5,8,2 },
	{ 0,3,6,1,4,7,8,2,5 },
	{ 0,3,6,1,4,7,8,5,2 },

	{ 0,3,6,1,7,4,2,8,5 },
	{ 0,3,6,4,1,7,5,2,8 },
	{ 0,3,6,4,7,1,5,8,2 },
	{ 0,3,6,7,4,1,8,2,5 },
	{ 0,3,6,7,1,4,8,5,2 },

	{ 0,6,3,1,4,7,2,5,8 },
	{ 0,6,3,1,7,4,2,5,8 },
	{ 0,6,3,4,1,7,2,5,8 },
	{ 0,6,3,4,7,1,2,5,8 },
	{ 0,6,3,7,1,4,2,5,8 },


	{ 0,6,3,1,4,7,2,8,5 },
	{ 0,6,3,1,4,7,5,2,8 },
	{ 0,6,3,1,4,7,5,8,2 },
	{ 0,6,3,1,4,7,8,2,5 },
	{ 0,6,3,1,4,7,8,5,2 },

	{ 0,6,3,1,7,4,2,8,5 },
	{ 0,6,3,4,1,7,5,2,8 },
	{ 0,6,3,4,7,1,5,8,2 },
	{ 0,6,3,7,4,1,8,2,5 },
	{ 0,6,3,7,1,4,8,5,2 },
};

//将数列se向右移动n位，将移动后的结果转化为符合要求的格式并存入result
void move_se(char* se, char* result, int n)
{
	for (int i = 0; i < n; i++)
	{
		result[i * 2] = se[9 - n + i];
		result[i * 2 + 1] = ' ';
	}

	for (int i = n; i < 9; i++)
	{
		result[i * 2] = se[i - n];
		result[i * 2 + 1] = ' ';
	}
	result[17] = '\n';
}

//把字符串形式的数独终局转化为数独题
void change_into_problem(char* sudoku_string)
{
	srand((unsigned int)(time(NULL)));

	//总挖空数
	int blank_sum = 30 + rand() % 30;

	//每个宫挖空两个格
	for (int x = 0; x <= 2; x++)
	{
		for (int y = 0; y <= 2; y++)
		{
			int is_exit[9] = { 0 };
			int pos;
			for (int t = 1; t <= 2; t++)
			{
				do
				{
					pos = rand() % 9;
				} while (is_exit[pos] == 1);

				is_exit[pos] = 1;

				int row = x * 3 + pos / 3;
				int col = y * 3 + pos % 3;
				col *= 2;

				sudoku_string[row * 18 + col] = '0';

			}
		}
	}

	//随机挖空直到达到总挖空数
	for (int i = 1; i <= blank_sum - 2 * 9; i++)
	{
		int pos, row, col;

		do
		{
			pos = rand() % 81;
			row = pos / 9;
			col = pos % 9;
			col *= 2;
		} while (sudoku_string[row * 18 + col] == '0');

		sudoku_string[row * 18 + col] = '0';
	}
}

//生成N个数独到文件file_name中。如果generate_way = 0则生成终局，为1生成数独题
int generate_sudoku(int N, char* file_name, int generate_way)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//打开成功返回非零，失败返回0

	char se[9] = { '2','1','3','4','5','6','7','8','9' };//学号后两位为5、5，(5+5)%9+1=2，所以数列首位为2


	int sudoku_sum = 0;//已生成的数独终局数

	while (1)
	{
		next_permutation(&se[1], &se[1] + 8);//对se的第二位到第九位进行全排列变换，得到一个新数列

		for (int i = 0; i < 30; i++)//对每个数列，生成30个数独
		{
			char sudoku_string[18 * 9 + 1] = { 0 };//一个数独的字符串形式

			for (int j = 0; j < 9; j++)
			{
				//按照移动表来移动数列se，移动后的结果拼接在sudoku_string后面
				move_se(se, &sudoku_string[strlen(sudoku_string)], move_way[i][j]);
			}
			sudoku_string[18 * 9] = '\n';

			if (generate_way == 1)
				change_into_problem(sudoku_string);


			sudoku_sum++;

			if (sudoku_sum == N)//已生成足够数目的数独
			{
				sudoku_string[18 * 9 - 1] = '\0';
				sudoku_string[18 * 9] = '\0';
				fputs(sudoku_string, fp);//输出数独字符串到文件
				fclose(fp);
				return N;
			}

			fputs(sudoku_string, fp);//输出数独字符串到文件
		}

	}

}

