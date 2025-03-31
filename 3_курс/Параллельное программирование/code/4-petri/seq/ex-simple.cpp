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

    // �������-�������
    place_type p1, p2, p3, p4, p5, p6;
    // �������-��������
    transition_1_type t1;
    transition_2_type t2;
    transition_3_type t3;

    // ���������� ����������� ���� �����
    petrinet_type::content_type content;
    // �������� �������
    content.add_place(p1);
    content.add_place(p2);
    content.add_place(p3);
    content.add_place(p4);
    content.add_place(p5);
    content.add_place(p6);
    // �������� ���������
    content.add_transition(t1);
    content.add_transition(t2);
    content.add_transition(t3);
    // �������� ������� ���
    content.add_arc(p1, t1);
    content.add_arc(p2, t2);
    content.add_arc(p3, t2);
    content.add_arc(p3, t3);
    content.add_arc(p4, t3);
    // �������� �������� ���
    content.add_arc(t1, p3);
    content.add_arc(t1, p4);
    content.add_arc(t2, p5);
    content.add_arc(t3, p5);
    content.add_arc(t3, p6);
    // ��������� ����� � �������
    content.add_token(p1, 2);
    content.add_token(p2);
    // �������� ���� �����
    petrinet_type petrinet(content);

    // ���������� ���� �����
    petrinet.live(env);

    return 0;
}
