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


class transition_1_type: public transition_simple_type
{
    void on_activate(void)
    {
        synprintf(stdout, "transition 1 fires\n");
    }
};

class transition_2_type: public transition_simple_type
{
    void on_activate(void)
    {
        synprintf(stdout, "transition 2 fires\n");
    }
};

class transition_3_type: public transition_simple_type
{
    void on_activate(void)
    {
        synprintf(stdout, "transition 3 fires\n");
    }
};

int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    randomenv_type env;

    // объекты-позиции
    place_type p1, p2, p3, p4, p5, p6;
    // объекты-переходы
    transition_1_type t1;
    transition_2_type t2;
    transition_3_type t3;

    // заполнение содержимого сети Петри
    petrinet_type::content_type content;
    // внесение позиций
    content.add_place(p1);
    content.add_place(p2);
    content.add_place(p3);
    content.add_place(p4);
    content.add_place(p5);
    content.add_place(p6);
    // внесение переходов
    content.add_transition(t1);
    content.add_transition(t2);
    content.add_transition(t3);
    // внесение входных дуг
    content.add_arc(p1, t1);
    content.add_arc(p2, t2);
    content.add_arc(p3, t2);
    content.add_arc(p3, t3);
    content.add_arc(p4, t3);
    // внесение выходных дуг
    content.add_arc(t1, p3);
    content.add_arc(t1, p4);
    content.add_arc(t2, p5);
    content.add_arc(t3, p5);
    content.add_arc(t3, p6);
    // помещение фишек в позиции
    content.add_token(p1, 2);
    content.add_token(p2);
    // создание сети Петри
    petrinet_type petrinet(content);

    // выполнение сети Петри
    petrinet.live(env);

    return 0;
}
