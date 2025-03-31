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


#ifndef _ZACTOR_HXX_
#define _ZACTOR_HXX_

#include <map>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <string>
#include <typeinfo>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

#include "../../common/synprintf.hxx"

namespace z {
namespace actor {


// ��������� ���������������� ����� � ��������� [0, bound)
inline int random(int bound)
{
    assert(bound > 0);
    return int(rand() / (RAND_MAX + 1.0) * bound);
}

class arbiter_type;
class factory_type;
class scheduler_type;

// �������� ����� ������
typedef int address_type;

// ������� ����� ������
class actor_type
{
private:
    // === ���������� ���� � ����� ������� ��� ���� ������� ===
    friend class factory_type;
    friend class arbiter_type;
    friend class scheduler_type;

    // ������������� ��������� ������
    typedef std::string defid_type;
    // ������������� ������� ���������
    typedef std::string patid_type;
    // ��������� �������� ������ ��� ���������,
    // ��������������� � ������� ������
    typedef std::vector<char> chunk_type;

    // ��������� ��������� ������
    struct behaviour_type
    {
        // ������������� ���������
        defid_type defid;
        // ��������� �������� ������
        chunk_type chunk;
    };

    // ��������� ���������
    struct task_type
    {
        // ����� ����������
        address_type address;
        // ������������� �������
        patid_type patid;
        // ��������� ���������
        chunk_type chunk;
    };

    // �������������� ��������� ���������� � ������ ������ � �������
    template <typename param_type>
    static
    chunk_type param2chunk(const param_type &p)
    {
        typedef chunk_type::value_type data_type;
        assert(sizeof(param_type) % sizeof(data_type) == 0);
        const data_type &c = reinterpret_cast<const data_type &>(p);
        int size = sizeof(param_type) / sizeof(data_type);
        // ������ ������������ �������� ��� ������
        return chunk_type(&c, &c + size);
    }
    template <typename param_type>
    static
    const param_type & chunk2param(const chunk_type &chunk)
    {
        return reinterpret_cast<const param_type &>(chunk.front());
    }

    // ��������� �������������� ��������� �� �����
    template <typename someactor_type, typename init_type>
    static
    defid_type def_id(void)
    {
        // �������� ������������ ���������� ������������
        assert(1 ? 1 : (delete new someactor_type(init_type()), 0));
        // ��������� ������������� �����������-��������
        return
            defid_type(typeid(someactor_type).name()) +
            "|" +
            defid_type(typeid(init_type).name());
    }

    // ��������� �������������� ������� ��������� �� ��� ����
    template <typename pattern_type>
    static
    patid_type pat_id(void)
    {
        return patid_type(typeid(pattern_type).name());
    }

public:
    // === ����, ������������ ���������� ����� ===

    // "������" ��� ���������� �������������, ������������
    // ��� ������� ������ �������� ������ ��� ����������
    class empty_type {};
    // �������������� ��� ���������, ������������ ��� ���������
    // ���������, �� ��������������� �� ������ �������
    class unknown_type
    {
        friend class actor_type;
        task_type task;
    };

private:
    // ��������� �� �������-���������
    typedef void (*activator_type)(actor_type *, const task_type &);
    // ������� �����������
    typedef std::map<patid_type, activator_type> activatortable_type;
    activatortable_type m_acttable;

    // �������� ������ � ������� � ������������
    scheduler_type *m_sched;
    arbiter_type *m_arbiter;

    // ������ ���������� ��������� ����������� ���������
    template <
        typename someactor_type,
        typename pattern_type,
        void (someactor_type::*action)(const pattern_type &)>
    static
    void activate(actor_type *thisp, const task_type &task)
    {
        someactor_type *actor = static_cast<someactor_type *>(thisp);
        (actor->*action)(chunk2param<pattern_type>(task.chunk));
    }
    // ������ ���������� ��������� "������" ���������
    template <
        typename someactor_type,
        void (someactor_type::*action)(const unknown_type &)>
    static
    void activate(actor_type *thisp, const task_type &task)
    {
        someactor_type *actor = static_cast<someactor_type *>(thisp);
        unknown_type unknown;
        unknown.task = task;
        (actor->*action)(unknown);
    }

    // ������� ��������������� ������� ����������
    // �� ������ ���������� �����
    template <typename someactor_type, typename pattern_type>
    activator_type get_activator(const someactor_type *, const pattern_type *)
    {
        return &actor_type::template
            activate<someactor_type, pattern_type, &someactor_type::action>;
    }
    template <typename someactor_type>
    activator_type get_activator(const someactor_type *, const unknown_type *)
    {
        return &actor_type::template
            activate<someactor_type, &someactor_type::action>;
    }

    // ���� ���� ������� ������� ����� ������� � ������������
    inline address_type raw_create(const behaviour_type &behaviour) const;
    inline void raw_become(const behaviour_type &behaviour) const;
    inline void raw_send(const task_type &task) const;
    inline address_type raw_self(void) const;

    // === ��������� ��� arbiter_type � scheduler_type ===
    // �������� ������ ��� ���������� ������
    void bind(scheduler_type *sched, arbiter_type *arbiter)
    {
        m_sched = sched;
        m_arbiter = arbiter;
    }
    // �������� ������� ���������� �� ���������
    bool match(const task_type &task) const
    {
        return
            m_acttable.find(task.patid) != m_acttable.end() ||
            m_acttable.find(pat_id<unknown_type>()) != m_acttable.end();
    }
    // ������ ���������� �� ���������
    void apply(const task_type &task)
    {
        assert(task.address == self());
        activatortable_type::iterator it;
        // ������ ���������� �� �������
        it = m_acttable.find(task.patid);
        // ���� ���, ������ ���������� �� ���������
        if (it == m_acttable.end())
            it = m_acttable.find(pat_id<unknown_type>());
        // ���������� ������ ���� ������, �������� ���
        assert(it != m_acttable.end());
        it->second(this, task);
    }

protected:
    // === ��������� ��� ����������� ���� ===

    // �����������
    actor_type(void):
        m_sched(NULL), m_arbiter(NULL)
    {}

    // ���������� ����������� ��������� ��������� �������
    template <typename someactor_type, typename pattern_type>
    void add_action(void)
    {
        patid_type patid = pat_id<pattern_type>();
        // ������ ����������� ���� �� ������ ����
        assert(m_acttable.find(patid) == m_acttable.end());
        // ������� ���������� �� ������ ������� �����
        m_acttable[patid] = get_activator(
            static_cast<const someactor_type *>(NULL),
            static_cast<const pattern_type *>(NULL));
    }

    // ���������� �������� become � �����������
    template <typename someactor_type, typename init_type>
    void become(const init_type &init) const
    {
        assert(m_arbiter != NULL);
        behaviour_type behaviour =
        {
            def_id<someactor_type, init_type>(),
            param2chunk(init)
        };
        raw_become(behaviour);
    }
    // �� �� ��� ����������
    template <typename someactor_type>
    void become(void) const
    {
        become<someactor_type>(empty_type());
    }

    // ��������� ������ �������� ������
    address_type self(void) const
    {
        return raw_self();
    }

public:
    // ���������� �������� create � �����������
    template <typename someactor_type, typename init_type>
    address_type create(const init_type &init) const
    {
        behaviour_type behaviour =
        {
            def_id<someactor_type, init_type>(),
            param2chunk(init)
        };
        return raw_create(behaviour);
    }
    // �� �� ��� ����������
    template <typename someactor_type>
    address_type create(void) const
    {
        return create<someactor_type>(empty_type());
    }

    // ���������� �������� ���������� ���������
    template <typename pattern_type>
    void send(const address_type &address, const pattern_type &msg) const
    {
        task_type task =
        {
            address,
            pat_id<pattern_type>(),
            param2chunk(msg)
        };
        raw_send(task);
    }
    // ��������������� "�������" ���������
    void send(const address_type &address, const unknown_type &msg) const
    {
        task_type task = msg.task;
        task.address = address;
        raw_send(task);
    }
};

// ����� ������� �������
class factory_type
{
private:
    typedef actor_type::defid_type defid_type;
    typedef actor_type::chunk_type chunk_type;
    typedef actor_type::behaviour_type behaviour_type;

    // ��� ��������� �� ������� �������� ������
    typedef actor_type *(*creator_type)(const chunk_type &chunk);
    // ��� ��������� �� ������� ����������� ������
    typedef void (*destroyer_type)(actor_type *actor);

    typedef std::map<defid_type, creator_type> creatortable_type;
    typedef std::map<defid_type, destroyer_type> destroyertable_type;
    creatortable_type m_ctortable;
    destroyertable_type m_dtortable;

    // ������� �������� � ����������� ������� ��������� ����
    template <typename someactor_type, typename init_type>
    static
    actor_type *construct(const chunk_type &chunk)
    {
        return new someactor_type(actor_type::chunk2param<init_type>(chunk));
    }
    template <typename someactor_type>
    static
    void destruct(actor_type *actor)
    {
        delete static_cast<someactor_type *>(actor);
    }

    // === ��������� ��� arbiter_type ===
    friend class arbiter_type;
    // �������� ������
    actor_type *create_actor(const behaviour_type &behaviour) const
    {
        // ���� ������� �������� ������ � �������� ����������
        creatortable_type::const_iterator it;
        it = m_ctortable.find(behaviour.defid);
        // ��������, ��� ��������� ���� ������� � �������
        assert(it != m_ctortable.end());
        // ������� ������ � ��������� �����������
        actor_type *actor = (it->second)(behaviour.chunk);
        return actor;
    }
    void destroy_actor(
        const behaviour_type &behaviour,
        actor_type *actor) const
    {
        // ���� �� ��������� ��������� ������� �����������
        destroyertable_type::const_iterator it;
        it = m_dtortable.find(behaviour.defid);
        // ��������, ��� ��������� ���� ������� � �������
        assert(it != m_dtortable.end());
        // � ���������� ������
        (it->second)(actor);
    }

public:
    // === ��������� ��� ����������� ���� ===

    // ���������� ��������� � �������
    template <typename someactor_type, typename init_type>
    void add_definition(void)
    {
        // ������������ ������� �������� � ����������� ������
        // � ���������� �� � ������������ � ����������
        defid_type defid = actor_type::def_id<someactor_type, init_type>();
        m_ctortable[defid] = &factory_type::template
            construct<someactor_type, init_type>;
        m_dtortable[defid] = &factory_type::template
            destruct<someactor_type>;
    }
};

// ������, ����������� ���� ���������� �� �������������
class arbiter_type
{
private:
    typedef actor_type::behaviour_type behaviour_type;
    typedef actor_type::task_type task_type;
    typedef std::list<task_type> mailbox_type;

    // ������� ��� �������� � ����������� �������
    const factory_type &m_factory;
    // ������ �����
    address_type m_self;
    // ����������� �������� �������
    scheduler_type *m_sched;
    // ������ ���������� ���������
    std::list<behaviour_type> m_behaviour;
    // ������� �����
    actor_type *m_actor;
    // �������� ����
    mailbox_type m_mailbox;

    // === ��������� ��� actor_type ===
    friend class actor_type;
    // ������� ������ ���������
    void new_behaviour(const behaviour_type &behaviour)
    {
        // � ������ ������ ������ ���� ����� ���� �������
        assert(m_behaviour.size() == 1);
        m_behaviour.push_back(behaviour);
    }
    // ��������� ������ ��������� �����
    const address_type & address(void) const
    {
        return m_self;
    }

    // === ��������� ��� scheduler_type ===
    friend class scheduler_type;
    // �����������
    arbiter_type(
        const factory_type &factory,
        const address_type &self,
        const behaviour_type &behaviour,
        scheduler_type *sched):
        m_factory(factory), m_self(self), m_sched(sched)
    {
        // �������� ��������� ���������
        m_behaviour.push_back(behaviour);
        // �������� ������
        m_actor = m_factory.create_actor(m_behaviour.front());
        // �������� � ������� � ������������
        m_actor->bind(m_sched, this);
    }
    // ����������
    ~arbiter_type(void)
    {
        m_factory.destroy_actor(m_behaviour.front(), m_actor);
    }
    // ��������� ��������� � �������� ����
    void deliver(const task_type &task)
    {
        m_mailbox.push_back(task);
    }
    // �������� ��������� ����� �� �������
    bool empty(void) const
    {
        return m_mailbox.empty();
    }
    // ���������� ���������� ��������� �� �����
    bool retrieve(task_type &task)
    {
        bool rc;
        if ((rc = !m_mailbox.empty()))
        {
            // �������� �������� ���������������������
            mailbox_type::iterator it = m_mailbox.begin();
            advance(it, random(m_mailbox.size()));
            // �������� ������������ ��������� ��������
            if ((rc = m_actor->match(*it)))
            {
                // ���������� � �������� ��������� �� �����
                task = *it;
                m_mailbox.erase(it);
            };
        };
        return rc;
    }
    // ��������� ���������
    void process(const task_type &task)
    {
        // ��������� ���������
        m_actor->apply(task);
        // ���� ���� ������ ����� ���������
        if (m_behaviour.size() > 1)
        {
            // ���������� ������
            m_factory.destroy_actor(m_behaviour.front(), m_actor);
            // ������� ������� ���������
            while (m_behaviour.size() > 1)
                m_behaviour.pop_front();
            // ������� � ����������� ������ ������
            m_actor = m_factory.create_actor(m_behaviour.front());
            m_actor->bind(m_sched, this);
        };
    }
};

// ����� ������������
class scheduler_type
{
private:
    typedef actor_type::behaviour_type behaviour_type;
    typedef actor_type::task_type task_type;

    typedef std::vector<arbiter_type *> arbiterlist_type;
    typedef std::list<std::pair<address_type, behaviour_type> > actorqueue_type;
    typedef std::list<task_type> taskqueue_type;

    // ������-�����, "����� �������" ��� ������� �������
    actor_type m_origin;
    // ������� �������� �� �������� �������
    actorqueue_type m_actorqueue;
    // ������� �������� �� �������� ���������
    taskqueue_type m_taskqueue;
    // ������ ��������� ��������
    arbiterlist_type m_arbiterlist;

    struct thrparam_type
    {
        scheduler_type *sched;
        const factory_type *factory;
        address_type address;
        behaviour_type behaviour;
        // ������� ������������� �������
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        // ������� ���������� ������
        bool finish;
    };
    // ������� ������������� ������������
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    // ��������� ������� ��������
    std::set<address_type> m_busy;

    // �������� ���� �������� ������� pthread_xxx
    static
    void chkzero(int retcode) { assert(retcode == 0); }

    // ������� ������, � ������ �������� �������� ������
    static
    void *thr_proc(void *param)
    {
        thrparam_type &p = *static_cast<thrparam_type *>(param);
        chkzero(::pthread_mutex_init(&p.mutex, NULL));
        chkzero(::pthread_cond_init(&p.cond, NULL));

        // ������� �������
        arbiter_type *arbiter = new arbiter_type(
            *p.factory,
            p.address,
            p.behaviour,
            p.sched);

        // ��������� ��������� � �������� ������������
        chkzero(::pthread_mutex_lock(&p.sched->m_mutex));
        p.sched->m_arbiterlist[p.address] = arbiter;
        chkzero(::pthread_cond_signal(&p.sched->m_cond));
        chkzero(::pthread_mutex_unlock(&p.sched->m_mutex));

        bool finish = false;
        do
        {
            chkzero(::pthread_mutex_lock(&p.mutex));
            // �������� �������� � ���������� ���������
            task_type task;
            if (arbiter->retrieve(task))
            {
                // �� ����� ��������� ��������� ����������
                chkzero(::pthread_mutex_unlock(&p.mutex));
                arbiter->process(task);
                chkzero(::pthread_mutex_lock(&p.mutex));
            };
            // ���� ���� ����
            if (arbiter->empty())
            {
                // ��������� ���� �� ������ �������
                chkzero(::pthread_mutex_lock(&p.sched->m_mutex));
                p.sched->m_busy.erase(p.address);
                chkzero(::pthread_cond_signal(&p.sched->m_cond));
                chkzero(::pthread_mutex_unlock(&p.sched->m_mutex));
                // � ������� ���������� ��� ������� ����� ���������
                while (!p.finish && arbiter->empty())
                    chkzero(::pthread_cond_wait(&p.cond, &p.mutex));
                // � ������ ���������� ������ ������� ����������
                finish = p.finish;
            };
            chkzero(::pthread_mutex_unlock(&p.mutex));
        } while (!finish);

        // ��� ���������� ���������� �������
        delete arbiter;

        chkzero(::pthread_cond_destroy(&p.cond));
        chkzero(::pthread_mutex_destroy(&p.mutex));
        return NULL;
    }

    // === ��������� ��� actor_type ===
    friend class actor_type;
    // ���������� ������� �� �������� ������
    address_type new_actor(const behaviour_type &behaviour)
    {
        chkzero(::pthread_mutex_lock(&m_mutex));
        // ������������ ������ ������
        address_type newaddr = m_arbiterlist.size() + m_actorqueue.size();
        // ���������� �������
        m_actorqueue.push_back(
            actorqueue_type::value_type(newaddr, behaviour));
        chkzero(::pthread_cond_signal(&m_cond));
        chkzero(::pthread_mutex_unlock(&m_mutex));
        return newaddr;
    }
    // ���������� ������� �� �������� ���������
    void new_task(const task_type &task)
    {
        chkzero(::pthread_mutex_lock(&m_mutex));
        m_taskqueue.push_back(task);
        chkzero(::pthread_cond_signal(&m_cond));
        chkzero(::pthread_mutex_unlock(&m_mutex));
    }

public:
    // === ��������� ��� ����������� ���� ===

    // �����������
    scheduler_type(void)
    {
        chkzero(::pthread_mutex_init(&m_mutex, NULL));
        chkzero(::pthread_cond_init(&m_cond, NULL));
        // ������-����� �������� ������ � ������������
        // ������� � ���� ���, ��� � ��������� ������
        m_origin.bind(this, NULL);
    }
    ~scheduler_type(void)
    {
        chkzero(::pthread_cond_destroy(&m_cond));
        chkzero(::pthread_mutex_destroy(&m_mutex));
    }

    // "����� �������" ������� �������
    const actor_type & system(void) const
    {
        return m_origin;
    }

    // ������ ���� �������� ������� �������
    void evolve(const factory_type &factory)
    {
        // �������������� �������
        std::vector<pthread_t> thr;
        // ��������� ���������� �������
        std::deque<thrparam_type> thrparam;

        chkzero(::pthread_mutex_lock(&m_mutex));
        do
        {
            // ��������� �������� �� �������� �������
            while (!m_actorqueue.empty())
            {
                // ������� �� ������� ����� � ��������������� ������
                address_type newaddr = m_actorqueue.front().first;
                behaviour_type behaviour = m_actorqueue.front().second;
                m_actorqueue.pop_front();
                // ����������� ����� ��� ��������� �� �������
                assert(size_t(newaddr) == m_arbiterlist.size());
                m_arbiterlist.push_back(NULL);
                // ��������� ����������
                chkzero(::pthread_mutex_unlock(&m_mutex));

                // ����������� ����� ��� ���������� ������
                assert(size_t(newaddr) == thr.size());
                thr.push_back(pthread_t());
                assert(size_t(newaddr) == thrparam.size());
                thrparam.push_back(thrparam_type());
                // ��������� ��������� ������
                thrparam[newaddr].sched = this;
                thrparam[newaddr].factory = &factory;
                thrparam[newaddr].address = newaddr;
                thrparam[newaddr].behaviour = behaviour;
                thrparam[newaddr].finish = false;

                // ������� �����
                chkzero(::pthread_create(&thr[newaddr], NULL, thr_proc, &thrparam[newaddr]));
                // �������� ���������� � ���� �������� �������
                chkzero(::pthread_mutex_lock(&m_mutex));
                while (!m_arbiterlist[newaddr])
                    chkzero(::pthread_cond_wait(&m_cond, &m_mutex));
            };

            // ��������� �������� �� �������� ���������
            taskqueue_type skipqueue;
            while (!m_taskqueue.empty())
            {
                // ������� ���������
                task_type task = m_taskqueue.front();
                m_taskqueue.pop_front();
                // ���� ��������������� ������ ��� ������
                if (size_t(task.address) < m_arbiterlist.size())
                {
                    // �������� ��� ��������
                    arbiter_type *arbiter = m_arbiterlist[task.address];
                    thrparam_type &p = thrparam[task.address];

                    // �������� ��������� ���������� ������������
                    chkzero(::pthread_mutex_unlock(&m_mutex));
                    // ��������� ������� � ����� ������������
                    chkzero(::pthread_mutex_lock(&p.mutex));
                    chkzero(::pthread_mutex_lock(&m_mutex));

                    // �������� ��������� � �������� ����
                    arbiter->deliver(task);
                    m_busy.insert(task.address);
                    chkzero(::pthread_cond_signal(&p.cond));

                    // ��������� �������
                    chkzero(::pthread_mutex_unlock(&p.mutex));
                }
                else
                    // ���� ������ ��� �� ������, ����������� ������
                    skipqueue.push_back(task);
            };
            m_taskqueue.swap(skipqueue);

            // ������� ������������ ������� ��� ������� �������
            while (!m_busy.empty() && m_actorqueue.empty() && m_taskqueue.empty())
                chkzero(::pthread_cond_wait(&m_cond, &m_mutex));

            // �����, ���� � ������� ��� ���������
        } while (!m_busy.empty() || !m_actorqueue.empty() || !m_taskqueue.empty());
        chkzero(::pthread_mutex_unlock(&m_mutex));

        // ������� �������� ��� ���� ������� � ���������� ������
        int allnum = m_arbiterlist.size();
        for (int addr = 0; addr < allnum; ++addr)
        {
            thrparam_type &p = thrparam[addr];
            chkzero(::pthread_mutex_lock(&p.mutex));
            p.finish = true;
            chkzero(::pthread_cond_signal(&p.cond));
            chkzero(::pthread_mutex_unlock(&p.mutex));
        };
        // ���� ���������� ���� �������
        for (int addr = 0; addr < allnum; ++addr)
            chkzero(::pthread_join(thr[addr], NULL));
        m_arbiterlist.clear();
    }
};

// �� ����������� ����� ������� ������ actor_type
address_type actor_type::raw_create(const behaviour_type &behaviour) const
{
    assert(m_sched != NULL);
    return m_sched->new_actor(behaviour);
}
void actor_type::raw_send(const task_type &task) const
{
    assert(m_sched != NULL);
    m_sched->new_task(task);
}
void actor_type::raw_become(const behaviour_type &behaviour) const
{
    assert(m_arbiter != NULL);
    m_arbiter->new_behaviour(behaviour);
}
address_type actor_type::raw_self(void) const
{
    assert(m_arbiter != NULL);
    return m_arbiter->address();
}


// ����� sink
class sink_type: public actor_type
{
public:
    sink_type(const empty_type &init)
    {
        add_action<sink_type, unknown_type>();
    }
    void action(const unknown_type &msg)
    {}
};

// ����� forwarder
class forwarder_type: public actor_type
{
private:
    address_type m_address;

public:
    struct init_type
    {
        address_type address;
    };

    forwarder_type(const init_type &init):
        m_address(init.address)
    {
        add_action<forwarder_type, unknown_type>();
    }
    void action(const unknown_type &msg)
    {
        send(m_address, msg);
    }
};

// ����� print
class print_type: public actor_type
{
public:
    struct strmsg_type
    {
        enum { MAX_SIZE = 4096 };
        char str[MAX_SIZE];
    };

    print_type(const empty_type &init)
    {
        add_action<print_type, int>();
        add_action<print_type, double>();
        add_action<print_type, strmsg_type>();
    }
    void action(const int &msg)
    {
        synprintf(stdout, "print int: %d\n", msg);
    }
    void action(const double &msg)
    {
        synprintf(stdout, "print dbl: %f\n", msg);
    }
    void action(const strmsg_type &msg)
    {
        synprintf(stdout, "print str: %s\n", msg.str);
    }
};


} // namespace actor
} // namespace z

#endif /* _ZACTOR_HXX_ */
