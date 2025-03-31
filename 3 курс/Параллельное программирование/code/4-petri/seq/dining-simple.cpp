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


// ������� "���������� � ���"
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

// ������� "���������� � ������������"
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

int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    randomenv_type env;

    const int N = 5;
    // ��������� ���������� ����
    deque<place_type> eating(N), thinking(N), fork(N);
    deque<transition_eating_start_type> start;
    deque<transition_eating_stop_type> stop;
    petrinet_type::content_type content;
    for (int i = 0; i < N; ++i)
    {
        // ��������� � ���� �������
        content.add_place(eating[i]);
        content.add_place(thinking[i]);
        content.add_place(fork[i]);
        // ������� � ��������� � ���� ��������
        start.push_back(transition_eating_start_type(i));
        content.add_transition(start.back());
        stop.push_back(transition_eating_stop_type(i));
        content.add_transition(stop.back());
    };
    for (int i = 0; i < N; ++i)
    {
        // ������� � �������� ���� �������� start
        content.add_arc(thinking[i], start[i]);
        content.add_arc(fork[(i + N - 1) % N], start[i]);
        content.add_arc(fork[i], start[i]);
        content.add_arc(start[i], eating[i]);
        // ������� � �������� ���� �������� stop
        content.add_arc(eating[i], stop[i]);
        content.add_arc(stop[i], fork[(i + N - 1) % N]);
        content.add_arc(stop[i], thinking[i]);
        content.add_arc(stop[i], fork[i]);
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
