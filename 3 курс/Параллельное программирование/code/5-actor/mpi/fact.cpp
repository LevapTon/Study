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


#include "zactor.hxx"

using namespace z;
using namespace z::actor;


int factorial(int n)
{
    assert(n >= 0);
    return n <= 1 ? 1 : (n * factorial(n - 1));
}


struct calcrequest_type
{
    int argument;
    address_type customer;
};

class customer_type: public actor_type
{
private:
    int m_multiplier;
    address_type m_customer;

public:
    struct init_type
    {
        int multiplier;
        address_type customer;
    };

    customer_type(const init_type &init):
        m_multiplier(init.multiplier),
        m_customer(init.customer)
    {
        add_action<customer_type, int>();
    }
    void action(const int &msg)
    {
        send(m_customer, m_multiplier * msg);
    }
};

class factorial_type: public actor_type
{
public:
    factorial_type(const empty_type &init)
    {
        add_action<factorial_type, calcrequest_type>();
    }
    void action(const calcrequest_type &msg)
    {
        assert(msg.argument >= 0);
        if (msg.argument == 0)
            send(msg.customer, 1);
        else
        {
            customer_type::init_type init = {
                msg.argument, msg.customer
            };
            calcrequest_type req = {
                msg.argument - 1,
                create<customer_type>(init)
            };
            send(self(), req);
        };
    }
};


int main(int argc, char *argv[])
{
    const int num = 12;
    synprintf(stdout, "%d\n", factorial(num));

    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    factory_type factory;
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<factorial_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    scheduler_type sched;
    if (rank == 0)
    {
        calcrequest_type req = {
            num,
            sched.system().create<print_type>()
        };
        sched.system().send(
            sched.system().create<factorial_type>(),
            req);
    };
    sched.evolve(factory);

    MPI_Finalize();
    return 0;
}
