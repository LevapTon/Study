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


// #include "../../common/synprintf.hxx"

void get_input(bool &in1, bool &in2)
{
    // ...
}

void put_output(char out)
{
    // ...
}

int main(int argc, char *argv[])
{
    char state = 'A';
    do
    {
        bool in1, in2;
        char out;
        get_input(in1, in2);
        switch (state)
        {
        case 'A':
            out = in1 ? (in2 ? 'c' : 'd') : (in2 ? 'b' : 'a');
            state = (in1 && !in2) ? 'C' : 'B';
            break;
        case 'B':
            out = in1 ? (in2 ? 'b' : 'd') : (in2 ? 'c' : 'a');
            state = !in2 ? 'C' : (in1 ? 'B' : 'A');
            break;
        case 'C':
            out = in1 ? (in2 ? 'c' : 'a') : (in2 ? 'e' : 'd');
            state = in2 ? 'B' : (in1 ? 'A' : 'C');
            break;
        };
        put_output(out);
    } while (state != 'A');
    return 0;
}
