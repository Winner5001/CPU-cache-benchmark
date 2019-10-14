#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

uint32_t xorshift32()
{
    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    static uint32_t state = 1567095237;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int func(int n, int d, int r)
{
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int duration;
    int* arr;
    arr = new int[n];
    for (int i=0; i<n; i++)
    {
        arr[i]= (i+d) % n;
    }
    int i = 0;
    t1 = high_resolution_clock::now();
    while(r-- > 0)
    {
        i = arr[i];
    }
    t2 = high_resolution_clock::now();
    cout << i << "\n";
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    delete[] arr;
    return duration;
}

int rand_access(int n, int accesses)
{
    // Initialize the array
    int* arr = new int[n];
    for (int i=0; i<n; i++)
    {
        arr[i]= i;
    }

    int accum = 0; // We accumulate in this variable and print so that the compile can't optimize the accesses away
    high_resolution_clock::time_point start_time = high_resolution_clock::now();
    for (int i=0; i<accesses; i++)
    {
        accum += arr[xorshift32()%n];
    }
    high_resolution_clock::time_point end_time = high_resolution_clock::now();
    cout << accum << endl;

    int duration = duration_cast<microseconds>( end_time - start_time ).count();
    delete[] arr;
    return duration;
}

int main()
{
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int duration;
    int sum = 0;
    string line;
    int r = 1000000;
    int d;
    int n;
    int times[13][25];
    int temp = 1;
    for (int h = 0; h <= 12; h++)
    {
        for (int b = 0; b < 25; b++)
        {
            times[h][b] = 0;
        }
    }
    for (int f = 0; f < 5; f++)
    {
        d = 1;
        for (int v = 0; v < 12; v++)
        {
            n = 2;
            for (int k = 2; k < 27; k++)
            {
                n = (n<<1);
                times[v][k-2] += func(n,d,r);
            }
            d = (d<<1);
        }
        n = 2;
        for (int k = 2; k < 27; k++)
        {
            n = (n<<1);
            times[12][k-2] += rand_access(n, r);
        }
        ifstream myfile;
        t1 = high_resolution_clock::now();
        myfile.open ("file.txt", ios::in);
        getline(myfile,line);
        t2 = high_resolution_clock::now();
        cout << line << endl;
        duration = duration_cast<microseconds>( t2 - t1 ).count();
        cout << duration << endl;
        myfile.close();
        sum += duration;
    }
    for (int m = 0; m <= 12; m++)
    {
        for (int z = 0; z < 25; z++)
        {
            cout << times[m][z]/5 << ",";
        }
        cout << "\n";
    }
    cout << sum / 5 << endl;
    return 0;
}
