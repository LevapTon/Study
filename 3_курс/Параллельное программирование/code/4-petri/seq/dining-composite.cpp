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
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// ��������� ������� "������� ���"
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
    // ������� ���� �������� ������
    deque<place_type> thinking(N), fork(N);
    // ���������� ������� � �������� ��������� ���������
    deque<place_type> started(N), stopped(N);
    deque<transition_simple_type> stop(N);
    // ���� ��������� ��������
    deque<transition_eat_type> eat;

    // ��������� ���������� ����
    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // ��������� � ���� �������
        content.add_place(thinking[i]);
        content.add_place(fork[i]);

        // ��������� ��������� �������
        transition_compound_type::content_type cnt;
        cnt.add_place(started[i]);
        cnt.add_place(stopped[i]);
        cnt.add_transition(stop[i]);
        cnt.add_arc(started[i], stop[i]);
        cnt.add_arc(stop[i], stopped[i]);
        cnt.add_token(started[i]);
        eat.push_back(transition_eat_type(cnt, i));
        // � ��������� ��� � ����
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
