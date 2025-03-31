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

int sequence(int i)
{
    return i % 5;
}

struct calcrequest_type
{
    int begin;
    int end;
    address_type customer;
};
struct result_type
{
    int maxval;
    int weight;
};

class customer_type: public actor_type
{
private:
    address_type m_customer;
    bool m_hasresult;
    result_type m_result;

public:
    struct init_type
    {
        address_type customer;
        bool hasresult;
        result_type result;
    };

    customer_type(const init_type &init):
        m_customer(init.customer),
        m_hasresult(init.hasresult),
        m_result(init.result)
    {
        add_action<customer_type, result_type>();
    }
    void action(const result_type &msg)
    {
        if (!m_hasresult)
        {
            init_type init = { m_customer, true, msg };
            become<customer_type>(init);
        }
        else
        {
            result_type res =
                (msg.maxval > m_result.maxval) ? msg : m_result;
            if (msg.maxval == m_result.maxval)
                res.weight = msg.weight + m_result.weight;
            send(m_customer, res);
        };
    }
};

class nummax_type: public actor_type
{
public:
    nummax_type(const empty_type &init)
    {
        add_action<nummax_type, calcrequest_type>();
    }
    void action(const calcrequest_type &msg)
    {
        assert(msg.end - msg.begin > 0);
        if (msg.end - msg.begin == 1)
        {
            result_type res = { sequence(msg.begin), 1 };
            send(msg.customer, res);
        }
        else
        {
            customer_type::init_type init = { msg.customer, false, {0} };
            address_type customer = create<customer_type>(init);

            int half = (msg.end - msg.begin) / 2;
            calcrequest_type req1 = { msg.begin, msg.begin + half, customer };
            send(create<nummax_type>(), req1);
            calcrequest_type req2 = { msg.begin + half, msg.end, customer };
            send(self(), req2);
        };
    }
};

class main_type: public actor_type
{
public:
    struct start_type {};

    main_type(const empty_type &init)
    {
        add_action<main_type, start_type>();
        add_action<main_type, result_type>();
    }
    void action(const start_type &msg)
    {
        calcrequest_type req = { 1, 100, self() };
        send(create<nummax_type>(), req);
    }
    void action(const result_type &msg)
    {
        send(create<print_type>(), msg.weight);
    }
};

int main(int argc, char *argv[])
{
    factory_type factory;
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<nummax_type, actor_type::empty_type>();
    factory.add_definition<main_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    scheduler_type sched;
    sched.system().send(
        sched.system().create<main_type>(),
        main_type::start_type());
    sched.evolve(factory);

    return 0;
}
