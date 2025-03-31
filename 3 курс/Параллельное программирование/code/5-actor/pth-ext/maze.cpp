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


#include <sstream>
#include <iomanip>
#include "zactor.hxx"

using namespace z;
using namespace z::actor;
using namespace std;

// лабиринт
class maze_type
{
private:
    // направления движения и их количество
    enum { NORTH, EAST, SOUTH, WEST, DIR_NUM };
    // конфигурация лабиринта
    const int m_width;
    const string m_data;

public:
    // позиция в лабиринте
    class position_type
    {
    private:
        friend class maze_type;
        // координаты
        int m_x, m_y;
        // направление
        int m_dir;

    public:
        // следующая позиция в прямом направлении
        position_type forward(void) const
        {
            position_type pos = *this;
            if (m_dir == NORTH || m_dir == SOUTH)
                pos.m_y += (m_dir == NORTH) ? -1 : 1;
            if (m_dir == WEST || m_dir == EAST)
                pos.m_x += (m_dir == WEST) ? -1 : 1;
            return pos;
        }
        // следующая позиция при движении налево
        position_type left(void) const
        {
            position_type pos = *this;
            pos.m_dir = (m_dir + DIR_NUM - 1) % DIR_NUM;
            return pos.forward();
        }
        // следующая позиция при движении направо
        position_type right(void) const
        {
            position_type pos = *this;
            pos.m_dir = (m_dir + 1) % DIR_NUM;
            return pos.forward();
        }
        // оператор вывода информации о позиции
        friend
        ostream & operator <<(ostream &o, const position_type &pos)
        {
            return (o << '(' << pos.m_x << ';' << pos.m_y << ')');
        }
    };

    // обозначения в data: i - вход, o - выход, # - стена;
    // по всей границе должна быть непрерывная стена;
    // вход должен прилегать хотя бы к одной стене;
    // не должно быть замкнутых контуров;
    // не должно быть коридоров шире одной клетки.
    maze_type(int width, const string &data):
        m_width(width), m_data(data)
    {
        assert(m_width > 0);
        assert(m_data.size() % m_width == 0);
        assert(m_data.find_first_not_of(" io#") == string::npos);
    }
    // получение начальной позиции
    position_type get_in(void) const
    {
        // найдем вход
        int i = m_data.find_first_of('i');
        assert(string::size_type(i) != string::npos);
        // вычислим координаты
        position_type pos;
        pos.m_x = i % m_width;
        pos.m_y = i / m_width;
        // найдем прилегающую стену
        for (i = 0; i < DIR_NUM; ++i)
        {
            pos.m_dir = i;
            if (is_wall(pos.forward()))
                break;
        };
        assert(i != DIR_NUM);
        // встанем к ней спиной
        pos.m_dir = (i + DIR_NUM / 2) % DIR_NUM;
        return pos;
    }
    // есть ли тут препятствие?
    bool is_wall(const position_type &pos) const
    {
        return m_data[pos.m_y * m_width + pos.m_x] == '#';
    }
    // тут ли выход?
    bool is_out(const position_type &pos) const
    {
        return m_data[pos.m_y * m_width + pos.m_x] == 'o';
    }
};

// конфигурация лабиринта
enum { WIDTH = 15 };
const char c_mazestr[] =
    "###############"
    "#     #     #i#"
    "# # # ## ## # #"
    "# ###     #   #"
    "### ### # ### #"
    "#       #   # #"
    "# # ### ### ###"
    "# #   ### #   #"
    "# ### #   ### #"
    "#   # # #     #"
    "### # # #######"
    "#   ###   #   #"
    "# # # ## ### ##"
    "#o#    #      #"
    "###############";
// объект-лабиринт
const maze_type c_maze(WIDTH, c_mazestr);

// актер-путник
class tracer_type: public actor_type
{
private:
    // актер, которому надлежит послать результат
    address_type m_prn;
    // актер-путник на предыдущем шаге
    address_type m_prev;
    // глубина пройденного пути
    int m_depth;
    // позиция текущего актера
    maze_type::position_type m_pos;

public:
    struct init_type
    {
        address_type print;
        address_type prev;
        int depth;
        maze_type::position_type pos;
    };
    struct success_type {};
    struct move_type {};

    tracer_type(const init_type &init):
        m_prn(init.print),
        m_prev(init.prev),
        m_depth(init.depth),
        m_pos(init.pos)
    {
        add_action<tracer_type, success_type>();
        add_action<tracer_type, move_type>();
    }

    void action(const success_type &msg)
    {
        // формируем информационное сообщение
        print_type::strmsg_type strmsg;
        ostringstream ostr;
        ostr << "step " << setw(3) << m_depth << ": " << m_pos << ends;
        ostr.str().copy(strmsg.str, print_type::strmsg_type::MAX_SIZE);
        // отправляем его актеру print
        send(m_prn, strmsg);
        // передаем сообщение об успехе дальше по цепочке
        send(m_prev, success_type());
    }

    void action(const move_type &msg)
    {
        // если выход найден, пошлем себе сообщение об успехе
        if (c_maze.is_out(m_pos))
            send(self(), success_type());
        else
        {
            // иначе инициируем проработку до трех новых направлений
            init_type init = { m_prn, self(), m_depth + 1, m_pos };
            if (!c_maze.is_wall(m_pos.forward()))
            {
                init.pos = m_pos.forward();
                send(create<tracer_type>(init), move_type());
            };
            if (!c_maze.is_wall(m_pos.left()))
            {
                init.pos = m_pos.left();
                send(create<tracer_type>(init), move_type());
            };
            if (!c_maze.is_wall(m_pos.right()))
            {
                init.pos = m_pos.right();
                send(create<tracer_type>(init), move_type());
            };
        };
    }
};

int main(int argc, char *argv[])
{
    factory_type factory;
    factory.add_definition<tracer_type, tracer_type::init_type>();
    factory.add_definition<print_type, actor_type::empty_type>();
    factory.add_definition<sink_type, actor_type::empty_type>();

    scheduler_type sched;
    address_type print = sched.system().create<print_type>();
    address_type sink = sched.system().create<sink_type>();
    tracer_type::init_type init = {
        print, sink, 0, c_maze.get_in()
    };
    sched.system().send(
        sched.system().create<tracer_type>(init),
        tracer_type::move_type());
    sched.evolve(factory);

    return 0;
}
