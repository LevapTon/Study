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


#include "zjobs.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::jobs;


// ��������� ���������� �����
class circuit_type
{
public:
    // ���������� ������������� ����������� ��������
    typedef int id_type;
    // �������� �� ������ ��������������� ���������
    typedef std::map<id_type, bool> valueset_type;

private:
    // ��������� ���������������
    typedef std::set<id_type> idset_type;

    // ��������� ������������� ���������� ��������
    class storage_type
    {
    private:
        // ��������� ���������� ��������
        valueset_type m_values;
        mutable CRITICAL_SECTION m_cs;
    public:
        storage_type(void)
        {
            ::InitializeCriticalSection(&m_cs);
        }
        ~storage_type(void)
        {
            ::DeleteCriticalSection(&m_cs);
        }
        // �������� ���������� ��������
        void put_value(id_type id, bool value)
        {
            ::EnterCriticalSection(&m_cs);
            m_values[id] = value;
            ::LeaveCriticalSection(&m_cs);
        }
        // ��������� ���������� ��������
        bool get_value(id_type id) const
        {
            bool rc;
            ::EnterCriticalSection(&m_cs);
            {
                valueset_type::const_iterator it = m_values.find(id);
                assert(it != m_values.end());
                rc = it->second;
            };
            ::LeaveCriticalSection(&m_cs);
            return rc;
        }
    };

    // ���������� �������
    class gate_type: public job_abstract_type
    {
    public:
        // ����������� �������� ���������� ��������
        enum op_type { AND, OR, NOT };
    private:
        // �������������� ������� � ��� ������
        const id_type m_id, m_in1, m_in2;
        // ������������� ��������
        const op_type m_op;
        // ����������� ��������� ������
        storage_type &m_storage;
    public:
        // �����������
        gate_type(
            id_type id, id_type in1, id_type in2, op_type op,
            storage_type &storage):
            m_id(id), m_in1(in1), m_in2(in2), m_op(op),
            m_storage(storage)
        {}
        // ���������� ������
        void run(void)
        {
            switch (m_op)
            {
            case AND:
            case OR:
                // ���������� �������� �, ���
                {
                    bool in1 = m_storage.get_value(m_in1);
                    bool in2 = m_storage.get_value(m_in2);
                    bool out = (m_op == AND) ? (in1 && in2) : (in1 || in2);
                    m_storage.put_value(m_id, out);
                };
                break;
            case NOT:
                // ���������� �������� ��
                m_storage.put_value(m_id, !m_storage.get_value(m_in1));
                break;
            };
        }
    };
    typedef std::map<id_type, gate_type> gates_type;


    // ��������� ���������� ��������
    gates_type m_gates;
    // �������������� ������ � �������
    idset_type m_inids, m_outids;
    // �������� �����
    jobcomplex_type m_jobcomplex;
    // ��������� ������������� ��������
    storage_type m_storage;


    // �������� ������ ��������� ��� �������� �������
    void add_gate(
        id_type id, id_type in1, id_type in2,
        gate_type::op_type op)
    {
        // id �� ������ ���� � ������� ��� ���������
        assert(m_inids.find(id) == m_inids.end());
        assert(m_gates.find(id) == m_gates.end());
        // ��� �����������, ��������, ������
        assert(
            m_inids.find(in1) != m_inids.end() ||
            m_gates.find(in1) != m_gates.end());
        assert(
            m_inids.find(in2) != m_inids.end() ||
            m_gates.find(in2) != m_gates.end());

        // ������� ����� ���������� �������
        gates_type::iterator it = m_gates.insert(
            gates_type::value_type(
                id,
                gate_type(id, in1, in2, op, m_storage))
        ).first;

        gates_type::iterator in1it = m_gates.find(in1);
        gates_type::iterator in2it = m_gates.find(in2);
        // ��������� ����� ������ � ��������
        m_jobcomplex.add_job(it->second);
        // ���� ����������� ������� ����� �����, ������ �� � ��������
        if (in1it != m_gates.end())
            m_jobcomplex.add_dependence(it->second, in1it->second);
        if (in2it != m_gates.end())
            m_jobcomplex.add_dependence(it->second, in2it->second);
    }

public:
    // �������� �������������� ����� �����
    void add_input(id_type id)
    {
        assert(m_inids.find(id) == m_inids.end());
        assert(m_gates.find(id) == m_gates.end());
        m_inids.insert(id);
    }
    // �������� �������������� ������ �����
    void add_output(id_type id)
    {
        assert(
            m_inids.find(id) != m_inids.end() ||
            m_gates.find(id) != m_gates.end());
        m_outids.insert(id);
    }
    // �������� �������� �
    void add_and(id_type id, id_type in1, id_type in2)
    {
        add_gate(id, in1, in2, gate_type::AND);
    }
    // �������� �������� ���
    void add_or(id_type id, id_type in1, id_type in2)
    {
        add_gate(id, in1, in2, gate_type::OR);
    }
    // �������� �������� ��
    void add_not(id_type id, id_type in)
    {
        add_gate(id, in, in, gate_type::NOT);
    }

    // ���������� ����� ��� �������� ������� ���������
    valueset_type execute(const valueset_type &input)
    {
        assert(input.size() == m_inids.size());
        idset_type::iterator it;
        // ���������� ��������� ������ �������� ����������
        for (it = m_inids.begin(); it != m_inids.end(); ++it)
        {
            valueset_type::const_iterator f = input.find(*it);
            assert(f != input.end());
            m_storage.put_value(f->first, f->second);
        };

        // ������������ ���������� ��������� �����
        m_jobcomplex.run();

        valueset_type output;
        // ������� �������� �������� �� ��������� ������
        for (it = m_outids.begin(); it != m_outids.end(); ++it)
            output[*it] = m_storage.get_value(*it);
        return output;
    }
};


int main(int argc, char *argv[])
{
    // �������������� ��������� �����
    enum {
        IN1, IN2, IN3,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        OUT1, OUT2, OUT3
    };

    // �����
    circuit_type circuit;

    // ������� ��������������
    circuit.add_input(IN1);
    circuit.add_input(IN2);
    circuit.add_input(IN3);
    // ���������� ��������
    circuit.add_not(F1, IN1);
    circuit.add_not(F2, IN2);
    circuit.add_not(F3, IN3);
    circuit.add_and(F4, IN1, IN2);
    circuit.add_and(F5, F1, F2);
    circuit.add_or(F6, F4, F5);
    circuit.add_and(F7, IN3, F6);
    circuit.add_and(F8, F2, F3);
    circuit.add_and(F9, IN1, F8);
    circuit.add_and(F10, F3, F6);
    circuit.add_and(F11, IN2, IN3);
    circuit.add_and(F12, F1, F11);
    circuit.add_or(OUT1, F9, F7);
    circuit.add_and(OUT2, IN2, F3);
    circuit.add_or(OUT3, F12, F10);
    // �������� ��������������
    circuit.add_output(OUT1);
    circuit.add_output(OUT2);
    circuit.add_output(OUT3);

    for (int i = 0; i < 8; ++i)
    {
        // ������� ������ ��������� ������ ������� ����������
        bool in1 = (i & 4) != 0;
        bool in2 = (i & 2) != 0;
        bool in3 = (i & 1) != 0;

        circuit_type::valueset_type input, output;
        // ������������ ��������� ������� ��������
        input[IN1] = in1;
        input[IN2] = in2;
        input[IN3] = in3;
        // ���������� �����
        output = circuit.execute(input);
        // ������ �������� ��������
        bool out1 = output[OUT1];
        bool out2 = output[OUT2];
        bool out3 = output[OUT3];

        // ����� ������ ������� ����������
        synprintf(stdout, "%d%d%d %d%d%d\n", in1, in2, in3, out1, out2, out3);
    };

    return 0;
}
