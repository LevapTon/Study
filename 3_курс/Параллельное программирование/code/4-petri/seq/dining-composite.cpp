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


#include <deque>
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// составной переход "философ ест"
class transition_eat_type: public transition_compound_type
{
private:
    int m_num;

    void on_activate(void)
    {
        synprintf(stdout, "philosopher %d begins eating\n", m_num);
    }
    void on_passivate(void)
    {
        synprintf(stdout, "philosopher %d begins thinking\n", m_num);
    }

public:
    transition_eat_type(const content_type &content, int num):
        transition_compound_type(content),
        m_num(num)
    {}
};

int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    randomenv_type env;

    const int N = 5;
    // позиции сети верхнего уровня
    deque<place_type> thinking(N), fork(N);
    // внутренние позиции и переходы составных переходов
    deque<place_type> started(N), stopped(N);
    deque<transition_simple_type> stop(N);
    // сами составные переходы
    deque<transition_eat_type> eat;

    // заполняем содержимое сети
    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // добавляем в сеть позиции
        content.add_place(thinking[i]);
        content.add_place(fork[i]);

        // заполняем составной переход
        transition_compound_type::content_type cnt;
        cnt.add_place(started[i]);
        cnt.add_place(stopped[i]);
        cnt.add_transition(stop[i]);
        cnt.add_arc(started[i], stop[i]);
        cnt.add_arc(stop[i], stopped[i]);
        cnt.add_token(started[i]);
        eat.push_back(transition_eat_type(cnt, i));
        // и добавляем его в сеть
        content.add_transition(eat.back());
    };
    for (int i = 0; i < N; ++i)
    {
        // входные дуги перехода eat
        content.add_arc(thinking[i], eat[i]);
        content.add_arc(fork[(i + N - 1) % N], eat[i]);
        content.add_arc(fork[i], eat[i]);
        // выходные дуги перехода eat
        content.add_arc(eat[i], thinking[i]);
        content.add_arc(eat[i], fork[(i + N - 1) % N]);
        content.add_arc(eat[i], fork[i]);
    };
    for (int i = 0; i < N; ++i)
    {
        // кладем фишки
        content.add_token(thinking[i]);
        content.add_token(fork[i]);
    };
    // создаем и запускаем сеть
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
