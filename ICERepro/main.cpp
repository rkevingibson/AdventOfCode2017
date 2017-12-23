#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>


int main()
{
	LARGE_INTEGER start, end, freq;

	QueryPerformanceCounter(&start);
	
	for (int i = 0; i < 1000; i++) {
		std::cout << "1" << std::endl;

	}
	QueryPerformanceCounter(&end);
	QueryPerformanceFrequency(&freq);
	double time_elapsed = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
	std::cout << "Took " << time_elapsed << "s" << std::endl;

}