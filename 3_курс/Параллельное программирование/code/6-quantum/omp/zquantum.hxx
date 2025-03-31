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


#ifndef _ZQUANTUM_HXX_
#define _ZQUANTUM_HXX_

#define _USE_MATH_DEFINES
#include <complex>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <set>
#include <list>
#include <algorithm>

#include "../../common/zmatrix.hxx"

#ifdef _OPENMP
#include <omp.h>
#endif //_OPENMP



namespace z {
namespace quantum {



// ===============================================================
// =============== ��������� ���������� ���������� ===============

inline void randomize(void)
{
    srand((unsigned int) time(NULL));
}

// ��������� ���������� ��������������� ����� [0.0, 1.0)
template <class real_type>
real_type random(void)
{
    return rand() / (real_type(RAND_MAX) + 1);
}


// === �����-������� ��� ��������� ��������� ���� � ���� ��������� ===
// ������������ ���������� ����������� ���� � ������ ����������
template <typename base_type>
class holder_type
{
private:
    enum op_type { BORN, DIE };
    // ������ �������� � ����������� ����������� ����� �����
    template <typename some_type>
    static
    void borndie(const base_type **pp, op_type op)
    {
        if (op == BORN)
            *pp = new some_type(*static_cast<const some_type *>(*pp));
        else if (op == DIE)
            delete static_cast<const some_type *>(*pp);
    }

    // ��������� �� ����������� ������� �������� � �����������
    void (*cdtor)(const base_type **, op_type);
    // ��������� �� ����������� ����� �����
    const base_type *ptr;

public:
    // ��������� ����������� ��� ������������ �����
    template <typename some_type>
    explicit
    holder_type(const some_type &circuit)
    {
        // ������������ ������ ��� ��������� ����
        cdtor = &holder_type::template borndie<some_type>;
        // ������� ����������� ����� �������
        ptr = &circuit;
        cdtor(&ptr, BORN);
    }
    // ����������
    ~holder_type(void)
    {
        // ���������� ����������� ����� �������
        cdtor(&ptr, DIE);
    }
    // ����������� �����������
    holder_type(const holder_type &src):
        cdtor(src.cdtor), ptr(src.ptr)
    {
        cdtor(&ptr, BORN);
    }
    // �������� ������������
    holder_type & operator =(const holder_type &src)
    {
        if (&src != this)
        {
            cdtor(&ptr, DIE);
            cdtor = src.cdtor;
            ptr = src.ptr;
            cdtor(&ptr, BORN);
        };
        return *this;
    }
    // ������ �� �������� ������
    const base_type & ref(void) const
    {
        return *ptr;
    }
};


// === ����������� ��������� ����� ===
class circuit_abstract_type
{
public:
    // �������������� �����
    typedef double real_type;
    // ����������� �����
    typedef std::complex<real_type> element_type;
    // ������ ���������� ���������
    typedef vector_type<element_type> qustate_type;
    // ����� ������� �������
    typedef std::vector<int> pinlist_type;

    // ������ ������ ������� ���������
    inline
    int qustate_size(void) const { return 1 << qubit_number(); }
    // ���������� �������������� �������
    virtual
    int qubit_number(void) const = 0;
    // ���������� ����� ��� ���������� �������� ���������
    virtual
    void execute(qustate_type &qustate) const = 0;
};


// === ������������ ������� ===
class gate_unary_type: public circuit_abstract_type
{
private:
    // ������� �������������� 2x2
    matrix_type<element_type> m_program;

protected:
    // ������� ������ �������������� ������ �� ���� �������� ���������
    // |state>  -->  a |0> + b |1>
    // � ����������� ������� ������ ���� ������� ������ - � 0 � 1
    void mapsto(
        int state,
        const element_type &a, const element_type &b)
    {
        assert(state >= 0 && state < qustate_size());
        m_program(0, state) = a;
        m_program(1, state) = b;
    }

public:
    gate_unary_type(void):
        m_program(qustate_size(), qustate_size())
    {}
    int qubit_number(void) const { return 1; }
    void execute(qustate_type &qustate) const
    {
        assert(qustate.vsize() == qustate_size());
        qustate = m_program * qustate;
    }
};


// === ����������� ����� ===
// ����������� ����� - ��������� (� ������������ �������)
class circuit_controlled_type: public circuit_abstract_type
{
private:
    // �����, ���������� ����������
    holder_type<circuit_abstract_type> m_circuit;

public:
    // ����������� ������������ ����������� �����
    template <typename some_type>
    explicit
    circuit_controlled_type(const some_type &circuit):
        m_circuit(circuit)
    {}
    // ����������� �����������
    circuit_controlled_type(const circuit_controlled_type &circuit):
        m_circuit(circuit.m_circuit)
    {}

    int qubit_number(void) const
    {
        // ���������� ������� ������ �� ���� (�����������)
        return 1 + m_circuit.ref().qubit_number();
    }
    void execute(qustate_type &qustate) const
    {
        assert(qustate.vsize() == qustate_size());
        // ��������� ������ �������� ������� ���������
        qustate_type substate(qustate.vsize() / 2);
        std::copy(
            &qustate(substate.vsize()),
            &qustate(substate.vsize()) + substate.vsize(),
            &substate(0));
        // ����������� �� ���������� ���������� ������
        m_circuit.ref().execute(substate);
        // �������� ���������� ��������� �������
        std::copy(
            &substate(0),
            &substate(0) + substate.vsize(),
            &qustate(substate.vsize()));
    }
};


// === ������������ ��������� ��������� ����� ===
class circuit_type: public circuit_abstract_type
{
private:
    // ����� ������� ����� �������� ���������
    typedef std::vector<int> indexmap_type;
    // ������ ���� ���������� ���� ������ � �������� ������� �����
    typedef std::list<
        std::pair<
            holder_type<circuit_abstract_type>,
            std::pair<indexmap_type, indexmap_type>
        > > circuits_type;

    // ���������� �������, � ������� ������������ �����
    int m_qubitnum;
    // ���������� ����� (������ ���������� ���� � ��������)
    circuits_type m_circuits;


public:
    // �����������
    circuit_type(int qubitnum):
        m_qubitnum(qubitnum)
    {
        assert(qubitnum > 0 && size_t(qubitnum) < sizeof(int) * 8);
    }

    // === ������� ���������� � �������� ���������� ������� ===
    // ���������� ������� �����
    template <typename some_type>
    void add_unary(
        const some_type &circuit,
        int qubit)
    {
        pinlist_type pin;
        pin.push_back(qubit);
        add_circuit(circuit, pin);
    }
    // ���������� �������� �����
    template <typename some_type>
    void add_binary(
        const some_type &circuit,
        int qubit1, int qubit2)
    {
        pinlist_type pin;
        pin.push_back(qubit1);
        pin.push_back(qubit2);
        add_circuit(circuit, pin);
    }
    // ���������� ��������� �����
    template <typename some_type>
    void add_ternary(
        const some_type &circuit,
        int qubit1, int qubit2, int qubit3)
    {
        pinlist_type pin;
        pin.push_back(qubit1);
        pin.push_back(qubit2);
        pin.push_back(qubit3);
        add_circuit(circuit, pin);
    }
    // ���������� ����� �� ������������ ���������� �������
    template <typename some_type>
    void add_circuit(
        const some_type &circuit,
        const pinlist_type &pin)
    {
        // �������� �������� ������ ������� �������
        assert(pin.size() == size_t(circuit.qubit_number()));
        assert(std::set<int>(pin.begin(), pin.end()).size() == pin.size());
        assert(*std::min_element(pin.begin(), pin.end()) >= 0);
        assert(*std::max_element(pin.begin(), pin.end()) < qubit_number());

        // ������ ������ ������� �������, ������� ��� � pin
        pinlist_type rest;
        // ������� ��� ������ �� pin � ������� �����
        int pinmask = 0;
        pinlist_type::const_iterator it;
        for (it = pin.begin(); it != pin.end(); ++it)
            pinmask |= (1 << *it);
        // ������� ��� ������ �������, ������� ��� � �����
        for (int i = 0; i < qubit_number(); ++i)
            if ((pinmask & (1 << i)) == 0)
                rest.push_back(i);

        indexmap_type::size_type i;
        pinlist_type::size_type j;
        // ������ �� ������ pin ����� ������� �����, ������������
        // ��������������� �������� ��������� ������� ��������
        // � ������� ����� ��������� ��������� ���� �����
        indexmap_type inner(1 << pin.size(), 0);
        for (i = 0; i < inner.size(); ++i)
            for (j = 0; j < pin.size(); ++j)
                inner[i] |= (i & (1 << j)) ? (1 << pin[j]) : 0;
        // ���������� ��� rest
        indexmap_type outer(1 << rest.size(), 0);
        for (i = 0; i < outer.size(); ++i)
            for (j = 0; j < rest.size(); ++j)
                outer[i] |= (i & (1 << j)) ? (1 << rest[j]) : 0;

        // ��������� ����� ����� � ��������������� ������
        m_circuits.push_back(
            circuits_type::value_type(
                holder_type<circuit_abstract_type>(circuit),
                make_pair(inner, outer)));
    }
    // ������� ���� ��������� �����
    void remove_all(void)
    {
        m_circuits.clear();
    }

    // === ���������� ������� ������������ ������ ===
    int qubit_number(void) const
    {
        return m_qubitnum;
    }
    void execute(qustate_type &qustate) const
    {
        assert(qustate.vsize() == qustate_size());

        // ��������������� ��������� ��� ��������
        circuits_type::const_iterator it;
        for (it = m_circuits.begin(); it != m_circuits.end(); ++it)
        {
            const circuit_abstract_type &c = it->first.ref();
            const indexmap_type &inner = it->second.first;
            const indexmap_type &outer = it->second.second;

            // ��������� �� ���� �������������
            int subnum = qustate_size() / c.qustate_size();
            #pragma omp parallel for
            for (int i = 0; i < subnum; ++i)
            {
                // ��������� ��������� ������������
                qustate_type substate(c.qustate_size());
                #pragma omp parallel for
                for (int j = 0; j < substate.vsize(); ++j)
                    substate(j) = qustate(outer[i] | inner[j]);

                // ��������� � ���� �����
                c.execute(substate);

                // ������������ ������������ �������
                #pragma omp parallel for
                for (int j = 0; j < substate.vsize(); ++j)
                    qustate(outer[i] | inner[j]) = substate(j);
            };
        };
    }

    // === ��������������� ������� ������� ��������� ��������� ===
    static
    pinlist_type range(int begin, int end)
    {
        int d = (begin < end) ? 1 : -1;
        pinlist_type pin;
        for (int i = begin; i != end; i += d)
            pin.push_back(i);
        return pin;
    }
};


// === ��������� ��������� ===
class quantum_machine_type: public circuit_type
{
private:
    // ������ �������� ���������
    qustate_type m_qustate;

public:
    quantum_machine_type(int qubitnum):
        circuit_type(qubitnum), m_qustate(qustate_size())
    {
        prepare(0);
    }

    // === ������������ ������� ���������� ���������� ===
    // ���������� (���������� � ���������� �������� ���������)
    int prepare(int state)
    {
        assert(state >= 0 && state < qustate_size());
        m_qustate = qustate_type(qustate_size());
        m_qustate(state) = 1.0;
        return state;
    }
    // ���������� ���������� ��������� �����
    void run(void)
    {
        execute(m_qustate);
    }
    // ��������� ���������� ���������
    int measure(void)
    {
        real_type rnd = random<real_type>();
        real_type prob = 0.0;
        int state = -1;
        for (int i = 0; state < 0 && i < m_qustate.vsize(); ++i)
        {
            prob += std::norm(m_qustate(i));
            state = (rnd < prob) ? i : state;
        };
        return prepare(state);
    }

    // === �������, ����������� ���� ������������ ===
    // ��������� ������� �������� ���������
    qustate_type qustate(void) const
    {
        return m_qustate;
    }
    // ���������� ������� ������������ ������ ��������������
    matrix_type<element_type> build_matrix(void) const
    {
        matrix_type<element_type> program(qustate_size(), qustate_size());

        for (int j = 0; j < program.hsize(); ++j)
        {
            // ��������� ��������� �������� ������
            qustate_type qustate(qustate_size());
            qustate(j) = 1.0;

            // ����������� ���
            execute(qustate);

            // �������� �������� ���������� � �������
            for (int i = 0; i < program.vsize(); ++i)
                program(i, j) = qustate(i);
        };
        return program;
    }
};



// ==================================================================
// =============== �������� ��������� ������� � ����� ===============

// ������� HADAMARD
class gate_hadamard_type: public gate_unary_type
{
public:
    gate_hadamard_type(void)
    {
        mapsto(0, M_SQRT1_2, M_SQRT1_2);
        mapsto(1, M_SQRT1_2, - M_SQRT1_2);
    }
};

// ������� PHASE SHIFT
// �������� - ����� ������� ������� (������� �� frac * 2 * pi)
class gate_shift_type: public gate_unary_type
{
public:
    gate_shift_type(real_type frac)
    {
        mapsto(0, 1.0, 0.0);
        mapsto(1, 0.0, exp(element_type(0.0, 2.0) * M_PI * frac));
    }
};

// ������� NOT
class gate_not_type: public gate_unary_type
{
public:
    gate_not_type(void)
    {
        mapsto(0, 0.0, 1.0);
        mapsto(1, 1.0, 0.0);
    }
};

// ������������ ������� CNOT
class gate_cnot_type: public circuit_controlled_type
{
public:
    gate_cnot_type(void):
        circuit_controlled_type(gate_not_type())
    {}
    gate_cnot_type(const gate_cnot_type &src):
        circuit_controlled_type(static_cast<const circuit_controlled_type &>(src))
    {}
};

// ������������ ������� TOFFOLI
class gate_toffoli_type: public circuit_controlled_type
{
public:
    gate_toffoli_type(void):
        circuit_controlled_type(gate_cnot_type())
    {}
    gate_toffoli_type(const gate_toffoli_type &src):
        circuit_controlled_type(static_cast<const circuit_controlled_type &>(src))
    {}
};
/*
// ������������ ������� TOFFOLI - �������������� ����������
class gate_toffoli_type: public circuit_type
{
public:
    gate_toffoli_type(void):
        circuit_type(3)
    {
        add_unary(gate_hadamard_type(), 0);
        add_binary(gate_cnot_type(), 0, 1);
        add_unary(gate_shift_type(0.875), 0);
        add_binary(gate_cnot_type(), 0, 2);
        add_unary(gate_shift_type(0.125), 0);
        add_binary(gate_cnot_type(), 0, 1);
        add_unary(gate_shift_type(0.875), 0);
        add_binary(gate_cnot_type(), 0, 2);
        add_unary(gate_shift_type(0.125), 0);
        add_unary(gate_hadamard_type(), 0);
        add_unary(gate_shift_type(0.875), 1);
        add_binary(gate_cnot_type(), 1, 2);
        add_unary(gate_shift_type(0.875), 1);
        add_binary(gate_cnot_type(), 1, 2);
        add_unary(gate_shift_type(0.250), 1);
        add_unary(gate_shift_type(0.125), 2);
    }
};
*/

// ������������ ������� SWAP
class gate_swap_type: public circuit_type
{
public:
    gate_swap_type(void):
        circuit_type(2)
    {
        add_binary(gate_cnot_type(), 0, 1);
        add_binary(gate_cnot_type(), 1, 0);
        add_binary(gate_cnot_type(), 0, 1);
    }
};

// ������������ ������� FREDKIN
class gate_fredkin_type: public circuit_controlled_type
{
public:
    gate_fredkin_type(void):
        circuit_controlled_type(gate_swap_type())
    {}
    gate_fredkin_type(const gate_fredkin_type &src):
        circuit_controlled_type(static_cast<const circuit_controlled_type &>(src))
    {}
};

// n-�������� ����� HADAMARD
class circuit_hadamard_type: public circuit_type
{
public:
    circuit_hadamard_type(int n):
        circuit_type(n)
    {
        for (int i = 0; i < qubit_number(); ++i)
            add_unary(gate_hadamard_type(), i);
    }
};

// n-�������� ����� CC...CCNOT
// ���������� ����� - ������
class circuit_ccccnot_type: public circuit_type
{
public:
    circuit_ccccnot_type(int n):
        circuit_type(n)
    {
        if (n > 1)
        {
            circuit_ccccnot_type lower(n - 1);
            add_circuit(
                circuit_controlled_type(lower),
                range(0, qubit_number()));
        }
        else
            add_unary(gate_not_type(), 0);
    }
};

// n-�������� ����� QFT
class circuit_fourier_type: public circuit_type
{
public:
    circuit_fourier_type(int n):
        circuit_type(n)
    {
        for (int i = qubit_number() - 1; i >= 0; --i)
        {
            // ������� �������
            add_unary(gate_hadamard_type(), i);
            for (int j = i - 1; j >= 0; --j)
            {
                // ����������� ����� ����
                gate_shift_type shift(1.0 / (1 << (i - j + 1)));
                add_binary(circuit_controlled_type(shift), i, j);
            };
        };
        // ��������� ������� ������� �������
        for (int i = 0; i < qubit_number() / 2; ++i)
            add_binary(gate_swap_type(), i, qubit_number() - i - 1);
    }
};



// ==============================================================
// =============== �������������� ��������� ����� ===============

// ���������� ����� a � ������� b �� ������ mod
int powmod(int a, int b, int mod)
{
    assert(a >= 0 && b >= 0 && mod > 0);
    a %= mod;
    // ��������� ��������, ����������� ��������� � ����� ����
    int p = (a != 0) ? 1 : 0, q = a, i = 0;
    // ���� � ������� ���� �������������� ��������� ����
    while ((b & ~((1 << i) - 1)) != 0)
    {
        // ���� ������� ��� � ������� ����������
        if ((b & (1 << i)) != 0)
            // ��������� �� ������� �����������
            p = p * q % mod;
        // �������� ������� ����������� � �������
        q = q * q % mod;
        // ��������� � ���������� ����
        ++i;
    };
    return p;
}

// ���������� ����������� ������ �������� (�������� �������)
int gcd(int a, int b)
{
    while (b != 0)
    {
        int r = a % b;
        a = b;
        b = r;
    };
    return (a < 0) ? - a : a;
}

// ���������� ��������� �������� ��������
int invmod(int a, int mod)
{
    assert(a > 0 && mod > 0);
    a %= mod;
    assert(gcd(a, mod) == 1);

    // ��������� �������� ��� ������������ �����������
    int ppre = 1, pcur = mod / a;
    // ������������ ����� � ������ �����
    int rpre = a, rcur = mod % a, n = 0;
    while (rcur != 0)
    {
        // ����� ��������� ���������� �����
        ++n;
        // ��������� ��������� ���������� �����
        int pnxt = (rpre / rcur) * pcur + ppre;
        ppre = pcur;
        pcur = pnxt;
        // ��������� ������� (�������� �������)
        int rnxt = rpre % rcur;
        rpre = rcur;
        rcur = rnxt;
    };
    // ��� ��������� n ����������� ����
    return ((n & 1) == 0) ? ppre : (mod - ppre);
}


// ����� ADD �������� ���� n-�������� �����
// ����������� �� 3n ������� (��������� �� ������ (n) + (n) + (n)):
// |a> |b> |0>   -->   |a> |a + b> |0>
// ������ ����� - a, �������� ����������
// ������ ����� - b, ���������� a + b
// ������ ����� - 0, ������������ � �������� � ������������ � 0
class circuit_adder_type: public circuit_type
{
private:
    class gate_fulladder_type: public circuit_type
    {
    public:
        gate_fulladder_type(void):
            circuit_type(4)
        {
            add_ternary(gate_toffoli_type(), 3, 1, 0);
            add_binary(gate_cnot_type(), 1, 0);
            add_ternary(gate_toffoli_type(), 3, 1, 2);
            add_binary(gate_cnot_type(), 1, 2);
        }
    };
    class gate_halfadder_type: public circuit_type
    {
    public:
        gate_halfadder_type(void):
            circuit_type(3)
        {
            add_ternary(gate_toffoli_type(), 2, 1, 0);
            add_binary(gate_cnot_type(), 1, 0);
        }
    };
    class gate_fulluncarry_type: public circuit_type
    {
    public:
        gate_fulluncarry_type(void):
            circuit_type(4)
        {
            add_unary(gate_not_type(), 1);
            add_ternary(gate_toffoli_type(), 3, 1, 0);
            add_binary(gate_cnot_type(), 1, 0);
            add_ternary(gate_toffoli_type(), 3, 1, 2);
            add_binary(gate_cnot_type(), 1, 0);
            add_unary(gate_not_type(), 1);
        }
    };
    class gate_halfuncarry_type: public circuit_type
    {
    public:
        gate_halfuncarry_type(void):
            circuit_type(3)
        {
            add_unary(gate_not_type(), 1);
            add_ternary(gate_toffoli_type(), 2, 1, 0);
            add_unary(gate_not_type(), 1);
        }
    };

public:
    circuit_adder_type(int n):
        circuit_type(3 * n)
    {
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                pinlist_type pin;
                pin.push_back(i);
                pin.push_back(n + i);
                pin.push_back(2 * n + i - 1);
                pin.push_back(2 * n + i);
                add_circuit(gate_fulladder_type(), pin);
            }
            else
                add_ternary(gate_halfadder_type(), i, n + i, 2 * n + i);
        };
        for (int i = n - 1; i >= 0; --i)
        {
            if (i > 0)
            {
                pinlist_type pin;
                pin.push_back(i);
                pin.push_back(n + i);
                pin.push_back(2 * n + i - 1);
                pin.push_back(2 * n + i);
                add_circuit(gate_fulluncarry_type(), pin);
            }
            else
                add_ternary(gate_halfuncarry_type(), i, n + i, 2 * n + i);
        };
    }
};

// ����� ADD �������� n-��������� ����� � ��������� num
// ����������� �� 2n-1 ��� 2n ������� (�������� storecarry)
// � ������ ������ ������ ��������� �� ������ (n) + (n-1):
// |a> |0>   -->   |a + num> |0>
// ������ ����� - a, ���������� (a + num) % 2^n
// ������ ����� - 0, ������������ � �������� � ������������ � 0
// �� ������ ������ ����������� ��� ���� �����:
// |a> |0> |0>   -->   |a + num> |0> |c>
// ������ ����� - ����� ��������, ������������� ��� ������������
class circuit_add_type: public circuit_type
{
public:
    circuit_add_type(int n, int num, bool storecarry = false):
        circuit_type(storecarry ? (2 * n) : (2 * n - 1))
    {
        assert(num >= 0 && num < (1 << n));
        // ������ ���������, �������� �����������
        for (int i = 0; i < n; ++i)
        {
            bool ibitset = (num & (1 << i)) != 0;
            // ������������, ����������� ����������
            if (i < n - 1 || storecarry)
                if (ibitset)
                    add_binary(gate_cnot_type(), n + i, i);
            if (ibitset)
                add_unary(gate_not_type(), i);
            // ��������� ����� ������� ���������
            if (i > 0)
            {
                if (i < n - 1 || storecarry)
                    add_ternary(gate_toffoli_type(), n + i, n + i - 1, i);
                add_binary(gate_cnot_type(), i, n + i - 1);
            };
        };
        // ������ ����������� ��������, ����� ����������
        for (int i = n - 2; i >= 0; --i)
        {
            bool ibitset = (num & (1 << i)) != 0;
            if (i > 0)
            {
                // ������������ ������� � ����� ������ - ����������
                // � ����� ��������������� ������
                add_unary(gate_not_type(), i);
                if (ibitset)
                    add_binary(gate_cnot_type(), n + i, i);
                if (ibitset)
                    add_unary(gate_not_type(), i);
                add_ternary(gate_toffoli_type(), n + i, n + i - 1, i);
                if (ibitset)
                    add_unary(gate_not_type(), i);
                add_unary(gate_not_type(), i);
            }
            else
            {
                // ����� ������ ������� ��������
                if (ibitset)
                {
                    add_unary(gate_not_type(), i);
                    add_binary(gate_cnot_type(), n + i, i);
                    add_unary(gate_not_type(), i);
                };
            };
        };
    }
};

// ����� LESS ��������� n-��������� ����� � ��������� num
// ����������� �� 2n �������
// ������ ��������� �� ������ (n) + (n-1) + (1):
// |a> |0> |0>   -->   |a> |0> |(a < num)>
// ������ ����� - a, �������� ����������
// ������ ����� - 0, ������������ � �������� � ������������ � 0
// ������ ����� - ���� less, ������������� ��� a < num
class circuit_less_type: public circuit_type
{
public:
    circuit_less_type(int n, int num):
        circuit_type(2 * n)
    {
        assert(num >= 0 && num < (1 << n));
        if (num > 0)
        {
            pinlist_type pin = range(0, qubit_number());
            // ������ ������� ����� � ��������� 2^n - num
            // � �������� ��������� ����� ��������
            add_circuit(circuit_add_type(n, (1 << n) - num, true), pin);
            // ������ ����� �� �����, ����� �������� �� �����������
            pin.pop_back();
            add_circuit(circuit_add_type(n, num), pin);
            // ����������� �����
            add_unary(gate_not_type(), qubit_number() - 1);
        };
    }
};

// ����� ADDMOD �������� n-��������� ����� � ��������� num
// �� ������ mod, ����������� �� 2n �������
// ������ ��������� �� ������ (n) + (n):
// |a> |0>  -->   |(a + num) % mod> |0>
// ������ ����� - a, ���������� (a + num) % mod
// ������ ����� - 0, ������������ � �������� � ������������ � 0
class circuit_addmod_type: public circuit_type
{
public:
    circuit_addmod_type(int n, int num, int mod):
        circuit_type(2 * n)
    {
        assert(mod > 0 && mod < (1 << n));
        assert(num >= 0 && num < mod);

        pinlist_type pin = range(0, qubit_number());

        // ��������, ������ �� ����� ��������� mod - num
        add_circuit(circuit_less_type(n, mod - num), pin);

        // ���� ������, �������� num
        circuit_add_type add1(n, num);
        add_circuit(circuit_controlled_type(add1), pin);
        // �����
        add_unary(gate_not_type(), pin.back());
        // �������� num - mod
        circuit_add_type add0(n, (1 << n) - mod + num);
        add_circuit(circuit_controlled_type(add0), pin);

        // ������ � ���� ����� ��������
        add_circuit(circuit_less_type(n, num), pin);
    }
};

// ����� MULMOD ��������� n-��������� ����� � ��������� num
// �� ������ mod, ����������� �� 3n �������
// ������ ��������� �� ������ (n) + (2n):
// |a> |0>  -->   |(a * num) % mod> |0>
// ������ ����� - a, ���������� (a * num) % mod
// ������ ����� - 0, ������������ � �������� � ������������ � 0
class circuit_mulmod_type: public circuit_type
{
public:
    circuit_mulmod_type(int n, int num, int mod):
        circuit_type(3 * n)
    {
        assert(mod > 0 && mod < (1 << n));
        assert(num >= 0 && num < mod);
        assert(gcd(num, mod) == 1);

        // ������ �������, � ������� ������������� ����� ��������
        pinlist_type pin = range(n, qubit_number());
        // �������� � �������� ����������� ���� ��������
        for (int i = 0; i < n; ++i)
        {
            int term = (((1 << i) % mod) * num) % mod;
            circuit_addmod_type addmod(n, term, mod);
            pin.push_back(i);
            add_circuit(circuit_controlled_type(addmod), pin);
            pin.pop_back();
        };
        // ������ ������� ���������� ��������� � ��������� ��������
        for (int i = 0; i < n; ++i)
            add_binary(gate_swap_type(), i, n + i);
        // ��������� ��������� �� ������ mod
        num = invmod(num, mod);
        // ��������� ��������� ��������
        for (int i = 0; i < n; ++i)
        {
            int term = (mod - (((1 << i) % mod) * num) % mod) % mod;
            circuit_addmod_type addmod(n, term, mod);
            pin.push_back(i);
            add_circuit(circuit_controlled_type(addmod), pin);
            pin.pop_back();
        };
    }
};

// ����� POWMOD ���������� ��������� num � �������
// m-��������� ����� �� n-��������� ������ mod
// ����������� �� m+3n �������
// ������ ��������� �� ������ (m) + (n) + (2n):
// |a> |0> |0>  -->   |a> |(num ^ a) % mod> |0>
// ������ ����� - a, �������� ����������
// ������ ����� - 0, ���������� (num ^ a) % mod
// ������ ����� - 0, ������������ � �������� � ������������ � 0
class circuit_powmod_type: public circuit_type
{
public:
    circuit_powmod_type(int m, int n, int num, int mod):
        circuit_type(m + 3 * n)
    {
        assert(m > 0 && n > 0);
        assert(mod > 0 && mod < (1 << n));
        assert(num >= 0 && num < mod);
        assert(gcd(num, mod) == 1);

        // ���������� �� ������ �������
        add_unary(gate_not_type(), m);

        // ������ �������, � ������� ������������� ����� ���������
        pinlist_type pin = range(m, qubit_number());
        // ������ ���������
        int factor = num;
        // �������� � �������� ����������� ���� ���������
        for (int i = 0; i < m; ++i)
        {
            // ����������� ����� ���������
            circuit_mulmod_type mulmod(n, factor, mod);
            // ���������� ������������ ������ � ������������� �����
            pin.push_back(i);
            add_circuit(circuit_controlled_type(mulmod), pin);
            pin.pop_back();
            // ���������� ������������ ���������
            factor = factor * factor % mod;
        };
    }
};


} // namespace quantum
} // namespace z

#endif /* _ZQUANTUM_HXX_ */
