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

using namespace z;
using namespace z::actor;


int fibonacci(int n)
{
    assert(n >= 0);
    return n <= 1 ? n : (fibonacci(n - 1) + fibonacci(n - 2));
}


struct calcrequest_type
{
    int argument;
    address_type customer;
};

class customer_type: public actor_type
{
private:
    address_type m_customer;
    bool m_hasvalue;
    int m_value;

public:
    struct init_type
    {
        address_type customer;
        bool hasvalue;
        int value;
    };

    customer_type(const init_type &init):
        m_customer(init.customer),
        m_hasvalue(init.hasvalue),
        m_value(init.value)
    {
        add_action<customer_type, int>();
    }
    void action(const int &msg)
    {
        if (!m_hasvalue)
        {
            init_type init = { m_customer, true, msg };
            become<customer_type>(init);
        }
        else
        {
            send(m_customer, msg + m_value);
            become<sink_type>();
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
        if (msg.argument <= 1)
            send(msg.customer, msg.argument);
        else
        {
            customer_type::init_type init = { msg.customer, false, 0 };
            address_type customer = create<customer_type>(init);

            calcrequest_type req1 = { msg.argument - 1, customer };
            send(self(), req1);
            calcrequest_type req2 = { msg.argument - 2, customer };
            send(create<fibonacci_type>(), req2);
        };
    }
};


int main(int argc, char *argv[])
{
    const int num = 10;
    synprintf(stdout, "%d\n", fibonacci(num));

    factory_type factory;
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<fibonacci_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();
    factory.add_definition<sink_type, actor_type::empty_type>();

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
