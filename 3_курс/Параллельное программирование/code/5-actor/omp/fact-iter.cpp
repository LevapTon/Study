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


int factorial(int n)
{
    int p = 1;
    assert(n >= 0);
    while (n)
        p *= n--;
    return p;
}


struct calcrequest_type
{
    int argument;
    address_type customer;
};

class loop_type: public actor_type
{
private:
    address_type m_customer;

public:
    struct init_type
    {
        address_type customer;
    };
    struct next_type
    {
        int argument;
        int product;
    };

    loop_type(const init_type &init):
        m_customer(init.customer)
    {
        add_action<loop_type, next_type>();
    }
    void action(const next_type &msg)
    {
        assert(msg.argument >= 0);
        if (msg.argument == 0)
            send(m_customer, msg.product);
        else
        {
            next_type next = {
                msg.argument - 1,
                msg.argument * msg.product
            };
            send(self(), next);
        };
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
        loop_type::init_type init = { msg.customer };
        loop_type::next_type next = { msg.argument, 1 };
        send(create<loop_type>(init), next);
    }
};


int main(int argc, char *argv[])
{
    const int num = 12;
    synprintf(stdout, "%d\n", factorial(num));

    factory_type factory;
    factory.add_definition<loop_type, loop_type::init_type>();
    factory.add_definition<factorial_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    scheduler_type sched;
    calcrequest_type req = {
        num,
        sched.system().create<print_type>()
    };
    sched.system().send(
        sched.system().create<factorial_type>(),
        req);
    sched.evolve(factory);

    return 0;
}
