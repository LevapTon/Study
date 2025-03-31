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


#ifndef _ZJOBS_HXX_
#define _ZJOBS_HXX_

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace z {
namespace jobs {

// абстрактный интерфейс работы
class job_abstract_type
{
public:
    // выполнение работы
    // получение исходных данных и вывод результата
    // выполняются внутри через разделяемые ресурсы
    virtual
    void run(void) = 0;
};

// комплекс работ
class jobcomplex_type: public job_abstract_type
{
private:
    // список работ в порядке добавления
    typedef std::vector<job_abstract_type *> joblist_type;
    // набор номеров работ
    typedef std::vector<int> jobnums_type;
    // таблица зависимостей работ по номерам
    typedef std::vector<jobnums_type> deplist_type;
    // номера работ в ярусно-параллельной форме
    typedef std::vector<jobnums_type> multilevel_type;

    // ====================================
    // описание содержимого комплекса работ
    // ====================================

    // множество работ с отображением на их номера
    typedef std::map<job_abstract_type *, int> jobs_type;
    jobs_type m_jobs;
    // множество зависимостей работ
    typedef std::set<std::pair<int, int> > deps_type;
    deps_type m_deps;

private:
    // функции получения списков работ и зависимостей
    joblist_type get_joblist(void) const
    {
        // заполняем список работ
        joblist_type joblist(m_jobs.size());
        jobs_type::const_iterator it;
        for (it = m_jobs.begin(); it != m_jobs.end(); ++it)
            joblist[it->second] = it->first;
        return joblist;
    }
    deplist_type get_deplist(void) const
    {
        // строим таблицу зависимостей работ
        deplist_type deplist(m_jobs.size());
        deps_type::const_iterator it;
        for (it = m_deps.begin(); it != m_deps.end(); ++it)
            deplist[it->first].push_back(it->second);
        return deplist;
    }

    // построение ярусно-параллельной структуры
    // на основе зависимостей работ между собой
    static
    multilevel_type build(const deplist_type &deplist)
    {
        enum { NOLEVEL = -1 };
        // номера ярусов всех работ, индексированные по их номерам
        typedef std::vector<int> levnums_type;
        levnums_type levnums(deplist.size(), NOLEVEL);

        // набор номеров работ, ярусы которых еще не определены
        jobnums_type nondetermined;
        levnums_type::iterator lt;
        for (lt = levnums.begin(); lt != levnums.end(); ++lt)
            nondetermined.push_back(std::distance(levnums.begin(), lt));

        // цикл определения ярусов работ
        while (!nondetermined.empty())
        {
            jobnums_type determined;
            // пройдемся по всем неопределенным работам
            jobnums_type::iterator it;
            for (it = nondetermined.begin(); it != nondetermined.end(); ++it)
            {
                // если зависимостей нет - нулевой ярус
                int lev = 0;
                // пройдемся по всем зависимостям, если есть
                deplist_type::value_type::const_iterator jt;
                for (jt = deplist[*it].begin(); jt != deplist[*it].end(); ++jt)
                {
                    // если ярус зависимости еще не определен
                    if (levnums[*jt] == NOLEVEL)
                    {
                        // текущая работа также остается неопределенной
                        lev = NOLEVEL;
                        break;
                    }
                    else
                        // иначе ярус на единицу больше максимального
                        lev = std::max(lev, levnums[*jt] + 1);
                };
                // если ярус определили, добавляем в определенные
                if (lev != NOLEVEL)
                {
                    determined.push_back(*it);
                    levnums[*it] = lev;
                };
            };

            // если ничего не удалось определить,
            // видимо, у нас циклические зависимости
            assert(!determined.empty());

            // выкинем из неопределенных те, что определили
            jobnums_type diff;
            std::set_difference(
                nondetermined.begin(), nondetermined.end(),
                determined.begin(), determined.end(),
                std::back_inserter(diff));
            nondetermined.swap(diff);
        };

        // получим высоту ярусно-параллельной структуры
        int height = *std::max_element(levnums.begin(), levnums.end()) + 1;

        // построим ярусно-параллельную структуру
        // из всех номеров работ на основе вычисленных ярусов
        multilevel_type multilevel(height);
        for (lt = levnums.begin(); lt != levnums.end(); ++lt)
            multilevel[*lt].push_back(std::distance(levnums.begin(), lt));

        return multilevel;
    }

public:

    // добавление работы в комплекс
    void add_job(job_abstract_type &job)
    {
        // добавляемой работы не должно быть в списке
        assert(m_jobs.find(&job) == m_jobs.end());
        int idx = m_jobs.size();
        m_jobs.insert(jobs_type::value_type(&job, idx));
    }

    // добавление зависимости одной работы от другой
    void add_dependence(
        job_abstract_type &dst,
        job_abstract_type &src)
    {
        // работы должны быть различными
        assert(&src != &dst);
        // обе работы уже должны быть в списке
        assert(m_jobs.find(&dst) != m_jobs.end());
        assert(m_jobs.find(&src) != m_jobs.end());
        m_deps.insert(deps_type::value_type(m_jobs[&dst], m_jobs[&src]));
    }

    // выполнение всего комплекса работ
    void run(void)
    {
        // получим список подлежащих выполнению работ
        joblist_type joblist = get_joblist();
        // построим ярусно-параллельную структуру номеров работ
        multilevel_type multilevel = build(get_deplist());

        multilevel_type::iterator it;
        // выполним по очереди каждый ярус работ
        for (it = multilevel.begin(); it != multilevel.end(); ++it)
        {
            int width = it->size();
            #pragma omp parallel for
            for (int i = 0; i < width; ++i)
                joblist[(*it)[i]]->run();
        };
    }
};

} // namespace jobs
} // namespace z

#endif /* _ZJOBS_HXX_ */
