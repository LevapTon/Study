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


#include <utility>
#include "zactor.hxx"

using namespace z;
using namespace z::actor;

std::pair<int, int> fibpair(int n)
{
    assert(n > 0);
    std::pair<int, int> fib(1, 0);
    if (n > 1)
    {
        std::pair<int, int> pre = fibpair(n - 1);
        fib = std::make_pair(pre.first + pre.second, pre.first);
    };
    return fib;
}

int fibonacci(int n)
{
    assert(n >= 0);
    return n == 0 ? 0 : fibpair(n).first;
}


struct calcrequest_type
{
    int argument;
    address_type customer;
};
struct result_type
{
    int result;
    int previous;
};

class customer_type: public actor_type
{
private:
    address_type m_customer;

public:
    struct init_type
    {
        address_type customer;
    };

    customer_type(const init_type &init):
        m_customer(init.customer)
    {
        add_action<customer_type, result_type>();
    }
    void action(const result_type &msg)
    {
        result_type res = {
            msg.result + msg.previous,
            msg.result
        };
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
        send(m_customer, msg.result);
    }
};

class fibpair_type: public actor_type
{
public:
    fibpair_type(const empty_type &init)
    {
        add_action<fibpair_type, calcrequest_type>();
    }
    void action(const calcrequest_type &msg)
    {
        assert(msg.argument > 0);
        if (msg.argument == 1)
        {
            result_type res = { 1, 0 };
            send(msg.customer, res);
        }
        else
        {
            customer_type::init_type init = { msg.customer };
            address_type customer = create<customer_type>(init);

            calcrequest_type res = { msg.argument - 1, customer };
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

            calcrequest_type req = { msg.argument, takeone };
            send(create<fibpair_type>(), req);
        };
    }
};


int main(int argc, char *argv[])
{
    const int num = 10;
    synprintf(stdout, "%d\n", fibonacci(num));

    factory_type factory;
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<takeone_type, takeone_type::init_type>();
    factory.add_definition<fibpair_type, actor_type::empty_type>();
    factory.add_definition<fibonacci_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    scheduler_type sched;
    calcrequest_type req = {
        num,
        sched.system().create<print_type>()
    };
    sched.system().send(
        sched.system().create<fibonacci_type>(),
        req);
    sched.evolve(factory);

    return 0;
}
