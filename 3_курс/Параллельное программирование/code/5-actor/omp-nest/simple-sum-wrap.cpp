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


#include "zactor.hxx"

using namespace z;
using namespace z::actor;
using namespace std;

struct message_type
{
    int arg1, arg2;
    address_type cust;
};

class summator_type: public actor_type
{
public:
    summator_type(const empty_type &init)
    {
        add_action<summator_type, message_type>();
    }
    void action(const message_type &msg)
    {
        send(msg.cust, msg.arg1 + msg.arg2);
    }
};

class main_type: public scheduler_type
{
public:
    struct start_type {};

    main_type(const empty_type &init)
    {
        add_action<main_type, start_type>();
        add_action<main_type, int>();
    }
    void action(const start_type &start)
    {
        // ������� �������� ������������
        factory_type factory;
        factory.add_definition<summator_type, actor_type::empty_type>();
        // ������������� ��������� ������� �������
        message_type msg = { 2, 2, self() };
        system().send(
            system().create<summator_type>(),
            msg);
        // ��������� ���� ��������� ������� �������
        evolve(factory);
    }
    void action(const int &result)
    {
        send(create<print_type>(), result);
    }
};

int main(int argc, char *argv[])
{
    // ����������� ������� � �������
    factory_type factory;
    factory.add_definition<main_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    // �����������
    scheduler_type sched;
    // ������������� ������� �������
    sched.system().send(
        sched.system().create<main_type>(),
        main_type::start_type());
    // ��������� ���� ������� �������
    sched.evolve(factory);

    return 0;
}
