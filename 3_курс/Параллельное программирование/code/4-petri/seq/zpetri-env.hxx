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


#ifndef _ZPETRI_ENV_HXX_
#define _ZPETRI_ENV_HXX_

#include <vector>
#include <deque>
#include <map>
#include <utility>
#include <cstdlib>
#include "zpetri.hxx"

namespace z {
namespace petri {

// класс окружения с произвольным выбором перехода
class randomenv_type: public petrinet_type::environment_abstract_type
{
public:
    // генерация псевдослучайного числа в интервале [0, bound)
    static
    int random(int bound)
    {
        assert(bound > 0);
        return int(rand() / (RAND_MAX + 1.0) * bound);
    }
    // срабатывает произвольный переход
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        return random(enabled.size());
    }
};

// класс окружения с поддержкой длительных операций
class threadenv_type: public randomenv_type
{
public:
    // абстрактный тип работы, выполняемой во время длительного перехода
    class longjob_abstract_type
    {
    public:
        // функция выполнения работы
        virtual
        void run(void) = 0;
    };

private:
    // переход завершения длительной операции
    class transition_stop_type: public transition_simple_type
    {
    private:
        // номер соответствующего длительного перехода
        int m_id;
    public:
        transition_stop_type(int id): m_id(id) {}
        int id(void) const { return m_id; }
    };

    // структура параметров
    struct threadparam_type
    {
        // указатель на соответствующую работу
        longjob_abstract_type *pjob;
    };

    // структура данных, связываемая с каждым длительным переходом
    struct jobdata_type
    {
        // содержимое соответствующего длительного перехода
        place_type started, stopped;
        transition_stop_type stop;
        // параметры
        threadparam_type param;
        // конструктор
        jobdata_type(int id, const threadparam_type &p): stop(id), param(p) {}
    };

public:
    // длительный переход, характеризующий выполнение работы
    class transition_long_type: public transition_compound_type
    {
    private:
        // окружение, с которым связан переход
        threadenv_type *m_penv;
        // номер перехода в списке длительных переходов окружения
        int m_id;

        // вызовы инициализации и финализации выполнения работы окружением
        void on_activate(void)
        {
            m_penv->initialize_longjob(m_id);
        }
        void on_passivate(void)
        {
            m_penv->finalize_longjob(m_id);
        }

    public:
        // конструктор длительного перехода
        // принимает ссылки на выполняемую работу и связанное окружение
        // первоначально создает пустой переход (без содержимого)
        transition_long_type(longjob_abstract_type &longjob, threadenv_type &env):
            transition_compound_type(content_type()), m_penv(&env)
        {
            // размещение данных длительного перехода
            std::pair<int, jobdata_type *> p = env.allocate_longjob(longjob);
            m_id = p.first;
            // формирование содержимого составного перехода
            jobdata_type &jobdata = *p.second;
            transition_compound_type::content_type content;
            content.add_place(jobdata.started);
            content.add_place(jobdata.stopped);
            content.add_transition(jobdata.stop);
            content.add_arc(jobdata.started, jobdata.stop);
            content.add_arc(jobdata.stop, jobdata.stopped);
            content.add_token(jobdata.started);
            // замена текущего содержимого составного перехода
            transition_compound_type::operator =(
                transition_compound_type(content));
        }
    };

private:
    // структуры данных длительных переходов
    // в векторе хранить нельзя, поскольку тогда при добавлении
    // нового элемента становятся невалидными указатели на старые
    std::deque<jobdata_type> m_alljobdata;

    void initialize_longjob(int id)
    {
        // выполнение работы
        m_alljobdata[id].param.pjob->run();
    }
    void finalize_longjob(int id)
    {}

    // размещение структуры данных длительного перехода
    // возвращает присвоенный номер и указатель на структуру
    std::pair<int, jobdata_type *> allocate_longjob(
        longjob_abstract_type &longjob)
    {
        // номер создаваемого длительного перехода
        int id = m_alljobdata.size();

        // заполним структуру параметров
        threadparam_type param = {};
        param.pjob = &longjob;
        // разместим данные длительного перехода
        m_alljobdata.push_back(jobdata_type(id, param));
        // вернем номер перехода и указатель на его данные
        return std::make_pair(id, &m_alljobdata.back());
    }

public:
    // ожидание срабатывания перехода
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        // списки индексов переданных переходов
        // finished - переходы завершения работ, которые уже завершены
        // free - переходы, не являющиеся переходами завершения работ
        std::vector<int> finished, free;
        // проходим по всему списку переходов
        petrinet_type::enabledlist_type::const_iterator it;
        for (it = enabled.begin(); it != enabled.end(); ++it)
        {
            transition_stop_type *stop = dynamic_cast<transition_stop_type *>(*it);
            // если это не переход завершения работы, кладем индекс в свободные
            if (!stop)
                free.push_back(std::distance(enabled.begin(), it));
            else
                // иначе - в завершенные
                finished.push_back(std::distance(enabled.begin(), it));
        };
        // формируем код возврата на основе приоритетов
        int rc;
        // если есть завершившиеся работы, возвращаем
        // индекс одного из соответствующих переходов
        if (!finished.empty())
            rc = finished[random(finished.size())];
        else
            // в противном случае есть свободные переходы
            rc = free[random(free.size())];
        return rc;
    }
};

} // namespace petri
} // namespace z

#endif /* _ZPETRI_ENV_HXX_ */
