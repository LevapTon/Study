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


#include "zfsm.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::fsm;

// класс автомата A
class fsm_a_type: public fsm_type
{
public:
    enum { INPUT0, INPUT1, INPUT2, INPUT3, INPUT_NUMBER };
    enum { OUTPUT0, OUTPUT1, OUTPUT2, OUTPUT_NUMBER };
    enum { STATE0, STATE1 /* ... прочие состояния */ };
private:
    state_type do_something(state_type state)
    {
        if (state == STATE_OFF && m_input[INPUT0] == /* ... */)
        {
            state = STATE0;
            m_output[OUTPUT0] = /* ... */;
            // ... заполнение остальных выходов
        };
        // ... обработка прочих условий
        return state;
    }
public:
    fsm_a_type(void):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER)
    {
        add_handler(STATE_OFF, handler_type(&fsm_a_type::do_something));
        // ... прочие обработчики
    }
};

// класс автомата B
class fsm_b_type: public fsm_type
{
    // ...
public:
    enum { INPUT0, INPUT1, INPUT2, INPUT3, INPUT_NUMBER };
    enum { OUTPUT0, OUTPUT1, OUTPUT2, OUTPUT_NUMBER };
    enum { STATE0, STATE1 /* ... прочие состояния */ };
private:
    state_type do_something(state_type state)
    {
        // ... обработка прочих условий
        return state;
    }
public:
    fsm_b_type(void):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER)
    {
        add_handler(STATE_OFF, handler_type(&fsm_b_type::do_something));
        // ... прочие обработчики
    }
};

// класс автомата C
class fsm_c_type: public fsm_type
{
    // ...
public:
    enum { INPUT0, INPUT1, INPUT2, INPUT3, INPUT_NUMBER };
    enum { OUTPUT0, OUTPUT1, OUTPUT_NUMBER };
    enum { STATE0, STATE1 /* ... прочие состояния */ };
private:
    state_type do_something(state_type state)
    {
        // ... обработка прочих условий
        return state;
    }
public:
    fsm_c_type(void):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER)
    {
        add_handler(STATE_OFF, handler_type(&fsm_c_type::do_something));
        // ... прочие обработчики
    }
};

// сеть из трех автоматов
class fsmnet_abc_type: public fsmnet_type
{
public:
    enum { FSM_A, FSM_B, FSM_C, FSM_NUMBER };
    enum { INPUT0, INPUT1, INPUT_NUMBER };
    enum { OUTPUT0, OUTPUT1, OUTPUT2, OUTPUT3, OUTPUT_NUMBER };

    // фабрика сети
    class factory_type: public factory_abstract_type
    {
    public:
        int number_fsm(void) const { return FSM_NUMBER; }
        int number_input(void) const { return INPUT_NUMBER; }
        int number_output(void) const { return OUTPUT_NUMBER; }
        links_type links(void) const
        {
            links_type links;
            // связи входов сети с входами автоматов
            links.input_to_fsm(INPUT0, FSM_A, fsm_a_type::INPUT0);
            links.input_to_fsm(INPUT1, FSM_A, fsm_a_type::INPUT1);
            links.input_to_fsm(INPUT0, FSM_B, fsm_b_type::INPUT1);
            links.input_to_fsm(INPUT1, FSM_B, fsm_b_type::INPUT0);
            // связи автоматов между собой
            links.fsm_to_fsm(FSM_A, fsm_a_type::OUTPUT0, FSM_C, fsm_c_type::INPUT2);
            links.fsm_to_fsm(FSM_A, fsm_a_type::OUTPUT1, FSM_B, fsm_b_type::INPUT3);
            links.fsm_to_fsm(FSM_A, fsm_a_type::OUTPUT1, FSM_C, fsm_c_type::INPUT0);
            links.fsm_to_fsm(FSM_A, fsm_a_type::OUTPUT2, FSM_B, fsm_b_type::INPUT2);
            links.fsm_to_fsm(FSM_B, fsm_b_type::OUTPUT0, FSM_C, fsm_c_type::INPUT3);
            links.fsm_to_fsm(FSM_B, fsm_b_type::OUTPUT1, FSM_A, fsm_a_type::INPUT3);
            links.fsm_to_fsm(FSM_B, fsm_b_type::OUTPUT1, FSM_C, fsm_c_type::INPUT1);
            links.fsm_to_fsm(FSM_B, fsm_b_type::OUTPUT2, FSM_A, fsm_a_type::INPUT2);
            // связи выходов автоматов с выходами сети
            links.fsm_to_output(FSM_A, fsm_a_type::OUTPUT0, OUTPUT0);
            links.fsm_to_output(FSM_C, fsm_c_type::OUTPUT0, OUTPUT1);
            links.fsm_to_output(FSM_C, fsm_c_type::OUTPUT1, OUTPUT2);
            links.fsm_to_output(FSM_B, fsm_b_type::OUTPUT0, OUTPUT3);
            return links;
        }
        fsm_abstract_type *create_fsm(int id)
        {
            if (id == FSM_A)
                return new fsm_a_type();
            else if (id == FSM_B)
                return new fsm_b_type();
            else
                return new fsm_c_type();
        }
        void destroy_fsm(int id, fsm_abstract_type *pfsm)
        {
            if (id == FSM_A)
                delete dynamic_cast<fsm_a_type *>(pfsm);
            else if (id == FSM_B)
                delete dynamic_cast<fsm_b_type *>(pfsm);
            else
                delete dynamic_cast<fsm_c_type *>(pfsm);
        }
    };

    fsmnet_abc_type(factory_type &factory):
        fsmnet_type(factory)
    {}
};

// цикл работы конечного автомата
// на входе: автомат, интервал наборов входных сигналов
// и начало интервала наборов выходных сигналов
template <typename InIt, typename OutIt>
OutIt run_fsm(fsm_abstract_type &fsm, InIt ib, InIt ie, OutIt ob)
{
    // цикл работы автомата
    do
    {
        // подать входные сигналы, если есть, или обнуленные
        fsm.put_input((ib != ie) ?
            *ib++ :
            fsm_type::signals_type(fsm.number_input(), 0));
        // выполнить такт работы автомата
        fsm.do_work();
        // получить очередные выходные сигналы
        *ob++ = fsm.get_output();
        // проверить, выключен ли автомат
    } while (!fsm.is_off());
    return ob;
}

int main(int argc, char *argv[])
{
    // контейнер наборов входных сигналов
    vector<fsm_type::signals_type> allins;
    // ... инициализация входных сигналов

    // контейнер всех наборов выходных сигналов
    vector<fsm_type::signals_type> allouts;

    // фабрика автоматной сети и сама сеть
    fsmnet_abc_type::factory_type factory;
    fsmnet_abc_type fsmnet(factory);

    // выполнение автоматной сети
    run_fsm(fsmnet,
        allins.begin(), allins.end(),
        back_inserter(allouts));
    // по завершении работы сети allouts содержит все наборы
    // выходных сигналов сети, включая последний (обнуленный)

    return 0;
}
