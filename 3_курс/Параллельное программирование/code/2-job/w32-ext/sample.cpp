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


#include "zjobs.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::jobs;

class job_type: public job_abstract_type
{
private:
    int m_num;
public:
    job_type(int num): m_num(num) {}
    void run(void)
    {
        for (int i = 0; i < 0xFFFFFFF * m_num; ++i) continue;
        synprintf(stdout, "job %d\n", m_num);
    }
};

int main(int argc, char *argv[])
{
    // �������� �������� �����
    job_type j1(1),j2(2),j3(3),j4(4),j5(5),j6(6),j7(7),j8(8);

    // �������� � ���������� ��������� �����
    jobcomplex_type jobcomplex;
    // ���������� ������ �����
    jobcomplex.add_job(j1);
    jobcomplex.add_job(j2);
    jobcomplex.add_job(j3);
    jobcomplex.add_job(j4);
    jobcomplex.add_job(j5);
    jobcomplex.add_job(j6);
    jobcomplex.add_job(j7);
    jobcomplex.add_job(j8);
    // ���������� ������� ������������
    jobcomplex.add_dependence(j1, j5);
    jobcomplex.add_dependence(j1, j8);
    jobcomplex.add_dependence(j2, j6);
    jobcomplex.add_dependence(j2, j8);
    jobcomplex.add_dependence(j3, j2);
    jobcomplex.add_dependence(j4, j1);
    jobcomplex.add_dependence(j5, j2);
    jobcomplex.add_dependence(j5, j6);
    jobcomplex.add_dependence(j5, j8);
    jobcomplex.add_dependence(j7, j1);
    jobcomplex.add_dependence(j7, j2);
    jobcomplex.add_dependence(j7, j3);
    jobcomplex.add_dependence(j7, j4);
    jobcomplex.add_dependence(j7, j5);

    // ���������� ��������� �����
    jobcomplex.run();

    return 0;
}
