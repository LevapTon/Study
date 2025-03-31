/* ------------------------------------------------------------------------- */
/*  ������ ���� �������� ������ ��������� �������� �������,                  */
/*  �������������� �������� ���������� �������:                              */
/*  ������� �.�.                                                             */
/*  ������ ������������� ����������������. - �.: �����-�����, 2012. - 384 �. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  ��� � ���� �������, ����������� � ���� �������� ������ �������������     */
/*  ���� ��� ������������ � ���������� ���������������� ����������           */
/*  ������������ ��������, � ����� ��� ���������� ���������� �������������.  */
/*  ������������� ����� ���� � �������� ������ ��� �������� ��������         */
/*  ���������, ������ ������� ��������� � ����� �������������� ����          */
/*  �� ������ ����� � ���� ������������.                                     */
/*  �������� ������ ��������������� "��� ����", ��� ����� �� �� �� ����      */
/*  ����� ��� ������� �������� ����������� � ������������� ����������.       */
/*                                                                           */
/*  Copyright � 2008-2011 ������� �.�.                                       */
/* ------------------------------------------------------------------------- */


#include <windows.h>
#include <cstdio>

using namespace std;

enum { NUM_ITER = 1000000, NUM_THREADS = 4 };

// ��������� ���������� ������
struct thr_param
{
    int begin;
    int end;
    double result;
};

// ������� ������
DWORD WINAPI thr_proc(LPVOID param)
{
    thr_param &p = *static_cast<thr_param *>(param);
    for (int i = p.begin; i < p.end; ++i)
        p.result += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);
    return 0;
}

int main(int argc, char *argv[])
{
    // ���������� ���������� ��� ������� ������
    thr_param param[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        param[i].begin = i * (NUM_ITER / NUM_THREADS);
        param[i].end = (i + 1) * (NUM_ITER / NUM_THREADS);
        param[i].result = 0.0;
    };
    // �������� �������
    HANDLE hdl[NUM_THREADS - 1];
    DWORD dwId[NUM_THREADS - 1];
    for (int i = 0; i < NUM_THREADS - 1; ++i)
        hdl[i] = ::CreateThread(
            NULL, 0,
            thr_proc, &param[i + 1],
            0, &dwId[i]);
    // ���������� � ������� ������
    thr_proc(&param[0]);
    // �������� ���������� ������ �������
    ::WaitForMultipleObjects(NUM_THREADS - 1, hdl, TRUE, INFINITE);
    // ������������ ��������
    for (int i = 0; i < NUM_THREADS - 1; ++i)
        ::CloseHandle(hdl[i]);
    // ������������ ����������� �������
    double sum = 0.0;
    for (int i = 0; i < NUM_THREADS; ++i)
        sum += param[i].result;
    fprintf(stdout, "%.16f\n", sum * 4.0);
    return 0;
}
