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
#include <pthread.h>
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

// класс окружения с поддержкой многопоточного выполнения
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

    // структура параметров потока
    struct threadparam_type
    {
        // указатель на соответствующую работу
        longjob_abstract_type *pjob;
        // окружение
        threadenv_type *penv;
        // номер в списке длительных переходов окружения
        int id;
        // признак завершения выполнения
        bool finished;
    };

    // структура данных, связываемая с каждым длительным переходом
    struct jobdata_type
    {
        // содержимое соответствующего длительного перехода
        place_type started, stopped;
        transition_stop_type stop;
        // выполняющий длительную работу поток
        pthread_t thread;
        // параметры потока
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

    // объекты синхронизации для ожидания завершения потоков
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    // номера последних завершившихся потоков
    std::vector<int> m_lastid;

    // проверка кода возврата функций pthread_xxx
    static
    void chkzero(int retcode) { assert(retcode == 0); }

    // функция потока - выполняет вызов функции выполнения работы
    static
    void *thr_proc(void *param)
    {
        threadparam_type &p = *static_cast<threadparam_type *>(param);
        // выполнение работы
        p.pjob->run();
        // установка блокировки
        chkzero(::pthread_mutex_lock(&p.penv->m_mutex));
        // модификация признаков завершения потока
        p.finished = true;
        p.penv->m_lastid.push_back(p.id);
        // отправка сигнала о завершении потока
        chkzero(::pthread_cond_signal(&p.penv->m_cond));
        // снятие блокировки
        chkzero(::pthread_mutex_unlock(&p.penv->m_mutex));
        return NULL;
    }

    // запуск выполнения работы
    void initialize_longjob(int id)
    {
        // установка признака - поток не завершен
        m_alljobdata[id].param.finished = false;
        // создание потока
        chkzero(::pthread_create(
            &m_alljobdata[id].thread, NULL,
            thr_proc, &m_alljobdata[id].param));
    }
    // заключительные действия после выполнения работы
    void finalize_longjob(int id)
    {
        // освобождение ресурсов
        // к этому моменту поток уже закончил работу
        chkzero(::pthread_join(m_alljobdata[id].thread, NULL));
    }

    // размещение структуры данных длительного перехода
    // возвращает присвоенный номер и указатель на структуру
    std::pair<int, jobdata_type *> allocate_longjob(
        longjob_abstract_type &longjob)
    {
        // номер создаваемого длительного перехода
        int id = m_alljobdata.size();

        // заполним структуру параметров потока
        threadparam_type param = {};
        param.pjob = &longjob;
        param.penv = this;
        param.id = id;
        // разместим данные длительного перехода
        m_alljobdata.push_back(jobdata_type(id, param));
        // вернем номер перехода и указатель на его данные
        return std::make_pair(id, &m_alljobdata.back());
    }

public:
    // вместе с объектом окружения создаются и уничтожаются
    // объекты синхронизации mutex и cond
    threadenv_type(void)
    {
        chkzero(::pthread_mutex_init(&m_mutex, NULL));
        chkzero(::pthread_cond_init(&m_cond, NULL));
    }
    ~threadenv_type(void)
    {
        chkzero(::pthread_cond_destroy(&m_cond));
        chkzero(::pthread_mutex_destroy(&m_mutex));
    }

    // ожидание срабатывания перехода
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        chkzero(::pthread_mutex_lock(&m_mutex));
        // списки индексов переданных переходов
        // finished - переходы завершения работ, которые уже завершены
        // free - переходы, не являющиеся переходами завершения работ
        std::vector<int> finished, free;
        // отображение номеров длительных переходов на индексы
        // переходов завершения работ, которые еще не завершены
        std::map<int, int> busybyid;
        // проходим по всему списку переходов
        petrinet_type::enabledlist_type::const_iterator it;
        for (it = enabled.begin(); it != enabled.end(); ++it)
        {
            transition_stop_type *stop = dynamic_cast<transition_stop_type *>(*it);
            // если это не переход завершения работы, кладем индекс в свободные
            if (!stop)
                free.push_back(std::distance(enabled.begin(), it));
            else
            {
                // иначе проверяем, завершился ли поток
                if (m_alljobdata[stop->id()].param.finished)
                    finished.push_back(std::distance(enabled.begin(), it));
                else
                    // если не завершился, добавляем информацию для ожидания
                    busybyid[stop->id()] = std::distance(enabled.begin(), it);
            };
        };
        // формируем код возврата на основе приоритетов
        int rc;
        // если есть завершившиеся работы, возвращаем
        // индекс одного из соответствующих переходов
        if (!finished.empty())
            rc = finished[random(finished.size())];
        // если есть свободные переходы, вернем индекс одного из них
        else if (!free.empty())
            rc = free[random(free.size())];
        else
        {
            // в остальных случаях дождемся завершения одного из потоков
            m_lastid.clear();
            while (m_lastid.empty())
                chkzero(::pthread_cond_wait(&m_cond, &m_mutex));
            // и вернем индекс соответствующего перехода
            rc = busybyid[m_lastid[random(m_lastid.size())]];
        };
        chkzero(::pthread_mutex_unlock(&m_mutex));
        return rc;
    }
};

} // namespace petri
} // namespace z

#endif /* _ZPETRI_ENV_HXX_ */
