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


#include <mpi.h>
#include <sstream>
#include "zfsm.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::fsm;

class ball_type: public fsm_type
{
public:
    enum {
        INPUT_ON,
        INPUT_INIT_X,
        INPUT_INIT_Y,
        INPUT_INIT_DIR,
        INPUT_OTHER_X,
        INPUT_OTHER_Y,
        INPUT_OTHER_DIR,
        INPUT_NUMBER
    };
    enum { OUTPUT_X, OUTPUT_Y, OUTPUT_DIR, OUTPUT_NUMBER };
    // ����������� �������� ����
    enum { DIR_NW, DIR_NE, DIR_SW, DIR_SE, DIR_NUMBER };

private:
    // ������� �����
    const int m_xsize, m_ysize;
    // ������� ���������� ����
    typedef pair<int, int> coords_type;
    coords_type m_coords;

    // ����� ��������� �� ���� ������ � �������� �����������
    coords_type update(state_type dir, const coords_type &coords)
    {
        int dx, dy;
        dx = (dir == DIR_NW || dir == DIR_SW) ? -1 : 1;
        dy = (dir == DIR_NW || dir == DIR_NE) ? -1 : 1;
        return coords_type(coords.first + dx, coords.second + dy);
    }
    // ��������� ����������� �� ���������������
    state_type invert(state_type dir)
    {
        assert(dir >= 0 && dir < DIR_NUMBER);
        state_type inv[] = { DIR_SE, DIR_SW, DIR_NE, DIR_NW };
        return inv[dir];
    }
    // ��������� ����������� �� ������ ����� (���� ���������)
    state_type reflect(state_type dir, const coords_type &coords)
    {
        assert(dir >= 0 && dir < DIR_NUMBER);
        state_type invx[] = { DIR_NE, DIR_NW, DIR_SE, DIR_SW };
        state_type invy[] = { DIR_SW, DIR_SE, DIR_NW, DIR_NE };
        coords_type ncoords = update(dir, coords);
        if (ncoords.first < 0 || ncoords.first >= m_xsize)
            dir = invx[dir];
        if (ncoords.second < 0 || ncoords.second >= m_ysize)
            dir = invy[dir];
        return dir;
    }

    state_type do_start(state_type state)
    {
        // ������������� ��������� � �����������
        m_coords = coords_type(m_input[INPUT_INIT_X], m_input[INPUT_INIT_Y]);
        state = m_input[INPUT_INIT_DIR];
        // ������� ����������
        m_output[OUTPUT_X] = m_coords.first;
        m_output[OUTPUT_Y] = m_coords.second;
        m_output[OUTPUT_DIR] = state;
        return state;
    }

    state_type do_step(state_type state)
    {
        if (m_input[INPUT_ON])
        {
            coords_type other = coords_type(
                m_input[INPUT_OTHER_X], m_input[INPUT_OTHER_Y]);
            // ��������� ����������� ����������� �����
            state_type mydir = reflect(state, m_coords);
            state_type otherdir = reflect(m_input[INPUT_OTHER_DIR], other);
            // ��������� ���������� ��������� (������ ���
            // ����� ���� � ������� ������� ��� � ���������)
            if (m_coords == other || update(mydir, m_coords) == other)
            {
                // ���� ����������� ��������������
                if (invert(mydir) == otherdir)
                {
                    // ����������� ������������ (����� �����������)
                    mydir = invert(mydir);
                    // � ����� ��������� �� ��������� ������
                    mydir = reflect(mydir, m_coords);
                };
            };
            // ������ ������� ����������
            m_coords = update(mydir, m_coords);
            // ������� �� ������ � ����� ������������
            m_output[OUTPUT_X] = m_coords.first;
            m_output[OUTPUT_Y] = m_coords.second;
            state = m_output[OUTPUT_DIR] = mydir;
        }
        else
            // ���������� ������ ������ - �����������
            state = STATE_OFF;
        return state;
    }

public:
    ball_type(int xsize, int ysize):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER, MPI_COMM_SELF),
        m_xsize(xsize), m_ysize(ysize)
    {
        add_handler(STATE_OFF, handler_type(&ball_type::do_start));
        for (int i = 0; i < DIR_NUMBER; ++i)
            add_handler(i, handler_type(&ball_type::do_step));
    }
};

class table_type: public fsm_type
{
public:
    enum {
        INPUT_X1,
        INPUT_Y1,
        INPUT_DIR1,
        INPUT_X2,
        INPUT_Y2,
        INPUT_DIR2,
        INPUT_NUMBER
    };
    enum { OUTPUT_ON, OUTPUT_PERIOD, OUTPUT_NUMBER };
    enum { STATE_ON, STATE_OUT };

private:
    // ������� �����
    const int m_xsize, m_ysize;
    // ���������� ����������� ����� ��������
    signal_type m_steps;
    // ��������� ��������� � ����������� �������� �����
    signals_type m_stored;

    state_type do_start(state_type state)
    {
        m_output[OUTPUT_ON] = 1;
        return STATE_ON;
    }

    state_type do_step(state_type state)
    {
        // ��������� �����-���� ����������� ����� � �����
        // ...
        // � ������� �������� � ��������� ����:
        ostringstream ostr;
        ostr << string(m_xsize, '=') << endl;
        for (int y = 0; y < m_ysize; ++y)
        {
            for (int x = 0; x < m_xsize; ++x)
            {
                if (x == m_input[INPUT_X1] && y == m_input[INPUT_Y1] &&
                    x == m_input[INPUT_X2] && y == m_input[INPUT_Y2])
                    ostr << 'x';
                else if (x == m_input[INPUT_X1] && y == m_input[INPUT_Y1])
                    ostr << '1';
                else if (x == m_input[INPUT_X2] && y == m_input[INPUT_Y2])
                    ostr << '2';
                else
                    ostr << '.';
            };
            ostr << endl;
        };
        synprintf(stdout, "%s\n", ostr.str().c_str());

        // ���������� ��������� ����� (�� ������ ����)
        if (m_steps == 0)
            m_stored = m_input;

        // �� ��������� ��������� ����������
        if (m_steps > 0 && m_stored == m_input)
        {
            // ���� �������, ��������� ���� ������
            state = STATE_OUT;
            m_output[OUTPUT_ON] = 0;
            m_output[OUTPUT_PERIOD] = m_steps;
        }
        else
        {
            // ����� ��������� � ���������� ����
            state = STATE_ON;
            m_output[OUTPUT_ON] = 1;
            ++m_steps;
        };
        return state;
    }

    state_type do_stop(state_type state)
    {
        m_steps = 0;
        m_stored.clear();
        return STATE_OFF;
    }

public:
    table_type(int xsize, int ysize):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER, MPI_COMM_SELF),
        m_xsize(xsize), m_ysize(ysize), m_steps(0)
    {
        add_handler(STATE_OFF, handler_type(&table_type::do_start));
        add_handler(STATE_ON, handler_type(&table_type::do_step));
        add_handler(STATE_OUT, handler_type(&table_type::do_stop));
    }
};

class billiard_type: public fsmnet_type
{
public:
    enum { FSM_TABLE, FSM_BALL1, FSM_BALL2, FSM_NUMBER };
    enum {
        INPUT_INIT_X1,
        INPUT_INIT_Y1,
        INPUT_INIT_DIR1,
        INPUT_INIT_X2,
        INPUT_INIT_Y2,
        INPUT_INIT_DIR2,
        INPUT_NUMBER
    };
    enum { OUTPUT_PERIOD, OUTPUT_NUMBER };

    class factory_type: public factory_abstract_type
    {
        const int m_xsize, m_ysize;
    public:
        factory_type(int xsize, int ysize):
            m_xsize(xsize), m_ysize(ysize)
        {
            assert(m_xsize > 1);
            assert(m_ysize > 1);
        }
        int number_fsm(void) const { return FSM_NUMBER; }
        int size_fsm(int id) const { return 1; }
        int number_input(void) const { return INPUT_NUMBER; }
        int number_output(void) const { return OUTPUT_NUMBER; }
        links_type links(void) const
        {
            links_type links;
            // �������� ���������� � ����������� � ����������� �����
            for (int i = 0; i < ball_type::OUTPUT_NUMBER; ++i)
            {
                // ��������� ���������� � ����������� �����
                links.input_to_fsm(
                    INPUT_INIT_X1 + i,
                    FSM_BALL1, ball_type::INPUT_INIT_X + i);
                links.input_to_fsm(
                    INPUT_INIT_X2 + i,
                    FSM_BALL2, ball_type::INPUT_INIT_X + i);
                // ���������� �� ����� �� ����� �����
                links.fsm_to_fsm(
                    FSM_BALL1, i,
                    FSM_TABLE, table_type::INPUT_X1 + i);
                links.fsm_to_fsm(
                    FSM_BALL2, i,
                    FSM_TABLE, table_type::INPUT_X2 + i);
                // ���������� �� ����� ���� �����
                links.fsm_to_fsm(
                    FSM_BALL1, i,
                    FSM_BALL2, ball_type::INPUT_OTHER_X + i);
                links.fsm_to_fsm(
                    FSM_BALL2, i,
                    FSM_BALL1, ball_type::INPUT_OTHER_X + i);
            };
            // ������ ��������� �� ����� � �����
            links.fsm_to_fsm(
                FSM_TABLE, table_type::OUTPUT_ON,
                FSM_BALL1, ball_type::INPUT_ON);
            links.fsm_to_fsm(
                FSM_TABLE, table_type::OUTPUT_ON,
                FSM_BALL2, ball_type::INPUT_ON);
            // ����� ���� - ������� �������� �������
            links.fsm_to_output(
                FSM_TABLE, table_type::OUTPUT_PERIOD,
                OUTPUT_PERIOD);
            return links;
        }
        fsm_abstract_type *create_fsm(int id, MPI_Comm commfull)
        {
            fsm_abstract_type *pfsm;
            if (id == FSM_TABLE)
                pfsm = new table_type(m_xsize, m_ysize);
            else
                pfsm = new ball_type(m_xsize, m_ysize);
            return pfsm;
        }
        void destroy_fsm(int id, fsm_abstract_type *pfsm)
        {
            if (id == FSM_TABLE)
                delete dynamic_cast<table_type *>(pfsm);
            else
                delete dynamic_cast<ball_type *>(pfsm);
        }
    };

public:
    billiard_type(factory_type &factory):
        fsmnet_type(factory, MPI_COMM_WORLD)
    {}
};


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == billiard_type::FSM_NUMBER)
    {
        billiard_type::factory_type factory(8, 6);
        billiard_type fsmnet(factory);

        fsm_type::signals_type ins(billiard_type::INPUT_NUMBER);
        // �� ������ ���� ����� � ������ �� ������ ���� �����������
        // (������ ��� �� ������ ��������� � ������� ��� ������� ����)
        ins[billiard_type::INPUT_INIT_X1] = 1;
        ins[billiard_type::INPUT_INIT_Y1] = 1;
        ins[billiard_type::INPUT_INIT_DIR1] = ball_type::DIR_NE;
        ins[billiard_type::INPUT_INIT_X2] = 6;
        ins[billiard_type::INPUT_INIT_Y2] = 4;
        ins[billiard_type::INPUT_INIT_DIR2] = ball_type::DIR_NW;

        fsm_type::signal_type period = 0;
        fsmnet.put_input(ins);
        do
        {
            fsmnet.do_work();
            period = max(
                period,
                fsmnet.get_output()[billiard_type::OUTPUT_PERIOD]);
        } while (!fsmnet.is_off());

        synprintf(stdout, "period: %d\n", period);
    }
    else
        synprintf(stderr, "error: must be executed in group of %d processes\n", billiard_type::FSM_NUMBER);

    MPI_Finalize();
    return 0;
}
