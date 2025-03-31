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
#include <unistd.h>
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// длительная операция "философ ест"
class longjob_eating_type: public threadenv_type::longjob_abstract_type
{
private:
    int m_num, m_delay;

    void run(void)
    {
        synprintf(stdout, "philosopher %d begins eating\n", m_num);
        // ... длительные вычисления
        ::usleep(m_delay * 1000);
        synprintf(stdout, "philosopher %d begins thinking\n", m_num);
    }

public:
    longjob_eating_type(int num):
        m_num(num), m_delay(threadenv_type::random(1000))
    {}
};


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    threadenv_type env;

    const int N = 5;
    // позиции сети верхнего уровня
    deque<place_type> thinking(N), fork(N);
    // длительные работы
    deque<longjob_eating_type> eatingjob;
    // длительные переходы для выполнения работ
    deque<threadenv_type::transition_long_type> eat;
    // заполняем содержимое сети
    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // добавляем в сеть позиции
        content.add_place(thinking[i]);
        content.add_place(fork[i]);
        // создаем и добавляем в сеть длительный переход
        eatingjob.push_back(longjob_eating_type(i));
        eat.push_back(
            threadenv_type::transition_long_type(eatingjob.back(), env));
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
