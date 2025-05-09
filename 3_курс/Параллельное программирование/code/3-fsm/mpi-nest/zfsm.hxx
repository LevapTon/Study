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

// ����������� ��� ��������
class fsm_abstract_type
{
public:
    // ��� ��������� ��������
    typedef int state_type;
    // ��� ������� � �������� ������
    typedef int signal_type;
    // ������ ����� ������� ��� �������� ��������
    typedef std::vector<signal_type> signals_type;

    // ���������� ������
    virtual
    int number_input(void) const = 0;
    // ���������� �������
    virtual
    int number_output(void) const = 0;
    // �������� �������� ������ ������� ������
    virtual
    void put_input(const signals_type &input) = 0;
    // ��������� ���� ������ ��������
    virtual
    void do_work(void) = 0;
    // �������� �� �������� ������ �������� ������
    virtual
    signals_type get_output(void) const = 0;
    // ��������, ��������� �� ������� � ��������� ���������
    virtual
    bool is_off(void) const = 0;
};

// �������� �������
class fsm_type: public fsm_abstract_type
{
protected:
    // ����������, ���������� �� do_work � ���������� ���������
    typedef state_type (fsm_type::*handler_type)(state_type state);
    // ���������/�������� ���������
    enum { STATE_OFF = -1 };

private:
    // ������� ���������
    state_type m_state;
    // ������� ������������
    typedef std::map<state_type, handler_type> handlertable_type;
    handlertable_type m_handlertable;

protected:
    // ������� � �������� ������� �������� �� ������� �����
    signals_type m_input, m_output;
    // ������������ �������� (����� ���� ����� ������ ��������)
    MPI_Comm m_comm;

    void add_handler(state_type state, handler_type handler)
    {
        m_handlertable[state] = handler;
    }

public:
    fsm_type(int inputnum, int outputnum, MPI_Comm comm):
        m_state(STATE_OFF),
        m_input(inputnum, 0), m_output(outputnum, 0),
        m_comm(comm)
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

        // ����� ���������� �������� ��������� � ������� ���
        handlertable_type::iterator it = m_handlertable.find(m_state);
        assert(it != m_handlertable.end());
        m_state = (this->*(it->second))(m_state);

        assert(insize == m_input.size());
        assert(outsize == m_output.size());

        // ���� ������� ����������, �������� ������
        if (m_state == STATE_OFF)
            m_output.assign(m_output.size(), 0);
    }

    signals_type get_output(void) const
    {
        return m_output;
    }

    bool is_off(void) const
    {
        // �������� ���������� ������
        return m_state == STATE_OFF;
    }
};

// ���� �������� ���������
class fsmnet_type: public fsm_abstract_type
{
private:
    // ������ ������ ��������� �� ������ � �������
    // � ����� (� ��������, � ������) -> (� ��������, � �����)
    typedef std::list<
        std::pair<std::pair<int, int>, std::pair<int, int> >
    > linklist_type;
    // ��� ������������ ��� ����������� ����� � ������ ����
    enum { PSEUDOFSM_NETINPUT = -1, PSEUDOFSM_NETOUTPUT = -2 };

    // ����� ��� ������ � ������ �������
    class shared_area_type
    {
    private:
        enum { NOPOS = -1 };
        // ����� ������ (������ ���� ��������� � ����� ����)
        signals_type m_data;
        // ������� ����� � ������� �������� ������� ��������� � ������ ����
        std::vector<int> m_outpos, m_outsize;
        // ������ ������� ������ ��������� � ������� ����
        std::vector<std::vector<int> > m_allinpos;

        // ������ �� ����-���������
        fsmnet_type &m_fsmnet;

    public:
        // � ������������ �� ������ ��������� ������ ������ ���������
        // ������� ����� ������, ������ ������� �������� ��������
        // ���������, � ����� ������ ������� ���������� ������ ���������
        shared_area_type(const linklist_type &linklist, fsmnet_type &fsmnet):
            m_fsmnet(fsmnet)
        {
            linklist_type::const_iterator it;
            // ������ ���������� ��������� = ������������ ��������� + 1
            int fsmnum = -1;
            for (it = linklist.begin(); it != linklist.end(); ++it)
            {
                fsmnum = std::max(fsmnum, it->first.first);
                fsmnum = std::max(fsmnum, it->second.first);
            };
            ++fsmnum;

            // ���������� ������� ��������� (�������) � ������ ���� (���������)
            std::vector<int> outsize(fsmnum + 1);
            // ���������� ������ ��������� (�������) � ������� ���� (���������)
            std::vector<int> insize(fsmnum + 1);
            // ���� ������������ ������ ������ � ������� ��� ����
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
            // ������������ �� �������� (��������� �� �������)
            std::transform(
                outsize.begin(), outsize.end(),
                outsize.begin(), std::bind2nd(std::plus<int>(), 1));
            std::transform(
                insize.begin(), insize.end(),
                insize.begin(), std::bind2nd(std::plus<int>(), 1));

            // ��������� ������ ������� ����� �������� �������
            // ������ ������� �� �������� ��������
            m_outpos.assign(1, 0);
            // �������� ��������� - ��������� ����� �������� ��������
            std::partial_sum(
                outsize.begin(), outsize.end(),
                std::back_inserter(m_outpos));
            // ��������� (������) ������� - ������ ���������� ���� �������
            m_data.assign(m_outpos.back(), 0);
            m_outpos.pop_back();
            // ������� �������� �������� � ������� ����� ������
            m_outsize.swap(outsize);

            // ������� ������ ������� ������
            m_allinpos.resize(fsmnum + 1);
            for (int i = 0; i < fsmnum + 1; ++i)
                m_allinpos[i].resize(insize[i], NOPOS);
            // ��������� ������ ������� ������
            for (it = linklist.begin(); it != linklist.end(); ++it)
            {
                int idxout = it->first.first;
                int idxin = it->second.first;
                idxout = (idxout != PSEUDOFSM_NETINPUT) ? idxout : fsmnum;
                idxin = (idxin != PSEUDOFSM_NETOUTPUT) ? idxin : fsmnum;

                // ������ ���� ����� ���� ��������� ���� � ������ ������
                assert(m_allinpos[idxin][it->second.second] == NOPOS);
                m_allinpos[idxin][it->second.second] =
                    m_outpos[idxout] + it->first.second;
            };
        }
        // ���������� �������� ������ ����������� ��������
        void put_output(int i, const signals_type &output)
        {
            // ������������ �������� ������ ��������� ������
            if (m_fsmnet.m_commlocal != MPI_COMM_NULL)
            {
                // ������������� ���� ��������� (��� ��������� �� ������ ������)
                MPI_Barrier(m_fsmnet.m_commlocal);
                assert(size_t(m_outsize[i]) == output.size());
                std::copy(
                    output.begin(), output.end(),
                    m_data.begin() + m_outpos[i]);
                // �������� ������ ����� - ��������� ��� ������ ���� ���������
                // � ����� �������� �������� � �������� ���� �� ������
                // ������� ��������, �� ��� �� ������ - ��� �� �����������
                MPI_Allgatherv(
                    &m_data[m_outpos[i]], m_outsize[i], MPI_INT,
                    &m_data[0], &m_outsize.front(), &m_outpos.front(), MPI_INT,
                    m_fsmnet.m_commlocal);
            };
        }
        // ���������� ������� ������ ����
        void put_input(const signals_type &input)
        {
            // ������������ �������� ������ ��������� ������
            if (m_fsmnet.m_commlocal != MPI_COMM_NULL)
            {
                // ���� ���� ����������� ������ ������� ��������� � ����
                int rank;
                MPI_Comm_rank(m_fsmnet.m_commlocal, &rank);
                if (rank == 0)
                {
                    assert(size_t(m_outsize.back()) == input.size());
                    std::copy(
                        input.begin(), input.end(),
                        m_data.begin() + m_outpos.back());
                };
                // � ����������� ���������
                MPI_Bcast(
                    &m_data[m_outpos.back()], m_outsize.back(), MPI_INT,
                    0, m_fsmnet.m_commlocal);
            };
        }
        // �������������� ������� ������ ����������� ��������
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
        // �������������� �������� ������ ����
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
    // ����������� ��� ������� ���������� ����
    class factory_abstract_type
    {
    public:
        // ��������� ������ (��� ��������� �� ����������)
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

        // ���������� ��������� � ����
        virtual
        int number_fsm(void) const = 0;
        // ������ ���������� ���������� � ��������
        virtual
        int size_fsm(int id) const = 0;
        // ���������� ������ ����
        virtual
        int number_input(void) const = 0;
        // ���������� ������� ����
        virtual
        int number_output(void) const = 0;
        // ��������� ������ ���� ������
        virtual
        links_type links(void) const = 0;
        // �������� ���������� �������� � ����
        virtual
        fsm_abstract_type *create_fsm(int id, MPI_Comm commfull) = 0;
        // ����������� ��� ��
        virtual
        void destroy_fsm(int id, fsm_abstract_type *pfsm) = 0;
    };

private:

    // ������ �������� ���������
    std::vector<fsm_abstract_type *> m_pfsm;
    // ������� ����
    factory_abstract_type &m_factory;
    // ����� ������� ������
    shared_area_type m_shared;

    // ����� �������� ������� ����, ������������ ����� ���������
    int m_fsmid;
    // ������ ������������ �� ���� ��������� ����
    MPI_Comm m_commfull;
    // ������������ ��������� ����� �� ���� ���������
    MPI_Comm m_commlocal;
    // ������������, ���������� ������ ����/��������
    MPI_Comm m_commlower;

public:
    fsmnet_type(factory_abstract_type &factory, MPI_Comm commfull):
        m_factory(factory), m_shared(m_factory.links().get(), *this),
        m_commfull(commfull)
    {
        // �������� ������ ��������� ������� (����������� �������)
        std::vector<int> loc;
        // � ���������� ���� �������������� ����������� ������� ��������
        int rank, size;
        MPI_Comm_rank(m_commfull, &rank);
        MPI_Comm_size(m_commfull, &size);
        m_fsmid = -1;
        for (int i = 0, cur = 0; i < m_factory.number_fsm(); ++i)
        {
            // ������� ������ ������� �������� ���������� ��������
            loc.push_back(cur);
            // �������� � ���������� ��������
            cur += m_factory.size_fsm(i);
            assert(i + 1 < m_factory.number_fsm() || cur == size);
            // �������� ������ ���������� ��������, ���� �� ���
            if (m_fsmid < 0 && rank < cur)
                m_fsmid = i;
        };
        assert(m_fsmid != -1);

        // �������� ������������ ��������� ������� ����
        MPI_Group group, newgroup;
        MPI_Comm_group(m_commfull, &group);
        MPI_Group_incl(group, loc.size(), &loc.front(), &newgroup);
        MPI_Comm_create(m_commfull, newgroup, &m_commlocal);
        MPI_Group_free(&newgroup);
        MPI_Group_free(&group);

        // �������� ������ ������������ (��� ������� ��� ��������)
        MPI_Comm_split(m_commfull, m_fsmid, rank, &m_commlower);

        // �������� ������ ������� ��� �������
        m_pfsm.push_back(m_factory.create_fsm(m_fsmid, m_commlower));
    }

    ~fsmnet_type(void)
    {
        // ����������� ��������
        m_factory.destroy_fsm(m_fsmid, m_pfsm.back());
        // �������� ��������� ��������������
        MPI_Comm_free(&m_commlower);
        if (m_commlocal != MPI_COMM_NULL)
            MPI_Comm_free(&m_commlocal);
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
        // ��������� ������� ������, ��������� ��������
        m_pfsm.back()->put_input(m_shared.get_input(m_fsmid));
        m_pfsm.back()->do_work();
        // �������� �������� (������ ������ ���� �������� �� ������)
        m_shared.put_output(m_fsmid, m_pfsm.back()->get_output());
    }

    signals_type get_output(void) const
    {
        return m_shared.get_output();
    }

    bool is_off(void) const
    {
        // ���� � ����������� ���������, ����� ��� �������� ���������
        int isoff = m_pfsm.back()->is_off();
        // ������ ������� �� ��������� ������� � ������� �������
        int allisoff;
        if (m_commlocal != MPI_COMM_NULL)
            MPI_Reduce(&isoff, &allisoff, 1, MPI_INT, MPI_LAND, 0, m_commlocal);
        // � ����������� ��������� ����� �����
        // �� ���������� ������� � m_commfull ��� ��, ��� � � m_commlocal
        MPI_Bcast(&allisoff, 1, MPI_INT, 0, m_commfull);
        return allisoff;
    }
};

} // namespace fsm
} // namespace z

#endif /* _ZFSM_HXX_ */
