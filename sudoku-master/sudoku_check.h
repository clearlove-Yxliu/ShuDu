#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//检查sudo是否符合要求，符合返回-1，否则返回错误的位置 
int c_is_suit(int sudo[][9])
{
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			if (sudo[x][y] <= 0 || sudo[x][y] > 9)
				return x*9+y;

			int num = sudo[x][y];

			for (int k = 0; k < 9; k++)//判断当前的行或者列有没有相同的数字
			{
				if ((k != y && sudo[x][k] == num) || (k != x && sudo[k][y] == num))
					return x*9+y;
			}

			//判断当前的宫是否有相同的数字
			for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
			{
				for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
				{
					if (sudo[i][j] == num && (!(i == x && j == y)))
						return x*9+y;
				}
			}
		}
	}


	return -1;
}


//检测文件filename中的数独是否正确，并打印结果
int is_right(char* filename)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, filename, "r");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", filename);
		return -1;
	}

	int file_check_result = 0;//整个文件的检查结果，完全无误则为0，否则为错误的数独数
	int sudoku_number = 0;//当前检查的数独的编号
	while (1)
	{
		int sudo[9][9] = { 0 };
		//获取81个数，如果未足81个数就已读到文件尾，则退出，返回0
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf_s(fp, "%c", &tem, 1) == -1)//读到文件尾
					{
						fclose(fp);

						if (file_check_result == 0)
						{
							printf("检查完毕，无误\n");
						}
						else
						{
							printf("共有 %d 个数独错误\n", file_check_result);
						}

						return sudoku_number;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';

			}
		}

		sudoku_number++;
		int sudoku_check_result = c_is_suit(sudo);//当前数独的检查结果，正确为-1，否则为正数
		
		if (sudoku_check_result != -1)
		{
			//发现的错误元素的行号和列号
			int error_row = sudoku_check_result / 9;
			int error_col = sudoku_check_result % 9;

			printf("第 %d 号数独第 %d 行第 %d 列有误\n", error_row, error_col);

		}
		else
			printf("第 %d 号数独检测无误\n", sudoku_number);


	}

}
