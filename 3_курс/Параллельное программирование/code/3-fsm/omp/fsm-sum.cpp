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


#include <deque>
#include <numeric>
#include "zfsm.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::fsm;

class calc_type: public fsm_type
{
public:
    enum { INPUT_ARG1, INPUT_ARG2, INPUT_NUMBER };
    enum { OUTPUT_RESULT, OUTPUT_NUMBER };
    enum { STATE_OUT };

private:
    state_type do_calc(state_type state)
    {
        if (m_input[INPUT_ARG1] || m_input[INPUT_ARG2])
        {
            state = STATE_OUT;
            m_output[OUTPUT_RESULT] = m_input[INPUT_ARG1] + m_input[INPUT_ARG2];
        }
        else
            state = STATE_OFF;
        return state;
    }
public:
    calc_type(void):
        fsm_type(INPUT_NUMBER, OUTPUT_NUMBER)
    {
        add_handler(STATE_OFF, handler_type(&calc_type::do_calc));
        add_handler(STATE_OUT, handler_type(&calc_type::do_calc));
    }
};

class ctrl_type: public fsm_type
{
public:
    enum { OUTPUT_RESULT, OUTPUT_ARG1_0, OUTPUT_ARG2_0 };
    enum { STATE_OUT, STATE_CALC_NUMBER_0 };

private:
    deque<signal_type> m_queue;
    int m_argnum; // количество аргументов сумматора
    int m_calcnum; // количество вычислителей

    // распределить пары данных, сколько есть в наличии, на вычислители
    int dispatch(void)
    {
        // количество задействуемых вычислителей - половина размера очереди
        int num = m_queue.size() / 2;
        // но не более общего количества вычислителей
        num = (num > m_calcnum) ? m_calcnum : num;
        // переносим пары аргументов на входы num вычислителей
        // остальные вычислители, если есть, не задействуем
        fill(
            copy(
                m_queue.begin(), m_queue.begin() + num * 2,
                m_output.begin() + OUTPUT_ARG1_0),
            m_output.end(), 0);
        m_queue.erase(m_queue.begin(), m_queue.begin() + num * 2);
        return num;
    }

    state_type do_start(state_type state)
    {
        // сложим все ненулевые входы в очередь
        remove_copy(
            m_input.begin(), m_input.begin() + m_argnum,
            back_inserter(m_queue), 0);
        // если таковые есть, переключимся на обработку
        if (!m_queue.empty())
        {
            state = STATE_CALC_NUMBER_0 + dispatch();
            m_output[OUTPUT_RESULT] = 0;
        }
        else
            // иначе продолжим быть выключенными
            state = STATE_OFF;
        return state;
    }

    state_type do_calc(state_type state)
    {
        // сложить все ненулевые результаты в конец очереди
        remove_copy(
            m_input.begin() + m_argnum, m_input.end(),
            back_inserter(m_queue), 0);
        // выход, если активно 0 вычислителей и в очереди один элемент
        if (state == STATE_CALC_NUMBER_0 && m_queue.size() == 1)
        {
            state = STATE_OUT;
            m_output[OUTPUT_RESULT] = m_queue.front();
            m_queue.pop_front();
        }
        else
        {
            // распределить пары из начала очереди
            state = STATE_CALC_NUMBER_0 + dispatch();
            m_output[OUTPUT_RESULT] = 0;
        };
        return state;
    }

    state_type do_stop(state_type state)
    {
        // перейдем в выключенное состояние
        return STATE_OFF;
    }

public:
    ctrl_type(int argnum, int calcnum):
        fsm_type(
            argnum + calcnum,
            OUTPUT_ARG1_0 + calcnum * calc_type::INPUT_NUMBER),
        m_argnum(argnum), m_calcnum(calcnum)
    {
        add_handler(STATE_OFF, handler_type(&ctrl_type::do_start));
        for (int i = 0; i <= calcnum; ++i)
            add_handler(STATE_CALC_NUMBER_0 + i, handler_type(&ctrl_type::do_calc));
        add_handler(STATE_OUT, handler_type(&ctrl_type::do_stop));
    }
};

class summator_type: public fsmnet_type
{
public:
    enum { FSM_CONTROL, FSM_CALCULATOR_0 };
    enum { OUTPUT_RESULT, OUTPUT_NUMBER };

    class factory_type: public factory_abstract_type
    {
    private:
        int m_inputnum, m_outputnum, m_fsmnum;

    public:
        factory_type(int inputnum, int outputnum, int fsmnum):
            m_inputnum(inputnum), m_outputnum(outputnum), m_fsmnum(fsmnum)
        {}
        int number_fsm(void) const { return m_fsmnum; }
        int number_input(void) const { return m_inputnum; }
        int number_output(void) const { return m_outputnum; }
        links_type links(void) const
        {
            links_type links;
            // входы сети прицепим к контроллеру
            for (int i = 0; i < m_inputnum; ++i)
                links.input_to_fsm(i, FSM_CONTROL, i);
            // выходы сети - первые два выхода от контроллера
            for (int i = 0; i < m_outputnum; ++i)
                links.fsm_to_output(FSM_CONTROL, i, i);
            // все вычислители к контроллеру подцепим
            for (int i = 0; i < m_fsmnum - FSM_CALCULATOR_0; ++i)
            {
                // аргументы вычислителя от контроллера (после выходов сети)
                for (int j = 0; j < calc_type::INPUT_NUMBER; ++j)
                    links.fsm_to_fsm(
                        FSM_CONTROL, m_outputnum + calc_type::INPUT_NUMBER * i + j,
                        FSM_CALCULATOR_0 + i, j);
                // выход вычислителя - к контроллеру, после входов сети
                links.fsm_to_fsm(
                    FSM_CALCULATOR_0 + i, calc_type::OUTPUT_RESULT,
                    FSM_CONTROL, m_inputnum + i);
            };
            return links;
        }
        fsm_abstract_type *create_fsm(int id)
        {
            fsm_abstract_type *pfsm;
            if (id == FSM_CONTROL)
                pfsm = new ctrl_type(m_inputnum, m_fsmnum - 1);
            else
                pfsm = new calc_type();
            return pfsm;
        }
        void destroy_fsm(int id, fsm_abstract_type *pfsm)
        {
            if (id == FSM_CONTROL)
                delete dynamic_cast<ctrl_type *>(pfsm);
            else
                delete dynamic_cast<calc_type *>(pfsm);
        }
    };

public:
    summator_type(factory_type &factory):
        fsmnet_type(factory)
    {}
};


int main(int argc, char *argv[])
{
    // вектор входных сигналов сети
    fsm_type::signals_type ins;
    // ... инициализация вектора входных сигналов
    fsm_type::signal_type arr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    ins.assign(&arr[0], &arr[sizeof(arr) / sizeof(arr[0])]);

    // количество автоматов: контроллер+вычислители
    // в простом случае создадим два вычислителя
    int fsmnum = 1 + IF_OPENMP_ELSE(omp_get_num_procs(), 2);
    // фабрика сети и сеть
    summator_type::factory_type factory(
        ins.size(), summator_type::OUTPUT_NUMBER, fsmnum);
    summator_type fsmnet(factory);

    // результат суммирования
    fsm_type::signal_type sum = 0;
    // передача входных сигналов
    fsmnet.put_input(ins);
    for (;;)
    {
        // такт работы сети
        fsmnet.do_work();
        // проверка выключения сети
        if (fsmnet.is_off())
            break;
        // чтение выходного сигнала
        sum = fsmnet.get_output()[summator_type::OUTPUT_RESULT];
    };

    synprintf(stdout, "result: %d\n", sum);
    synprintf(stdout, "test result: %d\n", accumulate(ins.begin(), ins.end(), 0));
    return 0;
}
