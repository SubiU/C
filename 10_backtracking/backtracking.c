
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }


#if DEF_FUNC("PATH")
// 矩阵中的路径
// 题目：请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有
// 字符的路径。路径可以从矩阵中任意一格开始，每一步可以在矩阵中向左、右、
// 上、下移动一格。如果一条路径经过了矩阵的某一格，那么该路径不能再次进入
// 该格子。例如在下面的3×4的矩阵中包含一条字符串“bfce”的路径（路径中的字
// 母用下划线标出）。但矩阵中不包含字符串“abfb”的路径，因为字符串的第一个
// 字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入这个格子。
// a b t g
// c f c s
// j d e h

bool hasPathCore(const char *matrix, int rows, int cols, int row, int col, int pathLength, const char *str, char *visited)
{
	printf_f(("row(%d)\tcol(%d)\tpathLength(%d)\n", row, col, pathLength));
	if(str[pathLength]=='\0')
		return true;

	if(row<0 || col<0)
		return false;
	
	bool hasPath = false;
	if(row>=0 && row<rows && col>=0 && col<cols && matrix[row*cols+col]==str[pathLength] && !visited[row*cols+col]) {
		pathLength++;
		visited[row*cols+col] = true;
	
		hasPath = hasPathCore(matrix, rows, cols, row, col-1, pathLength, str, visited) ||
			hasPathCore(matrix, rows, cols, row, col+1, pathLength, str, visited) ||
			hasPathCore(matrix, rows, cols, row-1, col, pathLength, str, visited) ||
			hasPathCore(matrix, rows, cols, row+1, col, pathLength, str, visited);

		if(!hasPath) {
			pathLength--;
			visited[row*cols+col] = false;
		}
	}

	return hasPath;
}

bool hasPath(char *matrix, int rows, int cols, char *str)
{
	if(matrix==NULL || rows<1 || cols<1 || str==NULL)
		return false;

	int row = 0, col = 0;
	int pathLength = 0;
	char visited[rows*cols];
	memset(visited, 0, rows*cols);
	
	for(row=0; row<rows; row++) {
		for(col=0; col<cols; col++) {
			if(hasPathCore(matrix, rows, cols, row, col, pathLength, str, visited))
				return true;
		}
	}
	return false;
}
#endif

#if DEF_FUNC("MOVE")
int digit_sum(int nums)
{
	int sum = 0;
	while(nums>0) {
		sum += nums%10;
		nums /= 10;
	}
	return sum;
}

bool check(int rows, int cols, int row, int col, int threshold, bool *visited)
{
	if(row>=0 && row<rows && col>=0 &&col<cols && (digit_sum(row)+digit_sum(col)<=threshold)
		&& !visited[row*cols+col])
		return true;
 
	return false;
}

int move_count_core(int rows, int cols, int row, int col, int threshold, bool *visited)
{
	int count = 0;
	printf_f(("row(%d)\tcol(%d)\tvisited(%d)\n",row,col,visited[row*cols+col]));
//	sleep(1);
	if(check(rows, cols, row, col, threshold, visited)) {
		visited[row*cols+col] = true;
		count = 1 + move_count_core(rows, cols, row-1, col, threshold, visited) +
			move_count_core(rows, cols, row, col-1, threshold, visited) +
			move_count_core(rows, cols, row+1, col, threshold, visited) +
			move_count_core(rows, cols, row, col+1, threshold, visited);
	}	

	printf_f(("count(%d)\n",count));

	return count;
}

int move_count(int rows, int cols, int threshold)
{
	if(rows<=0 || cols<=0 || threshold<0)
		return 0;

	bool visited[rows*cols];
	memset(visited, 0, rows*cols);

	return move_count_core(rows, cols, 0, 0, threshold, visited);
}
#endif

int main(void)
{

#if DEF_FUNC_CANCEL("PATH")
	char *str = "bfce";
	char *matrix = "abtgcfcsjdeh";

	bool result = hasPath(matrix, 3, 4, str);
	printf("Result : %s\n", (result? "hasPath":"has no path"));
#endif

#if DEF_FUNC("MOVE")
	int count = 0;
	count = move_count(5, 4, 5);
	printf("count : %d\n", count);
#endif

	return 0;
}
