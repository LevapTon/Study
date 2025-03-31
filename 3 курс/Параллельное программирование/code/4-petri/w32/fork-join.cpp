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


#include <windows.h>
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    // длительная операция
    class longjob_type: public threadenv_type::longjob_abstract_type
    {
    private:
        int m_num, m_delay;

        void run(void)
        {
            synprintf(stdout, "job %d begin\n", m_num);
            // ... длительные вычисления
            ::Sleep(m_delay);
            synprintf(stdout, "job %d end\n", m_num);
        }

    public:
        longjob_type(int num):
            m_num(num), m_delay(threadenv_type::random(1000))
        {}
    };

    // ...

    threadenv_type env;

    // позиции и простые переходы
    place_type started, stopped;
    place_type ready1, ready2;
    place_type done1, done2;
    transition_simple_type fork, join;
    // выполняемые длительные работы
    longjob_type job1(1), job2(2);
    // длительные переходы
    threadenv_type::transition_long_type exec1(job1, env);
    threadenv_type::transition_long_type exec2(job2, env);

    // наполнение сети
    petrinet_type::content_type content;
    content.add_place(started);
    content.add_place(stopped);
    content.add_place(ready1);
    content.add_place(ready2);
    content.add_place(done1);
    content.add_place(done2);
    content.add_transition(fork);
    content.add_transition(join);
    content.add_transition(exec1);
    content.add_transition(exec2);

    // внесение дуг
    content.add_arc(started, fork);
    content.add_arc(join, stopped);
    // первая ветвь
    content.add_arc(fork, ready1);
    content.add_arc(ready1, exec1);
    content.add_arc(exec1, done1);
    content.add_arc(done1, join);
    // вторая ветвь
    content.add_arc(fork, ready2);
    content.add_arc(ready2, exec2);
    content.add_arc(exec2, done2);
    content.add_arc(done2, join);

    // фишка в начальной позиции
    content.add_token(started);

    // создание и запуск сети
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
