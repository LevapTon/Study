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

    // �������� � ���������� ���������� ��������
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

    // �������� � ���������� ���� � ��������� ���������
    petrinet_type::content_type content;
    place_type begin, end;
    content.add_place(begin);
    content.add_place(end);
    content.add_transition(subnet);
    content.add_arc(begin, subnet);
    content.add_arc(subnet, end);
    content.add_token(begin);
    petrinet_type petrinet(content);

    // ���������� ����
    petrinet.live(env);

    return 0;
}
