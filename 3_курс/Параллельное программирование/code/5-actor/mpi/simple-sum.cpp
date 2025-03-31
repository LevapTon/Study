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

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // ����������� ������� � �������
    factory_type factory;
    factory.add_definition<summator_type, actor_type::empty_type>();
    factory.add_definition<print_type, actor_type::empty_type>();

    // �����������
    scheduler_type sched;
    if (rank == 0)
    {
        // ������������� ������� �������
        message_type msg = {
            2, 2,
            sched.system().create<print_type>()
        };
        sched.system().send(
            sched.system().create<summator_type>(),
            msg);
    };
    // ��������� ���� ������� �������
    sched.evolve(factory);

    MPI_Finalize();
    return 0;
}
