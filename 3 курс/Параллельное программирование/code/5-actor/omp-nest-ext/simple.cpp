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

struct message1_type
{
    // ...
};
struct message2_type
{
    // ...
};
class someactor_type: public actor_type
{
public:
    struct init_type
    {
        // ...
    };
    someactor_type(const init_type &init)
    {
        add_action<someactor_type, message1_type>();
        add_action<someactor_type, message2_type>();
    }
    void action(const message1_type &msg)
    {
        // ...
    }
    void action(const message2_type &msg)
    {
        // ...
    }
};
int main(int argc, char *argv[])
{
    // ����������� ������� � �������
    factory_type factory;
    factory.add_definition<someactor_type, someactor_type::init_type>();

    // �����������
    scheduler_type sched;

    // ������������� ������� �������
    address_type addr;
    someactor_type::init_type init = { /* ... */ };
    addr = sched.system().create<someactor_type>(init);
    message1_type msg1 = { /* ... */ };
    sched.system().send(addr, msg1);
    message2_type msg2 = { /* ... */ };
    sched.system().send(addr, msg2);

    // ��������� ���� ������� �������
    sched.evolve(factory);

    return 0;
}
