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


#ifndef _ZJOBS_HXX_
#define _ZJOBS_HXX_

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <cassert>

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>

namespace z {
namespace jobs {

// ����������� ��������� ������
class job_abstract_type
{
public:
    // ���������� ������
    // ��������� �������� ������ � ����� ����������
    // ����������� ������ ����� ����������� �������
    virtual
    void run(void) = 0;
};

// �������� �����
class jobcomplex_type: public job_abstract_type
{
private:
    // ������ ����� � ������� ����������
    typedef std::vector<job_abstract_type *> joblist_type;
    // ����� ������� �����
    typedef std::vector<int> jobnums_type;
    // ������� ������������ ����� �� �������
    typedef std::vector<jobnums_type> deplist_type;
    // ������ ����� � ������-������������ �����
    typedef std::vector<jobnums_type> multilevel_type;

    // ====================================
    // �������� ����������� ��������� �����
    // ====================================

    // ��������� ����� � ������������ �� �� ������
    typedef std::map<job_abstract_type *, int> jobs_type;
    jobs_type m_jobs;
    // ��������� ������������ �����
    typedef std::set<std::pair<int, int> > deps_type;
    deps_type m_deps;

private:
    // ������� ��������� ������� ����� � ������������
    joblist_type get_joblist(void) const
    {
        // ��������� ������ �����
        joblist_type joblist(m_jobs.size());
        jobs_type::const_iterator it;
        for (it = m_jobs.begin(); it != m_jobs.end(); ++it)
            joblist[it->second] = it->first;
        return joblist;
    }
    deplist_type get_deplist(void) const
    {
        // ������ ������� ������������ �����
        deplist_type deplist(m_jobs.size());
        deps_type::const_iterator it;
        for (it = m_deps.begin(); it != m_deps.end(); ++it)
            deplist[it->first].push_back(it->second);
        return deplist;
    }

    // ���������� ������-������������ ���������
    // �� ������ ������������ ����� ����� �����
    static
    multilevel_type build(const deplist_type &deplist)
    {
        enum { NOLEVEL = -1 };
        // ������ ������ ���� �����, ��������������� �� �� �������
        typedef std::vector<int> levnums_type;
        levnums_type levnums(deplist.size(), NOLEVEL);

        // ����� ������� �����, ����� ������� ��� �� ����������
        jobnums_type nondetermined;
        levnums_type::iterator lt;
        for (lt = levnums.begin(); lt != levnums.end(); ++lt)
            nondetermined.push_back(std::distance(levnums.begin(), lt));

        // ���� ����������� ������ �����
        while (!nondetermined.empty())
        {
            jobnums_type determined;
            // ��������� �� ���� �������������� �������
            jobnums_type::iterator it;
            for (it = nondetermined.begin(); it != nondetermined.end(); ++it)
            {
                // ���� ������������ ��� - ������� ����
                int lev = 0;
                // ��������� �� ���� ������������, ���� ����
                deplist_type::value_type::const_iterator jt;
                for (jt = deplist[*it].begin(); jt != deplist[*it].end(); ++jt)
                {
                    // ���� ���� ����������� ��� �� ���������
                    if (levnums[*jt] == NOLEVEL)
                    {
                        // ������� ������ ����� �������� ��������������
                        lev = NOLEVEL;
                        break;
                    }
                    else
                        // ����� ���� �� ������� ������ �������������
                        lev = std::max(lev, levnums[*jt] + 1);
                };
                // ���� ���� ����������, ��������� � ������������
                if (lev != NOLEVEL)
                {
                    determined.push_back(*it);
                    levnums[*it] = lev;
                };
            };

            // ���� ������ �� ������� ����������,
            // ������, � ��� ����������� �����������
            assert(!determined.empty());

            // ������� �� �������������� ��, ��� ����������
            jobnums_type diff;
            std::set_difference(
                nondetermined.begin(), nondetermined.end(),
                determined.begin(), determined.end(),
                std::back_inserter(diff));
            nondetermined.swap(diff);
        };

        // ������� ������ ������-������������ ���������
        int height = *std::max_element(levnums.begin(), levnums.end()) + 1;

        // �������� ������-������������ ���������
        // �� ���� ������� ����� �� ������ ����������� ������
        multilevel_type multilevel(height);
        for (lt = levnums.begin(); lt != levnums.end(); ++lt)
            multilevel[*lt].push_back(std::distance(levnums.begin(), lt));

        return multilevel;
    }

private:
    // ����� ������� �������
    typedef std::vector<HANDLE> handles_type;
    // ��������� ���������� ��� ������� ������
    struct threadparam_type
    {
        // ��������� �� ����� ������� ���� �������
        const handles_type *phdls;
        // ��������� �� ����������� ������� ������
        const jobnums_type *pdeps;
        // ��������� �� ������ ��� ����������
        job_abstract_type *pjob;
    };

    // �������� ���������� ������ ������ �� ����������� ������
    static
    int wait_any(const handles_type &hdls)
    {
        assert(!hdls.empty());
        DWORD dw = ::WaitForMultipleObjects(
            hdls.size(), &hdls.front(), FALSE, INFINITE);
        assert(dw >= WAIT_OBJECT_0 && dw < WAIT_OBJECT_0 + hdls.size());
        return dw - WAIT_OBJECT_0;
    }

    // �������� ���������� ���� ������� �� ����������� ������
    static
    void wait_all(const handles_type &hdls)
    {
        if (hdls.size())
        {
            DWORD dw = ::WaitForMultipleObjects(
                hdls.size(), &hdls.front(), TRUE, INFINITE);
            assert(dw >= WAIT_OBJECT_0 && dw < WAIT_OBJECT_0 + hdls.size());
        };
    }

    // �������, ����������� ��������
    static
    DWORD WINAPI thr_proc(LPVOID param)
    {
        threadparam_type &thrp = *static_cast<threadparam_type *>(param);

        // ������������ ������ ������� ��� ��������
        // �� ������ ������������ ������� ������
        handles_type hdls;
        jobnums_type::const_iterator it;
        for (it = thrp.pdeps->begin(); it != thrp.pdeps->end(); ++it)
            hdls.push_back((*thrp.phdls)[*it]);

        // �������� ���������� �����
        wait_all(hdls);

        // ���������� ������� ������
        thrp.pjob->run();

        return 0;
    }

public:

    // ���������� ������ � ��������
    void add_job(job_abstract_type &job)
    {
        // ����������� ������ �� ������ ���� � ������
        assert(m_jobs.find(&job) == m_jobs.end());
        int idx = m_jobs.size();
        m_jobs.insert(jobs_type::value_type(&job, idx));
    }

    // ���������� ����������� ����� ������ �� ������
    void add_dependence(
        job_abstract_type &dst,
        job_abstract_type &src)
    {
        // ������ ������ ���� ����������
        assert(&src != &dst);
        // ��� ������ ��� ������ ���� � ������
        assert(m_jobs.find(&dst) != m_jobs.end());
        assert(m_jobs.find(&src) != m_jobs.end());
        m_deps.insert(deps_type::value_type(m_jobs[&dst], m_jobs[&src]));
    }

    // ���������� ����� ��������� �����
    void run(void)
    {
        // ������� ������ ���������� ���������� �����
        joblist_type joblist = get_joblist();
        // ������� ������ ������������
        deplist_type deplist = get_deplist();
        // �������� ������-������������ ��������� ������� �����
        multilevel_type multilevel = build(deplist);

        // ������ ������� ���� �������
        handles_type allhdl(joblist.size());
        // ������ ���� �������� ����������
        std::vector<threadparam_type> thrparam(joblist.size());

        multilevel_type::iterator it;
        // �������� �� ������� ������ ���� �����
        for (it = multilevel.begin(); it != multilevel.end(); ++it)
        {
            handles_type lasthdl;
            // �������� ��� ������ �������� �����
            int width = it->size();
            for (int i = 0; i < width; ++i)
            {
                // ����� ������� ������
                int idx = (*it)[i];
                // ��������� ��������� ���������� ������
                thrparam[idx].phdls = &allhdl;
                thrparam[idx].pdeps = &deplist[idx];
                thrparam[idx].pjob = joblist[idx];

                // �������� ������
                DWORD dwId;
                allhdl[idx] = ::CreateThread(
                    NULL, 0,
                    thr_proc, &thrparam[idx],
                    0, &dwId);
                // ��������� � ������ ������� �������� �����
                lasthdl.push_back(allhdl[idx]);
            };
            // ������ ��� ���������� � ���������� �����,
            // �������� �������, ���� ����������
            // ���� �� ���� ����� �������� �����
            wait_any(lasthdl);
        };

        // ���� ���������� ���� ���������� �������
        wait_all(allhdl);
        // � ��������� ������
        handles_type::iterator ht;
        for (ht = allhdl.begin(); ht != allhdl.end(); ++ht)
            ::CloseHandle(*ht);
    }
};

} // namespace jobs
} // namespace z

#endif /* _ZJOBS_HXX_ */
