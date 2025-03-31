/* ------------------------------------------------------------------------- */
/*  Данный файл является частью комплекта исходных текстов,                  */
/*  иллюстрирующих материал следующего издания:                              */
/*  Федотов И.Е.                                                             */
/*  Модели параллельного программирования. - М.: СОЛОН-ПРЕСС, 2012. - 384 с. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  Как и само издание, прилагаемые к нему исходные тексты предназначены     */
/*  лишь для ознакомления с принципами высокоуровневого построения           */
/*  параллельных программ, а также для проведения дальнейших экспериментов.  */
/*  Использование этого кода в качестве основы для реальных программ         */
/*  допустимо, однако требует доработок и может осуществляться лишь          */
/*  на личный страх и риск разработчика.                                     */
/*  Исходные тексты предоставляются "как есть", без каких бы то ни было      */
/*  явных или неявных гарантий пригодности к практическому применению.       */
/*                                                                           */
/*  Copyright © 2008-2011 Федотов И.Е.                                       */
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

// структуры входных и выходных параметров процессов
struct proc_in_param
{
    // начало интервала итераций
    int begin;
    // конец интервала итераций
    int end;
};
struct proc_out_param
{
    // результат вычислений
    double result;
};


int main(int argc, char *argv[])
{
    int rc = 2;

    // признак, является ли программа сервером или клиентом
    char mode = (argc > 1) ? argv[1][0] : '\0';
    // адрес и порт сервера (в обоих режимах)
    unsigned long srvaddr;
    int srvport;
    // количество вычисляющих процессов (только для сервера)
    int size;
    // полное количество итераций (только для сервера)
    int n;

    // ------ чтение и проверка входных параметров ------
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
        // проверка кратности итераций процессам (только в сервере)
        if (mode != 's' || n % size == 0)
        {
            // формирование структуры адреса сервера
            struct sockaddr_in san = {0};
            san.sin_family = AF_INET;
            san.sin_port = htons(srvport);
            san.sin_addr.s_addr = srvaddr;

            if (mode == 's')
            {
                // ------ код сервера ------
                int srvsock;
                // создание серверного сокета
                if ((srvsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
                {
                    // привязка к адресу и задание размера очереди соединений
                    if (bind(srvsock, (struct sockaddr *) &san, sizeof(san)) == 0 &&
                        listen(srvsock, SOMAXCONN) == 0)
                    {
                        // раздача заданий
                        vector<int> clnsock(size);
                        for (int rank = 0; rank < size; ++rank)
                        {
                            // принятие очередного соединения
                            clnsock[rank] = accept(srvsock, NULL, 0);
                            assert(clnsock[rank] != -1);

                            // формирование задания - интервал итераций
                            proc_in_param param = {0};
                            param.begin = rank * (n / size);
                            param.end = (rank + 1) * (n / size);
                            // отправка назначенного задания
                            ret = send(clnsock[rank], &param, sizeof(param), 0);
                            assert(ret == sizeof(param));
                        };

                        // сбор результатов
                        double sum = 0.0;
                        for (int rank = 0; rank < size; ++rank)
                        {
                            // получение очередного промежуточного результата
                            proc_out_param param = {0};
                            int rsize = 0;
                            do
                                ret = recv(
                                    clnsock[rank], &((char *) &param)[rsize],
                                    sizeof(param) - rsize, 0);
                            while (ret > 0 && size_t(rsize += ret) < sizeof(param));
                            assert(rsize == sizeof(param));

                            // учет полученного результата
                            sum += param.result;

                            // уничтожение клиентского сокета
                            shutdown(clnsock[rank], SHUT_WR);
                            close(clnsock[rank]);
                        };

                        sum *= 4.0;
                        fprintf(stdout, "%.16f\n", sum);

                        // успешное завершение
                        rc = 0;
                    }
                    else
                        fprintf(stderr, "error: can't bind server socket or listen\n");
                    // уничтожение серверного сокета
                    close(srvsock);
                }
                else
                    fprintf(stderr, "error: can't create server socket\n");
            }
            else
            {
                // ------ код клиента ------
                int sock;
                // создание сокета
                if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
                {
                    // соединение с сервером
                    if (connect(sock, (struct sockaddr *) &san, sizeof(san)) == 0)
                    {
                        // получение назначенного задания
                        proc_in_param inparam = {0};
                        int rsize = 0;
                        do
                            ret = recv(
                                sock, &((char *) &inparam)[rsize],
                                sizeof(inparam) - rsize, 0);
                        while (ret > 0 && size_t(rsize += ret) < sizeof(inparam));
                        assert(rsize == sizeof(inparam));

                        // выполнение задания - вычисление своей части ряда
                        double locsum = 0.0;
                        for (int i = inparam.begin; i < inparam.end; ++i)
                            locsum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);

                        // отправка результата
                        proc_out_param outparam = {0};
                        outparam.result = locsum;
                        ret = send(sock, &outparam, sizeof(outparam), 0);
                        assert(ret == sizeof(outparam));
                        // закрытие канала
                        shutdown(sock, SHUT_WR);

                        // успешное завершение
                        rc = 0;
                    }
                    else
                        fprintf(stderr, "error: can't connect to server\n");
                    // уничтожение сокета
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
