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


#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// длительная операция
class longjob_type: public threadenv_type::longjob_abstract_type
{
private:
    int m_num;

    void run(void)
    {
        // ... длительные вычисления
        for (int i = 0; i < 0xFFFFFFF * m_num; ++i) continue;
        synprintf(stdout, "job %d\n", m_num);
    }

public:
    longjob_type(int num):
        m_num(num)
    {}
};

int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    threadenv_type env;

    // позиции и простые переходы
    place_type q0, q08, q06, q81, q85, q82, q62, q65, q25;
    place_type q27, q23, q51, q57, q37, q14, q17, q47, q7;
    transition_simple_type start;
    // выполняемые длительные работы
    longjob_type j1(1),j2(2),j3(3),j4(4),j5(5),j6(6),j7(7),j8(8);
    // длительные переходы
    threadenv_type::transition_long_type exec1(j1, env);
    threadenv_type::transition_long_type exec2(j2, env);
    threadenv_type::transition_long_type exec3(j3, env);
    threadenv_type::transition_long_type exec4(j4, env);
    threadenv_type::transition_long_type exec5(j5, env);
    threadenv_type::transition_long_type exec6(j6, env);
    threadenv_type::transition_long_type exec7(j7, env);
    threadenv_type::transition_long_type exec8(j8, env);

    // наполнение сети
    petrinet_type::content_type content;
    content.add_place(q0);
    content.add_place(q08);
    content.add_place(q06);
    content.add_place(q81);
    content.add_place(q85);
    content.add_place(q82);
    content.add_place(q62);
    content.add_place(q65);
    content.add_place(q25);
    content.add_place(q27);
    content.add_place(q23);
    content.add_place(q51);
    content.add_place(q57);
    content.add_place(q37);
    content.add_place(q14);
    content.add_place(q17);
    content.add_place(q47);
    content.add_place(q7);
    content.add_transition(start);
    content.add_transition(exec1);
    content.add_transition(exec2);
    content.add_transition(exec3);
    content.add_transition(exec4);
    content.add_transition(exec5);
    content.add_transition(exec6);
    content.add_transition(exec7);
    content.add_transition(exec8);

    // внесение дуг
    content.add_arc(q0, start);
    content.add_arc(start, q08);
    content.add_arc(q08, exec8);
    content.add_arc(start, q06);
    content.add_arc(q06, exec6);
    content.add_arc(exec8, q81);
    content.add_arc(q81, exec1);
    content.add_arc(exec8, q85);
    content.add_arc(q85, exec5);
    content.add_arc(exec8, q82);
    content.add_arc(q82, exec2);
    content.add_arc(exec6, q62);
    content.add_arc(q62, exec2);
    content.add_arc(exec6, q65);
    content.add_arc(q65, exec5);
    content.add_arc(exec2, q25);
    content.add_arc(q25, exec5);
    content.add_arc(exec2, q27);
    content.add_arc(q27, exec7);
    content.add_arc(exec2, q23);
    content.add_arc(q23, exec3);
    content.add_arc(exec5, q51);
    content.add_arc(q51, exec1);
    content.add_arc(exec5, q57);
    content.add_arc(q57, exec7);
    content.add_arc(exec3, q37);
    content.add_arc(q37, exec7);
    content.add_arc(exec1, q14);
    content.add_arc(q14, exec4);
    content.add_arc(exec1, q17);
    content.add_arc(q17, exec7);
    content.add_arc(exec4, q47);
    content.add_arc(q47, exec7);
    content.add_arc(exec7, q7);

    // фишка в начальной позиции
    content.add_token(q0);

    // создание и запуск сети
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
