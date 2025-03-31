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


#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// ��������� ������� � �������� ���������� ���������
struct proc_in_param
{
    // ������ ��������� ��������
    int begin;
    // ����� ��������� ��������
    int end;
};
struct proc_out_param
{
    // ��������� ����������
    double result;
};


int main(int argc, char *argv[])
{
    int rc = 2;

    // �������, �������� �� ��������� �������� ��� ��������
    char mode = (argc > 1) ? argv[1][0] : '\0';
    // ����� � ���� ������� (� ����� �������)
    unsigned long srvaddr;
    int srvport;
    // ���������� ����������� ��������� (������ ��� �������)
    int size;
    // ������ ���������� �������� (������ ��� �������)
    int n;

    // ------ ������ � �������� ������� ���������� ------
    if (!((mode == 's' && argc == 6) || (mode == 'c' && argc == 4)) ||
        (srvaddr = inet_addr(argv[2])) == INADDR_NONE ||
        (srvport = strtol(argv[3], NULL, 0)) == 0 ||
        (mode == 's' && (size = strtol(argv[4], NULL, 0)) == 0) ||
        (mode == 's' && (n = strtol(argv[5], NULL, 0)) == 0))
    {
        fprintf(stderr,
            "usage:\n"
            "%s s <addr> <port> <procnum> <iternum>\n"
            "or\n"
            "%s c <addr> <port>\n",
            argv[0], argv[0]);
    }
    else
    {
        int ret;
        // �������� ��������� �������� ��������� (������ � �������)
        if (mode != 's' || n % size == 0)
        {
            // ������������ ��������� ������ �������
            struct sockaddr_in san = {0};
            san.sin_family = AF_INET;
            san.sin_port = htons(srvport);
            san.sin_addr.s_addr = srvaddr;

            if (mode == 's')
            {
                // ------ ��� ������� ------
                int srvsock;
                // �������� ���������� ������
                if ((srvsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
                {
                    // �������� � ������ � ������� ������� ������� ����������
                    if (bind(srvsock, (struct sockaddr *) &san, sizeof(san)) == 0 &&
                        listen(srvsock, SOMAXCONN) == 0)
                    {
                        // ������� �������
                        vector<int> clnsock(size);
                        for (int rank = 0; rank < size; ++rank)
                        {
                            // �������� ���������� ����������
                            clnsock[rank] = accept(srvsock, NULL, 0);
                            assert(clnsock[rank] != -1);

                            // ������������ ������� - �������� ��������
                            proc_in_param param = {0};
                            param.begin = rank * (n / size);
                            param.end = (rank + 1) * (n / size);
                            // �������� ������������ �������
                            ret = send(clnsock[rank], &param, sizeof(param), 0);
                            assert(ret == sizeof(param));
                        };

                        // ���� �����������
                        double sum = 0.0;
                        for (int rank = 0; rank < size; ++rank)
                        {
                            // ��������� ���������� �������������� ����������
                            proc_out_param param = {0};
                            int rsize = 0;
                            do
                                ret = recv(
                                    clnsock[rank], &((char *) &param)[rsize],
                                    sizeof(param) - rsize, 0);
                            while (ret > 0 && size_t(rsize += ret) < sizeof(param));
                            assert(rsize == sizeof(param));

                            // ���� ����������� ����������
                            sum += param.result;

                            // ����������� ����������� ������
                            shutdown(clnsock[rank], SHUT_WR);
                            close(clnsock[rank]);
                        };

                        sum *= 4.0;
                        fprintf(stdout, "%.16f\n", sum);

                        // �������� ����������
                        rc = 0;
                    }
                    else
                        fprintf(stderr, "error: can't bind server socket or listen\n");
                    // ����������� ���������� ������
                    close(srvsock);
                }
                else
                    fprintf(stderr, "error: can't create server socket\n");
            }
            else
            {
                // ------ ��� ������� ------
                int sock;
                // �������� ������
                if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
                {
                    // ���������� � ��������
                    if (connect(sock, (struct sockaddr *) &san, sizeof(san)) == 0)
                    {
                        // ��������� ������������ �������
                        proc_in_param inparam = {0};
                        int rsize = 0;
                        do
                            ret = recv(
                                sock, &((char *) &inparam)[rsize],
                                sizeof(inparam) - rsize, 0);
                        while (ret > 0 && size_t(rsize += ret) < sizeof(inparam));
                        assert(rsize == sizeof(inparam));

                        // ���������� ������� - ���������� ����� ����� ����
                        double locsum = 0.0;
                        for (int i = inparam.begin; i < inparam.end; ++i)
                            locsum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);

                        // �������� ����������
                        proc_out_param outparam = {0};
                        outparam.result = locsum;
                        ret = send(sock, &outparam, sizeof(outparam), 0);
                        assert(ret == sizeof(outparam));
                        // �������� ������
                        shutdown(sock, SHUT_WR);

                        // �������� ����������
                        rc = 0;
                    }
                    else
                        fprintf(stderr, "error: can't connect to server\n");
                    // ����������� ������
                    close(sock);
                }
                else
                    fprintf(stderr, "error: can't create client socket\n");
            };
        }
        else
            fprintf(stderr, "error: iternum isn't divisible by procnum\n");
    };
    return rc;
}
