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


#include "zactor.hxx"
#include <algorithm>
#include "../../common/zmatrix.hxx"

using namespace std;
using namespace z;
using namespace z::actor;

matrix_type<int> mtxpow(const matrix_type<int> &mtx, int n)
{
    assert(mtx.vsize() == mtx.hsize());
    assert(n >= 0);
    if (n == 0)
    {
        matrix_type<int> e(mtx.vsize(), mtx.hsize());
        for (int i = 0; i < e.vsize(); ++i)
            for (int j = 0; j < e.hsize(); ++j)
                e(i, j) = (i == j) ? 1 : 0;
        return e;
    }
    else
    {
        matrix_type<int> r = mtxpow(mtx, n >> 1);
        return (n & 1) ? (r * r * mtx) : (r * r);
    };
}

int fibonacci(int n)
{
    assert(n >= 0);
    matrix_type<int> a(2, 2);
    for (int i = 0; i < a.vsize(); ++i)
        for (int j = 0; j < a.hsize(); ++j)
            a(i, j) = (i && j) ? 0 : 1;
    return n == 0 ? 0 : mtxpow(a, n - 1)(0, 0);
}

struct matrixdata_type
{
    int data[4];
};
struct powrequest_type
{
    matrixdata_type matrix;
    int power;
    address_type customer;
};
struct result_type
{
    matrixdata_type matrix;
};
struct calcrequest_type
{
    int argument;
    address_type customer;
};

class customer_type: public actor_type
{
private:
    matrixdata_type m_matrix;
    address_type m_customer;

public:
    struct init_type
    {
        matrixdata_type matrix;
        address_type customer;
    };

    customer_type(const init_type &init):
        m_matrix(init.matrix), m_customer(init.customer)
    {
        add_action<customer_type, result_type>();
    }
    void action(const result_type &msg)
    {
        result_type res;
        matrix_type<int> r(2, 2), m(2, 2);
        copy(&msg.matrix.data[0], &msg.matrix.data[0] + 4, &r(0, 0));
        copy(&m_matrix.data[0], &m_matrix.data[0] + 4, &m(0, 0));
        r = r * r * m;
        copy(&r(0, 0), &r(0, 0) + r.vsize() * r.hsize(), &res.matrix.data[0]);
        send(m_customer, res);
    }
};

class takeone_type: public actor_type
{
private:
    address_type m_customer;

public:
    struct init_type
    {
        address_type customer;
    };

    takeone_type(const init_type &init):
        m_customer(init.customer)
    {
        add_action<takeone_type, result_type>();
    }
    void action(const result_type &msg)
    {
        send(m_customer, msg.matrix.data[0]);
    }
};

class mtxpow_type: public actor_type
{
public:
    mtxpow_type(const empty_type &init)
    {
        add_action<mtxpow_type, powrequest_type>();
    }
    void action(const powrequest_type &msg)
    {
        assert(msg.power >= 0);
        matrixdata_type identity = { { 1, 0, 0, 1 } };
        if (msg.power == 0)
        {
            result_type res = { identity };
            send(msg.customer, res);
        }
        else
        {
            customer_type::init_type init = {
                (msg.power & 1) ? msg.matrix : identity,
                msg.customer
            };
            address_type customer = create<customer_type>(init);

            powrequest_type res = { msg.matrix, msg.power >> 1, customer };
            send(self(), res);
        };
    }
};

class fibonacci_type: public actor_type
{
public:
    fibonacci_type(const empty_type &init)
    {
        add_action<fibonacci_type, calcrequest_type>();
    }
    void action(const calcrequest_type &msg)
    {
        assert(msg.argument >= 0);
        if (msg.argument == 0)
            send(msg.customer, 0);
        else
        {
            takeone_type::init_type init = { msg.customer };
            address_type takeone = create<takeone_type>(init);

            powrequest_type req = {
                { { 1, 1, 1, 0 } }, msg.argument - 1, takeone
            };
            send(create<mtxpow_type>(), req);
        };
    }
};


int main(int argc, char *argv[])
{
    const int num = 10;
    synprintf(stdout, "%d\n", fibonacci(num));

    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    factory_type factory;
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<takeone_type, takeone_type::init_type>();
    factory.add_definition<mtxpow_type, actor_type::empty_type>();
    factory.add_definition<fibonacci_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    scheduler_type sched;
    if (rank == 0)
    {
        calcrequest_type req = {
            num,
            sched.system().create<print_type>()
        };
        sched.system().send(
            sched.system().create<fibonacci_type>(),
            req);
    };
    sched.evolve(factory);

    MPI_Finalize();
    return 0;
}
