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

struct message1_type
{
    // ...
};
struct message2_type
{
    // ...
};
class someactor_type: public actor_type
{
public:
    struct init_type
    {
        // ...
    };
    someactor_type(const init_type &init)
    {
        add_action<someactor_type, message1_type>();
        add_action<someactor_type, message2_type>();
    }
    void action(const message1_type &msg)
    {
        // ...
    }
    void action(const message2_type &msg)
    {
        // ...
    }
};
int main(int argc, char *argv[])
{
    // регистрация актеров в фабрике
    factory_type factory;
    factory.add_definition<someactor_type, someactor_type::init_type>();

    // планировщик
    scheduler_type sched;

    // инициализация системы актеров
    address_type addr;
    someactor_type::init_type init = { /* ... */ };
    addr = sched.system().create<someactor_type>(init);
    message1_type msg1 = { /* ... */ };
    sched.system().send(addr, msg1);
    message2_type msg2 = { /* ... */ };
    sched.system().send(addr, msg2);

    // жизненный цикл системы актеров
    sched.evolve(factory);

    return 0;
}
