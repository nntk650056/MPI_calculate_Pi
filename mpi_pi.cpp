#include<iostream>
#include"mpi.h"
#include<ctime>
#include<cmath>
#include <iomanip>
using namespace std;


const int N = 1E9;
double start, finish;//计算的起止时间
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);//MPI库的初始化
	int numprocs, myid;//定义进程总数及进程标识
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);//获得进程数
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);//获得当前进程标识号0,1,2,3,....,numprocs - 1

	start = MPI_Wtime();//获得时间
	long double partSum = 0.0;//定义部分和
	
    long double x, y, z=0.0;
    long double num = 0;
	long double pi = 0.0;//定义pi的值

	for (int j = 0; j < 2000; j++)
	{
		int k = 0;
		srand((int)time(NULL));  // 产生随机种子  把0换成NULL也行
		for (int i = myid; i < N; i += numprocs)
		{
			x = rand() / long double(RAND_MAX);
			y = rand() / long double(RAND_MAX);
			z = x * x + y * y;
			if (z <= 1)
			{
				k++;
			}

			//partSum += sqrt(1.0 - (double)(i * i) / (N * N))/N; int型乘法会有溢出问题
		}
		num  = num + long double(k) / 1E8;
		partSum = num / long double(10* (j + 1));
		MPI_Reduce(&partSum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		//规约操作，将各进程的partSum发送到0号进程并求和
		cout << "目前num的值为："<<num << endl;
		cout << "我的标识是" << myid << ",求得的partSum值为:" << setprecision(9) << partSum << endl;


		if (myid == 0)
		{
			pi *= 4.0;
			finish = MPI_Wtime();
			cout << "计算" << j+1 << ",000,000,000 次" << endl;
			cout << "求得近似的pi值为:" << setprecision(9) << pi << endl;
			cout << "调用" << numprocs << "个进程的计算时间为:" << finish - start << " s" << endl;
			cout << '\n';
		}

		if (fabs(pi - 3.1415926 )< 1E-7)
			break;
	}

	MPI_Finalize();
	//system("pause");
	return 0;
}
