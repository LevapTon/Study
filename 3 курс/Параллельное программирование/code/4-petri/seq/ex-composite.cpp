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


class transition_stop_type: public transition_simple_type
{
    void on_activate(void)
    {
        synprintf(stdout, "transition stop fires\n");
    }
};

int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    randomenv_type env;

    // создание и заполнение составного перехода
    place_type started, stopped;
    transition_stop_type stop;
    transition_compound_type::content_type cnt;
    cnt.add_place(started);
    cnt.add_place(stopped);
    cnt.add_transition(stop);
    cnt.add_arc(started, stop);
    cnt.add_arc(stop, stopped);
    cnt.add_token(started);
    transition_compound_type subnet(cnt);

    // создание и заполнение сети с составным переходом
    petrinet_type::content_type content;
    place_type begin, end;
    content.add_place(begin);
    content.add_place(end);
    content.add_transition(subnet);
    content.add_arc(begin, subnet);
    content.add_arc(subnet, end);
    content.add_token(begin);
    petrinet_type petrinet(content);

    // выполнение сети
    petrinet.live(env);

    return 0;
}
