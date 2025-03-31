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


// переход "философ передает вилку"
class transition_send_type: public transition_simple_type {};

// переход "приступает к еде"
class transition_eating_start_type: public transition_simple_type
{
private:
    int m_num;
    void on_activate(void)
    {
        synprintf(stdout, "philosopher %d begins eating\n", m_num);
    }
public:
    transition_eating_start_type(int num): m_num(num) {}
};

// переход "приступает к размышлениям"
class transition_eating_stop_type: public transition_simple_type
{
private:
    int m_num;
    void on_activate(void)
    {
        synprintf(stdout, "philosopher %d begins thinking\n", m_num);
    }
public:
    transition_eating_stop_type(int num): m_num(num) {}
};


// окружение с приоритетом срабатывания transition_send_type
class priorityenv_type: public randomenv_type
{
public:
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        std::vector<int> p, q;
        petrinet_type::enabledlist_type::const_iterator it;
        for (it = enabled.begin(); it != enabled.end(); ++it)
        {
            if (dynamic_cast<transition_send_type *>(*it))
                p.push_back(std::distance(enabled.begin(), it));
            else
                q.push_back(std::distance(enabled.begin(), it));
        };
        return !p.empty() ? p[random(p.size())] : q[random(q.size())];
    }
};


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    priorityenv_type env;

    const int N = 5;
    // заполняем содержимое сети
    deque<place_type> eating(N), thinking(N), hungry(N);
    deque<place_type> lfork(N), rfork(N), clean(N), dirty(N);
    deque<transition_eating_start_type> startdd, startdc, startcd, startcc;
    deque<transition_eating_stop_type> stop;
    deque<transition_send_type> lsend(N), rsend(N);
    deque<transition_simple_type> feel(N);

    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // добавляем в сеть позиции
        content.add_place(eating[i]);
        content.add_place(thinking[i]);
        content.add_place(hungry[i]);
        content.add_place(rfork[i]);
        content.add_place(lfork[i]);
        content.add_place(clean[i]);
        content.add_place(dirty[i]);
        // создаем и добавляем в сеть переходы
        startdd.push_back(transition_eating_start_type(i));
        content.add_transition(startdd.back());
        startdc.push_back(transition_eating_start_type(i));
        content.add_transition(startdc.back());
        startcd.push_back(transition_eating_start_type(i));
        content.add_transition(startcd.back());
        startcc.push_back(transition_eating_start_type(i));
        content.add_transition(startcc.back());
        stop.push_back(transition_eating_stop_type(i));
        content.add_transition(stop.back());
        content.add_transition(lsend[i]);
        content.add_transition(rsend[i]);
        content.add_transition(feel[i]);
    };

    for (int i = 0; i < N; ++i)
    {
        // дуги фрагмента вилки
        content.add_arc(lfork[i], rsend[i]);
        content.add_arc(rsend[i], rfork[i]);
        content.add_arc(rfork[i], lsend[i]);
        content.add_arc(lsend[i], lfork[i]);
        content.add_arc(dirty[i], lsend[i]);
        content.add_arc(dirty[i], rsend[i]);
        content.add_arc(lsend[i], clean[i]);
        content.add_arc(rsend[i], clean[i]);
        // зависимости
        content.add_arc(hungry[i], lsend[i]);
        content.add_arc(lsend[i], hungry[i]);
        content.add_arc(hungry[(i + 1) % N], rsend[i]);
        content.add_arc(rsend[i], hungry[(i + 1) % N]);

        // дуги фрагмента философа
        content.add_arc(thinking[i], feel[i]);
        content.add_arc(feel[i], hungry[i]);
        content.add_arc(hungry[i], startdd[i]);
        content.add_arc(startdd[i], eating[i]);
        content.add_arc(hungry[i], startdc[i]);
        content.add_arc(startdc[i], eating[i]);
        content.add_arc(hungry[i], startcd[i]);
        content.add_arc(startcd[i], eating[i]);
        content.add_arc(hungry[i], startcc[i]);
        content.add_arc(startcc[i], eating[i]);
        content.add_arc(eating[i], stop[i]);
        content.add_arc(stop[i], thinking[i]);
        // зависимости
        content.add_arc(rfork[(i + N - 1) % N], startdd[i]);
        content.add_arc(dirty[(i + N - 1) % N], startdd[i]);
        content.add_arc(lfork[i], startdd[i]);
        content.add_arc(dirty[i], startdd[i]);
        content.add_arc(rfork[(i + N - 1) % N], startdc[i]);
        content.add_arc(dirty[(i + N - 1) % N], startdc[i]);
        content.add_arc(lfork[i], startdc[i]);
        content.add_arc(clean[i], startdc[i]);
        content.add_arc(rfork[(i + N - 1) % N], startcd[i]);
        content.add_arc(clean[(i + N - 1) % N], startcd[i]);
        content.add_arc(lfork[i], startcd[i]);
        content.add_arc(dirty[i], startcd[i]);
        content.add_arc(rfork[(i + N - 1) % N], startcc[i]);
        content.add_arc(clean[(i + N - 1) % N], startcc[i]);
        content.add_arc(lfork[i], startcc[i]);
        content.add_arc(clean[i], startcc[i]);
        content.add_arc(stop[i], rfork[(i + N - 1) % N]);
        content.add_arc(stop[i], dirty[(i + N - 1) % N]);
        content.add_arc(stop[i], lfork[i]);
        content.add_arc(stop[i], dirty[i]);
    };

    for (int i = 0; i < N; ++i)
    {
        // кладем фишки
        content.add_token(thinking[i]);
        content.add_token(dirty[i]);
        content.add_token((i < N - 1) ? lfork[i] : rfork[i]);
    };

    // создаем и запускаем сеть
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
