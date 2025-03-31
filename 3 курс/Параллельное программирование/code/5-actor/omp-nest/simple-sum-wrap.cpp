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
using namespace std;

struct message_type
{
    int arg1, arg2;
    address_type cust;
};

class summator_type: public actor_type
{
public:
    summator_type(const empty_type &init)
    {
        add_action<summator_type, message_type>();
    }
    void action(const message_type &msg)
    {
        send(msg.cust, msg.arg1 + msg.arg2);
    }
};

class main_type: public scheduler_type
{
public:
    struct start_type {};

    main_type(const empty_type &init)
    {
        add_action<main_type, start_type>();
        add_action<main_type, int>();
    }
    void action(const start_type &start)
    {
        // фабрика текущего планировщика
        factory_type factory;
        factory.add_definition<summator_type, actor_type::empty_type>();
        // инициализация вложенной системы актеров
        message_type msg = { 2, 2, self() };
        system().send(
            system().create<summator_type>(),
            msg);
        // жизненный цикл вложенной системы актеров
        evolve(factory);
    }
    void action(const int &result)
    {
        send(create<print_type>(), result);
    }
};

int main(int argc, char *argv[])
{
    // регистрация актеров в фабрике
    factory_type factory;
    factory.add_definition<main_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    // планировщик
    scheduler_type sched;
    // инициализация системы актеров
    sched.system().send(
        sched.system().create<main_type>(),
        main_type::start_type());
    // жизненный цикл системы актеров
    sched.evolve(factory);

    return 0;
}
