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


#include <windows.h>
#include "zpetri.hxx"
#include "zpetri-env.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


int main(int argc, char *argv[])
{
    //~ srand(unsigned(time(NULL)));

    // ���������� ��������
    class longjob_type: public threadenv_type::longjob_abstract_type
    {
    private:
        int m_num, m_delay;

        void run(void)
        {
            synprintf(stdout, "job %d begin\n", m_num);
            // ... ���������� ����������
            ::Sleep(m_delay);
            synprintf(stdout, "job %d end\n", m_num);
        }

    public:
        longjob_type(int num):
            m_num(num), m_delay(threadenv_type::random(1000))
        {}
    };

    // ...

    threadenv_type env;

    // ������� � ������� ��������
    place_type started, stopped;
    place_type ready1, ready2;
    place_type done1, done2;
    transition_simple_type fork, join;
    // ����������� ���������� ������
    longjob_type job1(1), job2(2);
    // ���������� ��������
    threadenv_type::transition_long_type exec1(job1, env);
    threadenv_type::transition_long_type exec2(job2, env);

    // ���������� ����
    petrinet_type::content_type content;
    content.add_place(started);
    content.add_place(stopped);
    content.add_place(ready1);
    content.add_place(ready2);
    content.add_place(done1);
    content.add_place(done2);
    content.add_transition(fork);
    content.add_transition(join);
    content.add_transition(exec1);
    content.add_transition(exec2);

    // �������� ���
    content.add_arc(started, fork);
    content.add_arc(join, stopped);
    // ������ �����
    content.add_arc(fork, ready1);
    content.add_arc(ready1, exec1);
    content.add_arc(exec1, done1);
    content.add_arc(done1, join);
    // ������ �����
    content.add_arc(fork, ready2);
    content.add_arc(ready2, exec2);
    content.add_arc(exec2, done2);
    content.add_arc(done2, join);

    // ����� � ��������� �������
    content.add_token(started);

    // �������� � ������ ����
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
