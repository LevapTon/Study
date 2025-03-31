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

#include <mpi.h>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <typeinfo>
#include <cassert>
#include <cstdlib>
#include <ctime>

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
class dispatcher_type;

// �������� ����� ������
class address_type
{
    friend class dispatcher_type;
    int rank, num;
public:
    friend
    bool operator ==(const address_type &l, const address_type &r)
    {
        return l.rank == r.rank && l.num == r.num;
    }
};

// ������� ����� ������
class actor_type
{
private:
    // === ���������� ���� � ����� ������� ��� ���� ������� ===
    friend class factory_type;
    friend class arbiter_type;
    friend class dispatcher_type;
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

// ���������, ����������� ���� ���������� �� �������������
class dispatcher_type
{
private:
    typedef actor_type::chunk_type chunk_type;

    // ��� ������������� �������
    enum { REQ_CREATE, REQ_SEND };
    // ��������� �������
    struct request_type
    {
        int type;
        address_type addr;
        std::string id;
        chunk_type body;
    };

    // ��� ����������� ������
    typedef std::vector<char> pack_type;
    // ��� ������� ��������
    typedef std::list<request_type> queue_type;
    // �������� � ��������� ������������ �������
    enum {
        HDR_TYPE,
        HDR_ADDRRANK,
        HDR_ADDRNUM,
        HDR_IDLEN,
        HDR_BODYLEN,
        HEADERLENGTH
    };

    // ���������� ��������� � ���� ��������
    int m_size, m_rank;
    // ����������� ������� �� ���������� ������ ��������
    std::vector<std::vector<int> > m_addrmap;
    // �������� ��������� ������� � ������� �������� � �� ����
    int m_count, m_full;
    // �������� �������
    std::vector<queue_type> m_sque;
    // ������� �������
    queue_type m_rque;

    // �������� ������� ��� ��������
    static
    pack_type packque(const queue_type &queue)
    {
        pack_type pack;
        int pos = 0;
        queue_type::const_iterator it;
        for (it = queue.begin(); it != queue.end(); ++it)
        {
            const request_type &req = *it;
            // ��������� ��������� ������
            int hdr[HEADERLENGTH] = {0};
            hdr[HDR_TYPE] = req.type;
            hdr[HDR_ADDRRANK] = req.addr.rank;
            hdr[HDR_ADDRNUM] = req.addr.num;
            hdr[HDR_IDLEN] = req.id.size();
            hdr[HDR_BODYLEN] = req.body.size();
            // ��������� ������ ������ ������
            int full = 0, size;
            MPI_Pack_size(
                HEADERLENGTH, MPI_INT,
                MPI_COMM_WORLD, &size);
            full += size;
            MPI_Pack_size(
                req.id.size(), MPI_CHAR,
                MPI_COMM_WORLD, &size);
            full += size;
            MPI_Pack_size(
                req.body.size(), MPI_BYTE,
                MPI_COMM_WORLD, &size);
            full += size;
            // ��������� ����� � ��������� ��� �������
            pack.resize(pack.size() + full);
            MPI_Pack(
                &hdr, HEADERLENGTH, MPI_INT,
                &pack.front(), pack.size(), &pos,
                MPI_COMM_WORLD);
            // ������ ������, ���� ��� �� �����
            if (hdr[HDR_IDLEN] > 0)
            {
                MPI_Pack(
                    (void *) req.id.data(), req.id.size(), MPI_CHAR,
                    &pack.front(), pack.size(), &pos,
                    MPI_COMM_WORLD);
            };
            // ������ ����, ����� ���� �� �����
            if (hdr[HDR_BODYLEN] > 0)
            {
                MPI_Pack(
                    (void *) &req.body.front(), req.body.size(), MPI_BYTE,
                    &pack.front(), pack.size(), &pos,
                    MPI_COMM_WORLD);
            };
        };
        return pack;
    }

    // ���������� �������� ������ � ������������ �������
    static
    queue_type unpackque(const pack_type &pack)
    {
        queue_type queue;
        int pos = 0;
        while (size_t(pos) < pack.size())
        {
            request_type req;
            // ������������� ���������
            int hdr[HEADERLENGTH] = {0};
            MPI_Unpack(
                (void *) &pack.front(), pack.size(), &pos,
                &hdr, HEADERLENGTH, MPI_INT,
                MPI_COMM_WORLD);
            // ��������� ������ ���������
            req.type = hdr[HDR_TYPE];
            req.addr.rank = hdr[HDR_ADDRRANK];
            req.addr.num = hdr[HDR_ADDRNUM];
            // ������ ��������� ����� ������������� �����
            if (hdr[HDR_IDLEN] > 0)
            {
                std::vector<char> buf(hdr[HDR_IDLEN]);
                MPI_Unpack(
                    (void *) &pack.front(), pack.size(), &pos,
                    &buf.front(), buf.size(), MPI_CHAR,
                    MPI_COMM_WORLD);
                req.id = std::string(buf.begin(), buf.end());
            };
            // ���� � ������ �������� ��������
            if (hdr[HDR_BODYLEN] > 0)
            {
                req.body.resize(hdr[HDR_BODYLEN]);
                MPI_Unpack(
                    (void *) &pack.front(), pack.size(), &pos,
                    &req.body.front(), req.body.size(), MPI_BYTE,
                    MPI_COMM_WORLD);
            };
            queue.push_back(req);
        };
        return queue;
    }

    // === ��������� ��� scheduler_type ===
    friend class scheduler_type;
    // �����������
    dispatcher_type(void):
        m_count(0), m_full(0)
    {
        MPI_Comm_size(MPI_COMM_WORLD, &m_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
        m_addrmap.resize(m_size);
        m_sque.resize(m_size);
    }

    // �������� ������ ����������� ������
    address_type newaddr(void)
    {
        address_type newaddr;
        newaddr.rank = m_rank;
        newaddr.num = m_count++;
        return newaddr;
    }
    // ������������� ���������� �������
    void replicate(void)
    {
        // �������� �������� �� ���� ���������
        std::vector<int> allcnt(m_size);
        MPI_Allgather(
            &m_count, 1, MPI_INT,
            &allcnt.front(), 1, MPI_INT,
            MPI_COMM_WORLD);
        // ��������� ����� ������ ����������� ��������
        for (int rank = 0; rank < m_size; ++rank)
        {
            int oldcnt = m_addrmap[rank].size();
            m_addrmap[rank].resize(allcnt[rank]);
            for (int i = oldcnt; i < allcnt[rank]; ++i)
                m_addrmap[rank][i] = m_full++;
        };
    }
    // ���������� (���� ��������) ������� � �������� �������
    int location(const address_type &address) const
    {
        assert(address.rank >= 0 && address.rank < m_size);
        assert(
            address.num >= 0 &&
            size_t(address.num) < m_addrmap[address.rank].size());
        return m_addrmap[address.rank][address.num] % m_size;
    }
    // ����� ������� � ���������� ��� ��������
    int index(const address_type &address) const
    {
        assert(location(address) == m_rank);
        return m_addrmap[address.rank][address.num] / m_size;
    }

    // ��������� ������� � ���� �� �������� ��������
    void send(const request_type &req)
    {
        m_sque[location(req.addr)].push_back(req);
    }
    // ������� ������� �� ������� �������
    bool recv(request_type &req)
    {
        bool rc = !m_rque.empty();
        if (rc)
        {
            req = m_rque.front();
            assert(location(req.addr) == m_rank);
            m_rque.pop_front();
        };
        return rc;
    }
    // ������������� ����� ���������
    // ������������ �������� �������, ����������� �������
    void dispatch(void)
    {
        // ������������ � ����������� ������
        pack_type sdata, rdata;
        // ������� � �������� ������������ ������ ������
        std::vector<int> ssize(m_size), spos(m_size);
        // ������� � �������� ����������� ������ ������
        std::vector<int> rsize(m_size), rpos(m_size);

        // ��������� �� ���� �������� ��������
        for (int rank = 0; rank < m_size; ++rank)
        {
            // ������ � ������ �������
            pack_type pack = packque(m_sque[rank]);
            m_sque[rank].clear();
            // ��������� � ������������ ������
            sdata.insert(sdata.end(), pack.begin(), pack.end());
            // ��������� ������
            ssize[rank] = pack.size();
            // � �������� �� ������ ������������ ������
            spos[rank] = !rank ? 0 : spos[rank - 1] + ssize[rank - 1];
        };
        // ���������� ����� ��������� ������������ ������ ������
        // ��������� - ������� ����������� ������ ������
        MPI_Alltoall(
            &ssize.front(), 1, MPI_INT,
            &rsize.front(), 1, MPI_INT,
            MPI_COMM_WORLD);
        // ��������� ������ � �������� ��� �������� ������
        for (int rank = 0; rank < m_size; ++rank)
        {
            rdata.resize(rdata.size() + rsize[rank]);
            rpos[rank] = !rank ? 0 : rpos[rank - 1] + rsize[rank - 1];
        };

        // ��������� �� ����� ��� ����������� ��������� � front()
        sdata.push_back(0);
        rdata.push_back(0);
        // ���������� ����� ������� ����� ����� ����������
        MPI_Alltoallv(
            &sdata.front(), &ssize.front(), &spos.front(), MPI_PACKED,
            &rdata.front(), &rsize.front(), &rpos.front(), MPI_PACKED,
            MPI_COMM_WORLD);
        // ������� ������ ����
        sdata.pop_back();
        rdata.pop_back();

        // ������������� � ��������� �������� ������� ��������
        queue_type rque = unpackque(rdata);
        m_rque.insert(m_rque.end(), rque.begin(), rque.end());
    }

    // �������� ��������� �������
    bool busy(bool b) const
    {
        // ����������� ��������� ��������� �� ���� ���������
        int localbusy = b ? 1 : 0, allbusy;
        MPI_Allreduce(
            &localbusy, &allbusy, 1, MPI_INT,
            MPI_LOR, MPI_COMM_WORLD);
        return allbusy != 0;
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
    // ��������� ��������
    dispatcher_type m_disp;

    // === ��������� ��� actor_type ===
    friend class actor_type;
    // ���������� ������� �� �������� ������
    address_type new_actor(const behaviour_type &behaviour)
    {
        // ������������ ������ ������
        address_type newaddr = m_disp.newaddr();
        // ���������� �������
        m_actorqueue.push_back(
            actorqueue_type::value_type(newaddr, behaviour));
        return newaddr;
    }
    // ���������� ������� �� �������� ���������
    void new_task(const task_type &task)
    {
        m_taskqueue.push_back(task);
    }

public:
    // === ��������� ��� ����������� ���� ===

    // �����������
    scheduler_type(void)
    {
        // ������-����� �������� ������ � ������������
        // ������� � ���� ���, ��� � ��������� ������
        m_origin.bind(this, NULL);
    }

    // "����� �������" ������� �������
    const actor_type & system(void) const
    {
        return m_origin;
    }

    // ������ ���� �������� ������� �������
    void evolve(const factory_type &factory)
    {
        bool busy;
        do
        {
            // ���������� ����� �������� �������
            m_disp.replicate();

            // �������� ���� ��������
            while (!m_actorqueue.empty())
            {
                // ������� �� ������� ����� ����� � ���������
                dispatcher_type::request_type req = {
                    dispatcher_type::REQ_CREATE,
                    m_actorqueue.front().first,
                    m_actorqueue.front().second.defid,
                    m_actorqueue.front().second.chunk
                };
                m_actorqueue.pop_front();
                // �������� ������ ����������
                m_disp.send(req);
            };
            while (!m_taskqueue.empty())
            {
                // ������� �� ������� ���������
                dispatcher_type::request_type req = {
                    dispatcher_type::REQ_SEND,
                    m_taskqueue.front().address,
                    m_taskqueue.front().patid,
                    m_taskqueue.front().chunk
                };
                m_taskqueue.pop_front();
                // �������� ������ ����������
                m_disp.send(req);
            };

            // ������������� ���� �������� ����� ����������
            m_disp.dispatch();

            // ��������� � ��������� ���� ����� ��������
            dispatcher_type::request_type req;
            while (m_disp.recv(req))
            {
                if (req.type == dispatcher_type::REQ_CREATE)
                {
                    // ��������� ������� �� �������� ������
                    behaviour_type behaviour = { req.id, req.body };
                    // ������� �������
                    assert(
                        size_t(m_disp.index(req.addr)) ==
                        m_arbiterlist.size());
                    m_arbiterlist.push_back(
                        new arbiter_type(factory, req.addr, behaviour, this));
                }
                else
                {
                    // ��������� ������� �� �������� ���������
                    task_type task = { req.addr, req.id, req.body };
                    // �������� ��� � ��������������� �������� ����
                    m_arbiterlist[m_disp.index(req.addr)]->deliver(task);
                };
            };

            // ���� ������ ������� �������
            busy = false;
            int allnum = m_arbiterlist.size();
            for (int addr = 0; addr < allnum; ++addr)
            {
                // ��������� ������� ������� ��������� � �������
                busy = busy || !m_arbiterlist[addr]->empty();
                // �������� �������� � ���������� ���������
                task_type task;
                if (m_arbiterlist[addr]->retrieve(task))
                    m_arbiterlist[addr]->process(task);
            };
            // �����, ���� � ������� ��� ���������
        } while (m_disp.busy(busy));

        // ����������� ��������
        int allnum = m_arbiterlist.size();
        for (int addr = 0; addr < allnum; ++addr)
            delete m_arbiterlist[addr];
        m_arbiterlist.clear();
        m_disp = dispatcher_type();
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
