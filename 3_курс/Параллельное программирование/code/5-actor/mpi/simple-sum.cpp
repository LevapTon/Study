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

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // регистрация актеров в фабрике
    factory_type factory;
    factory.add_definition<summator_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    // планировщик
    scheduler_type sched;
    if (rank == 0)
    {
        // инициализация системы актеров
        message_type msg = {
            2, 2,
            sched.system().create<print_type>()
        };
        sched.system().send(
            sched.system().create<summator_type>(),
            msg);
    };
    // жизненный цикл системы актеров
    sched.evolve(factory);

    MPI_Finalize();
    return 0;
}
