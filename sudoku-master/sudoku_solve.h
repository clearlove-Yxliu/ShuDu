#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>



//����ļ��ڵ�����
void s_clear_file(char* file_name)
{
	FILE *fp;
	//fp = fopen(file_name, "w");
	errno_t open_error = fopen_s(&fp, file_name, "w");//�򿪳ɹ����ط��㣬ʧ�ܷ���0
	if (open_error)
	{
		printf("���ļ� %s ʧ��\n", file_name); 
		return;
	}

	fclose(fp);
}

//��old_sudo���Ƶ�new_sudo
void copy_sudo(int old_sudo[][9], int new_sudo[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			new_sudo[i][j] = old_sudo[i][j];
		}
	}
}

//�ж���sudo[x][y]�Ϸ�������num�Ƿ���Ϲ������򷵻�1�����򷵻�0
int s_is_suit(int sudo[][9], int pos, int num)
{
	int x = pos / 9;
	int y = pos % 9;

	for (int k = 0; k < 9; k++)//�жϵ�ǰ���л�������û����ͬ������
	{
		if ((k != y && sudo[x][k] == num) || (k != x && sudo[k][y] == num))
			return 0;
	}

	//�жϵ�ǰ�Ĺ��Ƿ�����ͬ������
	for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
	{
		for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
		{
			if (sudo[i][j] == num && (!(i == x && j == y)))
				return 0;
		}
	}

	return 1;
}

//�ݹ飬��num���õ���λ��pos��0~80����,ֱ�����еĿ�λ�ñ���������õ���������result��
void place_num(int sudo[][9], int pos, int num, int result[][9])
{
	int copy[9][9] = { 0 };
	copy_sudo(sudo, copy);

	if (pos >= 0)//��ǰλ�úϷ�������λ����Ϊnum
		copy[pos / 9][pos % 9] = num;

	//�ҵ���һ����Ӧ������Ϊ0��λ��
	do
	{
		pos++;
		if (pos > 80)//��ǰ���������վ�
		{
			copy_sudo(copy, result);
			return;
		}
	} while (copy[pos / 9][pos % 9] != 0);

	//���Խ���λ�õ���һλ����Ϊn��n�ķ�Χ��1~9
	for (int n = 1; n <= 9; n++)
	{
		if (s_is_suit(copy, pos, n) == 1)//�����ǰλ����Ϊn���ʣ���ݹ�������һ��Ϊ0��λ��
		{
			place_num(copy, pos, n, result);
		}
	}

}

//��problem_filename�л�ȡ��������⣬���뵽result_filename
int solve_problem(char* problem_filename, char* result_filename)
{
	FILE *fp1;
	errno_t open_error = fopen_s(&fp1, problem_filename, "r");//�򿪳ɹ����ط��㣬ʧ�ܷ���0
	if (open_error)
	{
		printf("���ļ� %s ʧ��\n", problem_filename);
		return -1;
	}

	s_clear_file(result_filename);//��ռ�¼������ļ�
	FILE *fp2;
	open_error = fopen_s(&fp2, result_filename, "a+");//�򿪳ɹ����ط��㣬ʧ�ܷ���0
	if (open_error)
	{
		printf("���ļ� %s ʧ��\n", result_filename);
		return -1;
	}

	int sudoku_sum = 0;
	while (1)
	{
		int sudo[9][9] = { 0 };
		//��ȡ81���������δ��81�������Ѷ����ļ�β�����˳�������0
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf_s(fp1, "%c", &tem, 1) == -1)//�����ļ�β
					{
						fclose(fp1);
						fclose(fp2);
						return sudoku_sum;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		sudoku_sum++;

		int result[9][9] = { 0 };
		place_num(sudo, -1, 0, result);
		//����Ƿ��ѽ��
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (result[i][j] <= 0 || result[i][j] > 9)
				{
					printf("�� %d ���������޽�\n", sudoku_sum);
					continue;
				}
			}
		}

		//����������ת����һ���ַ���
		char sudoku_string[18 * 9 + 1] = { 0 };//�������ַ�����ʽ
		char se_string[18 + 1] = { 0 };//һ���������ַ�����ʽ
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				se_string[j * 2] = result[i][j] + '0';

				se_string[j * 2 + 1] = ' ';

				if (j != 8)
					se_string[j * 2 + 1] = ' ';
				else
					se_string[j * 2 + 1] = '\n';
			}
			se_string[18] = '\0';

			strcat_s(sudoku_string, se_string);//��copyƴ�ӵ������վ���
		}
		sudoku_string[18 * 9] = '\n';

		fputs(sudoku_string, fp2);//������ļ�
	}
}