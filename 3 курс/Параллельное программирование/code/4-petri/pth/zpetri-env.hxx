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

// ����� ��������� � ������������ ������� ��������
class randomenv_type: public petrinet_type::environment_abstract_type
{
public:
    // ��������� ���������������� ����� � ��������� [0, bound)
    static
    int random(int bound)
    {
        assert(bound > 0);
        return int(rand() / (RAND_MAX + 1.0) * bound);
    }
    // ����������� ������������ �������
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        return random(enabled.size());
    }
};

// ����� ��������� � ���������� �������������� ����������
class threadenv_type: public randomenv_type
{
public:
    // ����������� ��� ������, ����������� �� ����� ����������� ��������
    class longjob_abstract_type
    {
    public:
        // ������� ���������� ������
        virtual
        void run(void) = 0;
    };

private:
    // ������� ���������� ���������� ��������
    class transition_stop_type: public transition_simple_type
    {
    private:
        // ����� ���������������� ����������� ��������
        int m_id;
    public:
        transition_stop_type(int id): m_id(id) {}
        int id(void) const { return m_id; }
    };

    // ��������� ���������� ������
    struct threadparam_type
    {
        // ��������� �� ��������������� ������
        longjob_abstract_type *pjob;
        // ���������
        threadenv_type *penv;
        // ����� � ������ ���������� ��������� ���������
        int id;
        // ������� ���������� ����������
        bool finished;
    };

    // ��������� ������, ����������� � ������ ���������� ���������
    struct jobdata_type
    {
        // ���������� ���������������� ����������� ��������
        place_type started, stopped;
        transition_stop_type stop;
        // ����������� ���������� ������ �����
        pthread_t thread;
        // ��������� ������
        threadparam_type param;
        // �����������
        jobdata_type(int id, const threadparam_type &p): stop(id), param(p) {}
    };

public:
    // ���������� �������, ��������������� ���������� ������
    class transition_long_type: public transition_compound_type
    {
    private:
        // ���������, � ������� ������ �������
        threadenv_type *m_penv;
        // ����� �������� � ������ ���������� ��������� ���������
        int m_id;

        // ������ ������������� � ����������� ���������� ������ ����������
        void on_activate(void)
        {
            m_penv->initialize_longjob(m_id);
        }
        void on_passivate(void)
        {
            m_penv->finalize_longjob(m_id);
        }

    public:
        // ����������� ����������� ��������
        // ��������� ������ �� ����������� ������ � ��������� ���������
        // ������������� ������� ������ ������� (��� �����������)
        transition_long_type(longjob_abstract_type &longjob, threadenv_type &env):
            transition_compound_type(content_type()), m_penv(&env)
        {
            // ���������� ������ ����������� ��������
            std::pair<int, jobdata_type *> p = env.allocate_longjob(longjob);
            m_id = p.first;
            // ������������ ����������� ���������� ��������
            jobdata_type &jobdata = *p.second;
            transition_compound_type::content_type content;
            content.add_place(jobdata.started);
            content.add_place(jobdata.stopped);
            content.add_transition(jobdata.stop);
            content.add_arc(jobdata.started, jobdata.stop);
            content.add_arc(jobdata.stop, jobdata.stopped);
            content.add_token(jobdata.started);
            // ������ �������� ����������� ���������� ��������
            transition_compound_type::operator =(
                transition_compound_type(content));
        }
    };

private:
    // ��������� ������ ���������� ���������
    // � ������� ������� ������, ��������� ����� ��� ����������
    // ������ �������� ���������� ����������� ��������� �� ������
    std::deque<jobdata_type> m_alljobdata;

    // ������� ������������� ��� �������� ���������� �������
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    // ������ ��������� ������������� �������
    std::vector<int> m_lastid;

    // �������� ���� �������� ������� pthread_xxx
    static
    void chkzero(int retcode) { assert(retcode == 0); }

    // ������� ������ - ��������� ����� ������� ���������� ������
    static
    void *thr_proc(void *param)
    {
        threadparam_type &p = *static_cast<threadparam_type *>(param);
        // ���������� ������
        p.pjob->run();
        // ��������� ����������
        chkzero(::pthread_mutex_lock(&p.penv->m_mutex));
        // ����������� ��������� ���������� ������
        p.finished = true;
        p.penv->m_lastid.push_back(p.id);
        // �������� ������� � ���������� ������
        chkzero(::pthread_cond_signal(&p.penv->m_cond));
        // ������ ����������
        chkzero(::pthread_mutex_unlock(&p.penv->m_mutex));
        return NULL;
    }

    // ������ ���������� ������
    void initialize_longjob(int id)
    {
        // ��������� �������� - ����� �� ��������
        m_alljobdata[id].param.finished = false;
        // �������� ������
        chkzero(::pthread_create(
            &m_alljobdata[id].thread, NULL,
            thr_proc, &m_alljobdata[id].param));
    }
    // �������������� �������� ����� ���������� ������
    void finalize_longjob(int id)
    {
        // ������������ ��������
        // � ����� ������� ����� ��� �������� ������
        chkzero(::pthread_join(m_alljobdata[id].thread, NULL));
    }

    // ���������� ��������� ������ ����������� ��������
    // ���������� ����������� ����� � ��������� �� ���������
    std::pair<int, jobdata_type *> allocate_longjob(
        longjob_abstract_type &longjob)
    {
        // ����� ������������ ����������� ��������
        int id = m_alljobdata.size();

        // �������� ��������� ���������� ������
        threadparam_type param = {};
        param.pjob = &longjob;
        param.penv = this;
        param.id = id;
        // ��������� ������ ����������� ��������
        m_alljobdata.push_back(jobdata_type(id, param));
        // ������ ����� �������� � ��������� �� ��� ������
        return std::make_pair(id, &m_alljobdata.back());
    }

public:
    // ������ � �������� ��������� ��������� � ������������
    // ������� ������������� mutex � cond
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

    // �������� ������������ ��������
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        chkzero(::pthread_mutex_lock(&m_mutex));
        // ������ �������� ���������� ���������
        // finished - �������� ���������� �����, ������� ��� ���������
        // free - ��������, �� ���������� ���������� ���������� �����
        std::vector<int> finished, free;
        // ����������� ������� ���������� ��������� �� �������
        // ��������� ���������� �����, ������� ��� �� ���������
        std::map<int, int> busybyid;
        // �������� �� ����� ������ ���������
        petrinet_type::enabledlist_type::const_iterator it;
        for (it = enabled.begin(); it != enabled.end(); ++it)
        {
            transition_stop_type *stop = dynamic_cast<transition_stop_type *>(*it);
            // ���� ��� �� ������� ���������� ������, ������ ������ � ���������
            if (!stop)
                free.push_back(std::distance(enabled.begin(), it));
            else
            {
                // ����� ���������, ���������� �� �����
                if (m_alljobdata[stop->id()].param.finished)
                    finished.push_back(std::distance(enabled.begin(), it));
                else
                    // ���� �� ����������, ��������� ���������� ��� ��������
                    busybyid[stop->id()] = std::distance(enabled.begin(), it);
            };
        };
        // ��������� ��� �������� �� ������ �����������
        int rc;
        // ���� ���� ������������� ������, ����������
        // ������ ������ �� ��������������� ���������
        if (!finished.empty())
            rc = finished[random(finished.size())];
        // ���� ���� ��������� ��������, ������ ������ ������ �� ���
        else if (!free.empty())
            rc = free[random(free.size())];
        else
        {
            // � ��������� ������� �������� ���������� ������ �� �������
            m_lastid.clear();
            while (m_lastid.empty())
                chkzero(::pthread_cond_wait(&m_cond, &m_mutex));
            // � ������ ������ ���������������� ��������
            rc = busybyid[m_lastid[random(m_lastid.size())]];
        };
        chkzero(::pthread_mutex_unlock(&m_mutex));
        return rc;
    }
};

} // namespace petri
} // namespace z

#endif /* _ZPETRI_ENV_HXX_ */
