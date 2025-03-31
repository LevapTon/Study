/* ------------------------------------------------------------------------- */
/*  ������ ���� �������� ������ ��������� �������� �������,                  */
/*  �������������� �������� ���������� �������:                              */
/*  ������� �.�.                                                             */
/*  ������ ������������� ����������������. - �.: �����-�����, 2012. - 384 �. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  ��� � ���� �������, ����������� � ���� �������� ������ �������������     */
/*  ���� ��� ������������ � ���������� ���������������� ����������           */
/*  ������������ ��������, � ����� ��� ���������� ���������� �������������.  */
/*  ������������� ����� ���� � �������� ������ ��� �������� ��������         */
/*  ���������, ������ ������� ��������� � ����� �������������� ����          */
/*  �� ������ ����� � ���� ������������.                                     */
/*  �������� ������ ��������������� "��� ����", ��� ����� �� �� �� ����      */
/*  ����� ��� ������� �������� ����������� � ������������� ����������.       */
/*                                                                           */
/*  Copyright � 2008-2011 ������� �.�.                                       */
/* ------------------------------------------------------------------------- */


#include <deque>
#include <unistd.h>
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// ������� "������� �������� �����"
class transition_send_type: public transition_simple_type {};

// ���������� �������� "������� ���"
class longjob_eating_type: public threadenv_type::longjob_abstract_type
{
private:
    int m_num, m_delay;

    void run(void)
    {
        synprintf(stdout, "philosopher %d begins eating\n", m_num);
        // ... ���������� ����������
        ::usleep(m_delay * 1000);
        synprintf(stdout, "philosopher %d begins thinking\n", m_num);
    }

public:
    longjob_eating_type(int num):
        m_num(num), m_delay(threadenv_type::random(1000))
    {}
};


// ��������� � ������������ ������������
class priorityenv_type: public threadenv_type
{
public:
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        petrinet_type::enabledlist_type hi, lo;
        petrinet_type::enabledlist_type::const_iterator it;
        for (it = enabled.begin(); it != enabled.end(); ++it)
        {
            if (dynamic_cast<transition_long_type *>(*it) ||
                dynamic_cast<transition_send_type *>(*it))
                hi.push_back(*it);
            else
                lo.push_back(*it);
        };
        if (hi.empty())
            hi = lo;
        return std::find(
                enabled.begin(),
                enabled.end(),
                hi[threadenv_type::wait(hi, marked)]) - enabled.begin();
    }
};


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    priorityenv_type env;

    const int N = 5;
    // ��������� ���������� ����
    deque<place_type> ready(N), thinking(N), hungry(N);
    deque<place_type> lfork(N), rfork(N), clean(N), dirty(N);
    deque<transition_simple_type> takedd(N), takedc(N), takecd(N), takecc(N);
    deque<transition_send_type> lsend(N), rsend(N);
    deque<transition_simple_type> feel(N);

    // ���������� ������
    deque<longjob_eating_type> eatingjob;
    // ���������� �������� ��� ���������� �����
    deque<threadenv_type::transition_long_type> eat;

    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // ��������� � ���� �������
        content.add_place(ready[i]);
        content.add_place(thinking[i]);
        content.add_place(hungry[i]);
        content.add_place(rfork[i]);
        content.add_place(lfork[i]);
        content.add_place(clean[i]);
        content.add_place(dirty[i]);
        // ������� � ��������� � ���� ��������
        content.add_transition(takedd[i]);
        content.add_transition(takedc[i]);
        content.add_transition(takecd[i]);
        content.add_transition(takecc[i]);
        content.add_transition(lsend[i]);
        content.add_transition(rsend[i]);
        content.add_transition(feel[i]);
        // ������� � ��������� � ���� ���������� �������
        eatingjob.push_back(longjob_eating_type(i));
        eat.push_back(
            threadenv_type::transition_long_type(eatingjob.back(), env));
        content.add_transition(eat.back());
    };

    for (int i = 0; i < N; ++i)
    {
        // ���� ��������� �����
        content.add_arc(lfork[i], rsend[i]);
        content.add_arc(rsend[i], rfork[i]);
        content.add_arc(rfork[i], lsend[i]);
        content.add_arc(lsend[i], lfork[i]);
        content.add_arc(dirty[i], lsend[i]);
        content.add_arc(dirty[i], rsend[i]);
        content.add_arc(lsend[i], clean[i]);
        content.add_arc(rsend[i], clean[i]);
        // �����������
        content.add_arc(hungry[i], lsend[i]);
        content.add_arc(lsend[i], hungry[i]);
        content.add_arc(hungry[(i + 1) % N], rsend[i]);
        content.add_arc(rsend[i], hungry[(i + 1) % N]);

        // ���� ��������� ��������
        content.add_arc(thinking[i], feel[i]);
        content.add_arc(feel[i], hungry[i]);
        content.add_arc(hungry[i], takedd[i]);
        content.add_arc(takedd[i], ready[i]);
        content.add_arc(hungry[i], takedc[i]);
        content.add_arc(takedc[i], ready[i]);
        content.add_arc(hungry[i], takecd[i]);
        content.add_arc(takecd[i], ready[i]);
        content.add_arc(hungry[i], takecc[i]);
        content.add_arc(takecc[i], ready[i]);
        content.add_arc(ready[i], eat[i]);
        content.add_arc(eat[i], thinking[i]);
        // �����������
        content.add_arc(rfork[(i + N - 1) % N], takedd[i]);
        content.add_arc(dirty[(i + N - 1) % N], takedd[i]);
        content.add_arc(lfork[i], takedd[i]);
        content.add_arc(dirty[i], takedd[i]);
        content.add_arc(rfork[(i + N - 1) % N], takedc[i]);
        content.add_arc(dirty[(i + N - 1) % N], takedc[i]);
        content.add_arc(lfork[i], takedc[i]);
        content.add_arc(clean[i], takedc[i]);
        content.add_arc(rfork[(i + N - 1) % N], takecd[i]);
        content.add_arc(clean[(i + N - 1) % N], takecd[i]);
        content.add_arc(lfork[i], takecd[i]);
        content.add_arc(dirty[i], takecd[i]);
        content.add_arc(rfork[(i + N - 1) % N], takecc[i]);
        content.add_arc(clean[(i + N - 1) % N], takecc[i]);
        content.add_arc(lfork[i], takecc[i]);
        content.add_arc(clean[i], takecc[i]);
        content.add_arc(eat[i], rfork[(i + N - 1) % N]);
        content.add_arc(eat[i], dirty[(i + N - 1) % N]);
        content.add_arc(eat[i], lfork[i]);
        content.add_arc(eat[i], dirty[i]);
    };

    for (int i = 0; i < N; ++i)
    {
        // ������ �����
        content.add_token(thinking[i]);
        content.add_token(dirty[i]);
        content.add_token((i < N - 1) ? lfork[i] : rfork[i]);
    };

    // ������� � ��������� ����
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
