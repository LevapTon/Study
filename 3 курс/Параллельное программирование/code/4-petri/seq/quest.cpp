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


#include <string>
#include <deque>
#include "zpetri.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// позиция сети Петри (состояние какого-либо элемента в игре)
class gstate_type: public place_type
{
private:
    int type;
public:
    enum {
        OUTSPACE,   // герой снаружи
        ROOM1,      // герой в комнате 1
        ROOM2,      // герой в комнате 2
        ROOM3,      // герой в комнате 3
        ROOM4,      // герой в комнате 4
        SWOFF,      // рычаг в положении "выкл"
        SWON,       // рычаг в положении "вкл"
        KLYING,     // ключ на полу
        KTAKEN,     // ключ у героя
        KUSED,      // герой использовал ключ
        DLYING,     // гиря на полу
        DTAKEN,     // гиря взята героем
        DHAVENO,    // у героя нет гири (может взять)
        DPLACED,    // гиря помещена на весы
        NUMBER      // полное количество позиций
    };
    explicit
    gstate_type(int t): type(t)
    {
        assert(type >=0 && type < NUMBER);
    }
    // ...
    string what(void) const
    {
        const char *name[] = {
            "we are in outspace",
            "we are in room 1",
            "we are in room 2",
            "we are in room 3",
            "we are in room 4",
            "switch is off",
            "switch is on",
            "key is somewere on the floor",
            "we have a key",
            "key is used",
            "dumbbell is somewere on the floor",
            "we have a dumbbell",
            "we can take a dumbbell",
            "dumbbell is placed on the scale platform"
        };
        return name[type];
    }
};

// переход сети Петри (какое-либо действие в игре)
class gaction_type: public transition_simple_type
{
private:
    int type;
public:
    enum {
        EXIT,       // покинуть помещение
        LEFT21,     // пройти влево из 2 комнаты в 1
        RIGHT12,    // пройти вправо из 1 комнаты во 2
        LEFT34,     // пройти влево из 3 комнаты в 4
        RIGHT43,    // пройти вправо из 4 комнаты в 3
        UP32,       // подняться из 3 комнаты во 2
        DOWN23,     // спуститься из 2 комнаты в 3
        TURNON,     // повернуть рычаг в положение "вкл"
        TURNOFF,    // повернуть рычаг в положение "выкл"
        GETKEY,     // взять ключ
        USEKEY,     // отпереть дверь
        GETDMB,     // взять гирю
        PUTDMB,     // положить гирю на весы
        NUMBER      // полное количество переходов
    };
    explicit
    gaction_type(int t): type(t)
    {
        assert(type >=0 && type < NUMBER);
    }
    // ...
    string what(void) const
    {
        const char *name[] = {
            "exit",
            "move left",
            "move right",
            "move left",
            "move right",
            "move up",
            "move down",
            "turn switch on",
            "turn switch off",
            "find the key",
            "open the door",
            "get dumbbell",
            "put dumbbell"
        };
        return name[type];
    }
};

// окружение сети Петри (взаимодействие игры с игроком)
class genv_type: public petrinet_type::environment_abstract_type
{
public:
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        // отображение игроку текущего состояния игры
        // (на основе содержимого marked)
        // ...
        {
            petrinet_type::markedlist_type::const_iterator it;
            for (it = marked.begin(); it != marked.end(); ++it)
                for (int i = 0; i < it->second; ++i)
                    fprintf(stdout, "%s\n",
                        static_cast<gstate_type *>(it->first)->what().c_str());
        };

        // предложение игроку возможных вариантов действий
        // (на основе содержимого enabled)
        // ...
        {
            petrinet_type::enabledlist_type::const_iterator it;
            for (it = enabled.begin(); it != enabled.end(); ++it)
                fprintf(stdout, "%d: %s\n",
                    std::distance(enabled.begin(), it),
                    static_cast<gaction_type *>(*it)->what().c_str());
        };

        // получение от игрока номера выбранного действия
        // int choice = ...
        int choice = -1;
        do
        {
            assert(!feof(stdin));
            fputs("action number> ", stdout);
            char buf[256] = "";
            fgets(buf, sizeof(buf) / sizeof(buf[0]), stdin);
            sscanf(buf, "%d\n", &choice);
        } while (choice < 0 || size_t(choice) >= enabled.size());
        // сообщим игроку его выбор
        fprintf(stdout, "ok, action %d: %s\n\n",
            choice,
            static_cast<gaction_type *>(enabled[choice])->what().c_str());

        return choice;
    }
};

int main(int argc, char *argv[])
{
    // заполнение содержимого сети Петри
    petrinet_type::content_type content;

    // внесение объектов-позиций
    deque<gstate_type> p;
    for (int i = 0; i < gstate_type::NUMBER; ++i)
    {
        p.push_back(gstate_type(i));
        content.add_place(p.back());
    };
    // внесение объектов-переходов
    deque<gaction_type> t;
    for (int i = 0; i < gaction_type::NUMBER; ++i)
    {
        t.push_back(gaction_type(i));
        content.add_transition(t.back());
    };

    // дуги фрагмента перемещения героя
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::RIGHT12]);
    content.add_arc(t[gaction_type::RIGHT12], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::LEFT21]);
    content.add_arc(t[gaction_type::LEFT21], p[gstate_type::ROOM1]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::DOWN23]);
    content.add_arc(t[gaction_type::DOWN23], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::UP32]);
    content.add_arc(t[gaction_type::UP32], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::LEFT34]);
    content.add_arc(t[gaction_type::LEFT34], p[gstate_type::ROOM4]);
    content.add_arc(p[gstate_type::ROOM4], t[gaction_type::RIGHT43]);
    content.add_arc(t[gaction_type::RIGHT43], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::EXIT]);
    content.add_arc(t[gaction_type::EXIT], p[gstate_type::OUTSPACE]);
    // возможность перемещения в зависимости от состояния дверей
    content.add_arc(p[gstate_type::KUSED], t[gaction_type::LEFT21]);
    content.add_arc(t[gaction_type::LEFT21], p[gstate_type::KUSED]);
    //~ content.add_arc(p[gstate_type::KUSED], t[gaction_type::RIGHT12]);
    //~ content.add_arc(t[gaction_type::RIGHT12], p[gstate_type::KUSED]);
    content.add_arc(p[gstate_type::SWON], t[gaction_type::LEFT34]);
    content.add_arc(t[gaction_type::LEFT34], p[gstate_type::SWON]);
    //~ content.add_arc(p[gstate_type::SWON], t[gaction_type::RIGHT43]);
    //~ content.add_arc(t[gaction_type::RIGHT43], p[gstate_type::SWON]);
    content.add_arc(p[gstate_type::DPLACED], t[gaction_type::EXIT], 2);

    // дуги фрагмента переключения рычага
    content.add_arc(p[gstate_type::SWOFF], t[gaction_type::TURNON]);
    content.add_arc(t[gaction_type::TURNON], p[gstate_type::SWON]);
    content.add_arc(p[gstate_type::SWON], t[gaction_type::TURNOFF]);
    content.add_arc(t[gaction_type::TURNOFF], p[gstate_type::SWOFF]);
    // зависимости возможности переключения от расположения героя
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::TURNON]);
    content.add_arc(t[gaction_type::TURNON], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::TURNOFF]);
    content.add_arc(t[gaction_type::TURNOFF], p[gstate_type::ROOM2]);

    // дуги фрагмента операций с ключом
    content.add_arc(p[gstate_type::KLYING], t[gaction_type::GETKEY]);
    content.add_arc(t[gaction_type::GETKEY], p[gstate_type::KTAKEN]);
    content.add_arc(p[gstate_type::KTAKEN], t[gaction_type::USEKEY]);
    content.add_arc(t[gaction_type::USEKEY], p[gstate_type::KUSED]);
    // зависимости от расположения героя
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::GETKEY]);
    content.add_arc(t[gaction_type::GETKEY], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::USEKEY]);
    content.add_arc(t[gaction_type::USEKEY], p[gstate_type::ROOM2]);

    // дуги фрагмента операций с гирями
    content.add_arc(p[gstate_type::DLYING], t[gaction_type::GETDMB]);
    content.add_arc(p[gstate_type::DHAVENO], t[gaction_type::GETDMB]);
    content.add_arc(t[gaction_type::GETDMB], p[gstate_type::DTAKEN]);
    content.add_arc(p[gstate_type::DTAKEN], t[gaction_type::PUTDMB]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::DHAVENO]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::DPLACED]);
    // зависимости от расположения
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::GETDMB]);
    content.add_arc(t[gaction_type::GETDMB], p[gstate_type::ROOM1]);
    content.add_arc(p[gstate_type::ROOM4], t[gaction_type::PUTDMB]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::ROOM4]);

    // начальная разметка
    content.add_token(p[gstate_type::DLYING], 2);
    content.add_token(p[gstate_type::DHAVENO]);
    content.add_token(p[gstate_type::KLYING]);
    content.add_token(p[gstate_type::SWOFF]);
    content.add_token(p[gstate_type::ROOM2]);

    // создание и выполнение сети Петри
    genv_type env;
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
