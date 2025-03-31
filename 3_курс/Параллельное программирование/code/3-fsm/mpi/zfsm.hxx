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


#ifndef _ZFSM_HXX_
#define _ZFSM_HXX_

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace z {
namespace fsm {

// абстрактный тип автомата
class fsm_abstract_type
{
public:
    // тип состояния автомата
    typedef int state_type;
    // тип входных и выходных данных
    typedef int signal_type;
    // полный набор входных или выходных сигналов
    typedef std::vector<signal_type> signals_type;

    // количество входов
    virtual
    int number_input(void) const = 0;
    // количество выходов
    virtual
    int number_output(void) const = 0;
    // передать автомату вектор входных данных
    virtual
    void put_input(const signals_type &input) = 0;
    // выполнить такт работы автомата
    virtual
    void do_work(void) = 0;
    // получить от автомата вектор выходных данных
    virtual
    signals_type get_output(void) const = 0;
    // проверка, находится ли автомат в начальном состоянии
    virtual
    bool is_off(void) const = 0;
};

// конечный автомат
class fsm_type: public fsm_abstract_type
{
protected:
    // обработчик, вызываемый из do_work в конкретном состоянии
    typedef state_type (fsm_type::*handler_type)(state_type state);
    // начальное/конечное состояние
    enum { STATE_OFF = -1 };

private:
    // текущее состояние
    state_type m_state;
    // таблица обработчиков
    typedef std::map<state_type, handler_type> handlertable_type;
    handlertable_type m_handlertable;

protected:
    // входные и выходные сигналы автомата на текущем такте
    signals_type m_input, m_output;

    void add_handler(state_type state, handler_type handler)
    {
        m_handlertable[state] = handler;
    }

public:
    fsm_type(int inputnum, int outputnum):
        m_state(STATE_OFF),
        m_input(inputnum, 0), m_output(outputnum, 0)
    {}

    int number_input(void) const
    {
        return m_input.size();
    }

    int number_output(void) const
    {
        return m_output.size();
    }

    void put_input(const signals_type &input)
    {
        assert(input.size() == m_input.size());
        m_input.assign(input.begin(), input.end());
    }

    void do_work(void)
    {
        signals_type::size_type insize = m_input.size();
        signals_type::size_type outsize = m_output.size();

        // найти обработчик текущего состояния и вызвать его
        handlertable_type::iterator it = m_handlertable.find(m_state);
        assert(it != m_handlertable.end());
        m_state = (this->*(it->second))(m_state);

        assert(insize == m_input.size());
        assert(outsize == m_output.size());

        // если автомат выключился, обнулить выходы
        if (m_state == STATE_OFF)
            m_output.assign(m_output.size(), 0);
    }

    signals_type get_output(void) const
    {
        return m_output;
    }

    bool is_off(void) const
    {
        // проверка завершения работы
        return m_state == STATE_OFF;
    }
};

// сеть конечных автоматов
class fsmnet_type: public fsm_abstract_type
{
private:
    // список связей автоматов по входам и выходам
    // в форме (№ автомата, № выхода) -> (№ автомата, № входа)
    typedef std::list<
        std::pair<std::pair<int, int>, std::pair<int, int> >
    > linklist_type;
    // два псевдономера для обозначения входа и выхода сети
    enum { PSEUDOFSM_NETINPUT = -1, PSEUDOFSM_NETOUTPUT = -2 };

    // класс для работы с общими данными
    class shared_area_type
    {
    private:
        enum { NOPOS = -1 };
        // общие данные (выходы всех автоматов и входы сети)
        signals_type m_data;
        // позиции начал и размеры областей выходов автоматов и входов сети
        std::vector<int> m_outpos, m_outsize;
        // списки позиций входов автоматов и выходов сети
        std::vector<std::vector<int> > m_allinpos;

    public:
        // в конструкторе на основе входящего списка связей формируем
        // область общих данных, список позиций выходных областей
        // автоматов, а также списки позиций конкретных входов автоматов
        shared_area_type(const linklist_type &linklist)
        {
            linklist_type::const_iterator it;
            // полное количество автоматов = максимальный указанный + 1
            int fsmnum = -1;
            for (it = linklist.begin(); it != linklist.end(); ++it)
            {
                fsmnum = std::max(fsmnum, it->first.first);
                fsmnum = std::max(fsmnum, it->second.first);
            };
            ++fsmnum;

            // количества выходов автоматов (сначала) и входов сети (последний)
            std::vector<int> outsize(fsmnum + 1);
            // количества входов автоматов (сначала) и выходов сети (последний)
            std::vector<int> insize(fsmnum + 1);
            // ищем максимальные номера входов и выходов для всех
            for (it = linklist.begin(); it != linklist.end(); ++it)
            {
                int idxout = it->first.first;
                int idxin = it->second.first;
                idxout = (idxout != PSEUDOFSM_NETINPUT) ? idxout : fsmnum;
                idxin = (idxin != PSEUDOFSM_NETOUTPUT) ? idxin : fsmnum;

                assert(it->first.first != PSEUDOFSM_NETOUTPUT);
                assert(it->second.first != PSEUDOFSM_NETINPUT);
                outsize[idxout] = std::max(outsize[idxout], it->first.second);
                insize[idxin] = std::max(insize[idxin], it->second.second);
            };
            // корректируем до размеров (добавляем по единице)
            std::transform(
                outsize.begin(), outsize.end(),
                outsize.begin(), std::bind2nd(std::plus<int>(), 1));
            std::transform(
                insize.begin(), insize.end(),
                insize.begin(), std::bind2nd(std::plus<int>(), 1));

            // формируем список позиций начал областей выходов
            // первая область по нулевому смещению
            m_outpos.assign(1, 0);
            // смещения остальных - частичные суммы размеров областей
            std::partial_sum(
                outsize.begin(), outsize.end(),
                std::back_inserter(m_outpos));
            // последний (лишний) элемент - полное количество всех выходов
            m_data.assign(m_outpos.back(), 0);
            m_outpos.pop_back();
            // размеры выходных областей в массиве общих данных
            m_outsize.swap(outsize);

            // создаем списки позиций входов
            m_allinpos.resize(fsmnum + 1);
            for (int i = 0; i < fsmnum + 1; ++i)
                m_allinpos[i].resize(insize[i], NOPOS);
            // заполняем списки позиций входов
            for (it = linklist.begin(); it != linklist.end(); ++it)
            {
                int idxout = it->first.first;
                int idxin = it->second.first;
                idxout = (idxout != PSEUDOFSM_NETINPUT) ? idxout : fsmnum;
                idxin = (idxin != PSEUDOFSM_NETOUTPUT) ? idxin : fsmnum;

                // каждый вход может быть подключен лишь к одному выходу
                assert(m_allinpos[idxin][it->second.second] == NOPOS);
                m_allinpos[idxin][it->second.second] =
                    m_outpos[idxout] + it->first.second;
            };
        }
        // сохранение выходных данных конкретного автомата
        void put_output(int i, const signals_type &output)
        {
            // синхронизация всех автоматов (для отделения от чтения входов)
            MPI_Barrier(MPI_COMM_WORLD);
            assert(size_t(m_outsize[i]) == output.size());
            std::copy(
                output.begin(), output.end(),
                m_data.begin() + m_outpos[i]);
            // выполним полный обмен - раскидаем все выходы всем процессам
            // в конце массивов размеров и смещений есть по одному
            // лишнему элементу, но это не мешает - они не учитываются
            MPI_Allgatherv(
                &m_data[m_outpos[i]], m_outsize[i], MPI_INT,
                &m_data[0], &m_outsize.front(), &m_outpos.front(), MPI_INT,
                MPI_COMM_WORLD);
        }
        // сохранение входных данных сети
        void put_input(const signals_type &input)
        {
            assert(size_t(m_outsize.back()) == input.size());
            std::copy(
                input.begin(), input.end(),
                m_data.begin() + m_outpos.back());
        }
        // восстановление входных данных конкретного автомата
        signals_type get_input(int i) const
        {
            int signum = m_allinpos[i].size();
            signals_type input(signum);
            for (int j = 0; j < signum; ++j)
            {
                assert(m_allinpos[i][j] != NOPOS);
                input[j] = m_data[m_allinpos[i][j]];
            };
            return input;
        }
        // восстановление выходных данных сети
        signals_type get_output(void) const
        {
            int signum = m_allinpos.back().size();
            signals_type output(signum);
            for (int j = 0; j < signum; ++j)
            {
                assert(m_allinpos.back()[j] != NOPOS);
                output[j] = m_data[m_allinpos.back()[j]];
            };
            return output;
        }
    };

public:
    // абстрактный тип фабрики автоматной сети
    class factory_abstract_type
    {
    public:
        // хранилище связей (для упрощения их добавления)
        class links_type
        {
        private:
            linklist_type m_linklist;
        public:
            void fsm_to_fsm(int srcfsm, int srcnum, int dstfsm, int dstnum)
            {
                m_linklist.push_back(linklist_type::value_type(
                    linklist_type::value_type::first_type(srcfsm, srcnum),
                    linklist_type::value_type::second_type(dstfsm, dstnum)));
            }
            void input_to_fsm(int srcnum, int dstfsm, int dstnum)
            {
                fsm_to_fsm(PSEUDOFSM_NETINPUT, srcnum, dstfsm, dstnum);
            }
            void fsm_to_output(int srcfsm, int srcnum, int dstnum)
            {
                fsm_to_fsm(srcfsm, srcnum, PSEUDOFSM_NETOUTPUT, dstnum);
            }
            linklist_type get(void) const
            {
                return m_linklist;
            }
        };

        // количество автоматов в сети
        virtual
        int number_fsm(void) const = 0;
        // количество входов сети
        virtual
        int number_input(void) const = 0;
        // количество выходов сети
        virtual
        int number_output(void) const = 0;
        // получение списка всех связей
        virtual
        links_type links(void) const = 0;
        // создание очередного автомата в сети
        virtual
        fsm_abstract_type *create_fsm(int id) = 0;
        // уничтожение его же
        virtual
        void destroy_fsm(int id, fsm_abstract_type *pfsm) = 0;
    };

private:

    // список активных автоматов
    std::vector<fsm_abstract_type *> m_pfsm;
    // фабрика сети
    factory_abstract_type &m_factory;
    // общая область памяти
    shared_area_type m_shared;

    // номер автомата текущей сети, реализуемого нашим процессом
    int m_fsmid;

public:
    fsmnet_type(factory_abstract_type &factory):
        m_factory(factory), m_shared(m_factory.links().get())
    {
        int size;
        MPI_Comm_rank(MPI_COMM_WORLD, &m_fsmid);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        assert(size == m_factory.number_fsm());
        // создание автомата
        m_pfsm.push_back(m_factory.create_fsm(m_fsmid));
    }

    ~fsmnet_type(void)
    {
        // уничтожение автомата
        m_factory.destroy_fsm(m_fsmid, m_pfsm.back());
    }

    int number_input(void) const
    {
        return m_factory.number_input();
    }

    int number_output(void) const
    {
        return m_factory.number_output();
    }

    void put_input(const signals_type &input)
    {
        m_shared.put_input(input);
    }

    void do_work(void)
    {
        // прочитать входные данные, выполнить действия
        m_pfsm.back()->put_input(m_shared.get_input(m_fsmid));
        m_pfsm.back()->do_work();
        // записать выходные (запись должна быть отделена от чтения)
        m_shared.put_output(m_fsmid, m_pfsm.back()->get_output());
    }

    signals_type get_output(void) const
    {
        return m_shared.get_output();
    }

    bool is_off(void) const
    {
        // сеть в выключенном состоянии, когда все автоматы выключены
        int isoff = m_pfsm.back()->is_off();
        int allisoff;
        MPI_Allreduce(&isoff, &allisoff, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        return allisoff;
    }
};

} // namespace fsm
} // namespace z

#endif /* _ZFSM_HXX_ */
