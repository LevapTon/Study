/* ------------------------------------------------------------------------- */
/*  Данный файл является частью комплекта исходных текстов,                  */
/*  иллюстрирующих материал следующего издания:                              */
/*  Федотов И.Е.                                                             */
/*  Модели параллельного программирования. - М.: СОЛОН-ПРЕСС, 2012. - 384 с. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  Как и само издание, прилагаемые к нему исходные тексты предназначены     */
/*  лишь для ознакомления с принципами высокоуровневого построения           */
/*  параллельных программ, а также для проведения дальнейших экспериментов.  */
/*  Использование этого кода в качестве основы для реальных программ         */
/*  допустимо, однако требует доработок и может осуществляться лишь          */
/*  на личный страх и риск разработчика.                                     */
/*  Исходные тексты предоставляются "как есть", без каких бы то ни было      */
/*  явных или неявных гарантий пригодности к практическому применению.       */
/*                                                                           */
/*  Copyright © 2008-2011 Федотов И.Е.                                       */
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
    // направления движения шара
    enum { DIR_NW, DIR_NE, DIR_SW, DIR_SE, DIR_NUMBER };

private:
    // размеры стола
    const int m_xsize, m_ysize;
    // текущие координаты шара
    typedef pair<int, int> coords_type;
    coords_type m_coords;

    // сдвиг координат на одну клетку в заданном направлении
    coords_type update(state_type dir, const coords_type &coords)
    {
        int dx, dy;
        dx = (dir == DIR_NW || dir == DIR_SW) ? -1 : 1;
        dy = (dir == DIR_NW || dir == DIR_NE) ? -1 : 1;
        return coords_type(coords.first + dx, coords.second + dy);
    }
    // обращение направления на противоположное
    state_type invert(state_type dir)
    {
        assert(dir >= 0 && dir < DIR_NUMBER);
        state_type inv[] = { DIR_SE, DIR_SW, DIR_NE, DIR_NW };
        return inv[dir];
    }
    // отражение направления от границ стола (если требуется)
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
        // инициализация координат и направления
        m_coords = coords_type(m_input[INPUT_INIT_X], m_input[INPUT_INIT_Y]);
        state = m_input[INPUT_INIT_DIR];
        // выводим координаты
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
            // вычисляем последующее направление шаров
            state_type mydir = reflect(state, m_coords);
            state_type otherdir = reflect(m_input[INPUT_OTHER_DIR], other);
            // проверяем совпадение координат (другой шар
            // может быть в текущей позиции или в следующей)
            if (m_coords == other || update(mydir, m_coords) == other)
            {
                // если направления противоположны
                if (invert(mydir) == otherdir)
                {
                    // выполняется столкновение (смена направления)
                    mydir = invert(mydir);
                    // и снова отражение от возможных границ
                    mydir = reflect(mydir, m_coords);
                };
            };
            // меняем текущие координаты
            m_coords = update(mydir, m_coords);
            // выводим их вместе с новым направлением
            m_output[OUTPUT_X] = m_coords.first;
            m_output[OUTPUT_Y] = m_coords.second;
            state = m_output[OUTPUT_DIR] = mydir;
        }
        else
            // закончился сигнал работы - завершаемся
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
    // размеры стола
    const int m_xsize, m_ysize;
    // количество выполненных шагов движения
    signal_type m_steps;
    // начальное положение и направление движения шаров
    signals_type m_stored;

    state_type do_start(state_type state)
    {
        m_output[OUTPUT_ON] = 1;
        return STATE_ON;
    }

    state_type do_step(state_type state)
    {
        // выполняем какое-либо отображение стола и шаров
        // ...
        // в текущем варианте в текстовом виде:
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

        // запоминаем положение шаров (на первом шаге)
        if (m_steps == 0)
            m_stored = m_input;

        // на остальных проверяем совпадение
        if (m_steps > 0 && m_stored == m_input)
        {
            // если совпало, завершаем цикл работы
            state = STATE_OUT;
            m_output[OUTPUT_ON] = 0;
            m_output[OUTPUT_PERIOD] = m_steps;
        }
        else
        {
            // иначе переходим к следующему шагу
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
            // передача информации о координатах и направлении шаров
            for (int i = 0; i < ball_type::OUTPUT_NUMBER; ++i)
            {
                // начальные координаты и направление шаров
                links.input_to_fsm(
                    INPUT_INIT_X1 + i,
                    FSM_BALL1, ball_type::INPUT_INIT_X + i);
                links.input_to_fsm(
                    INPUT_INIT_X2 + i,
                    FSM_BALL2, ball_type::INPUT_INIT_X + i);
                // информация от шаров на входы стола
                links.fsm_to_fsm(
                    FSM_BALL1, i,
                    FSM_TABLE, table_type::INPUT_X1 + i);
                links.fsm_to_fsm(
                    FSM_BALL2, i,
                    FSM_TABLE, table_type::INPUT_X2 + i);
                // информация от шаров друг другу
                links.fsm_to_fsm(
                    FSM_BALL1, i,
                    FSM_BALL2, ball_type::INPUT_OTHER_X + i);
                links.fsm_to_fsm(
                    FSM_BALL2, i,
                    FSM_BALL1, ball_type::INPUT_OTHER_X + i);
            };
            // сигнал включения от стола к шарам
            links.fsm_to_fsm(
                FSM_TABLE, table_type::OUTPUT_ON,
                FSM_BALL1, ball_type::INPUT_ON);
            links.fsm_to_fsm(
                FSM_TABLE, table_type::OUTPUT_ON,
                FSM_BALL2, ball_type::INPUT_ON);
            // выход сети - искомая величина периода
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
        // на первом шаге рядом с шарами не должно быть препятствий
        // (каждый шар не должен примыкать к границе или другому шару)
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
