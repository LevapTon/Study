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


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    threadenv_type env;

    const int N = 5;
    // ������� ���� �������� ������
    deque<place_type> thinking(N), fork(N);
    // ���������� ������
    deque<longjob_eating_type> eatingjob;
    // ���������� �������� ��� ���������� �����
    deque<threadenv_type::transition_long_type> eat;
    // ��������� ���������� ����
    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // ��������� � ���� �������
        content.add_place(thinking[i]);
        content.add_place(fork[i]);
        // ������� � ��������� � ���� ���������� �������
        eatingjob.push_back(longjob_eating_type(i));
        eat.push_back(
            threadenv_type::transition_long_type(eatingjob.back(), env));
        content.add_transition(eat.back());
    };
    for (int i = 0; i < N; ++i)
    {
        // ������� ���� �������� eat
        content.add_arc(thinking[i], eat[i]);
        content.add_arc(fork[(i + N - 1) % N], eat[i]);
        content.add_arc(fork[i], eat[i]);
        // �������� ���� �������� eat
        content.add_arc(eat[i], thinking[i]);
        content.add_arc(eat[i], fork[(i + N - 1) % N]);
        content.add_arc(eat[i], fork[i]);
    };
    for (int i = 0; i < N; ++i)
    {
        // ������ �����
        content.add_token(thinking[i]);
        content.add_token(fork[i]);
    };
    // ������� � ��������� ����
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
