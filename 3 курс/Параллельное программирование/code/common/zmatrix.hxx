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


#ifndef _ZMATRIX_HXX_
#define _ZMATRIX_HXX_

#include <vector>
#include <cassert>

namespace z {

// �������
template <typename e_t>
class matrix_type
{
public:
    typedef e_t element_type;

private:
    int m_vsize, m_hsize;
    std::vector<element_type> m_data;

public:
    // ����������� (������������� ���������� element_type())
    matrix_type(int vsize, int hsize):
        m_vsize(vsize), m_hsize(hsize)
    {
        assert(m_vsize > 0 && m_hsize > 0);
        m_data.resize(m_vsize * m_hsize, element_type());
    }

    // ������� �� ��������� � �����������
    int vsize(void) const
    {
        return m_vsize;
    }
    int hsize(void) const
    {
        return m_hsize;
    }

    // ��������� � ��������� �� �������� (��������� � ����)
    element_type & operator ()(int i, int j)
    {
        assert(i >= 0 && j >= 0 && i < vsize() && j < hsize());
        return m_data[i * hsize() + j];
    }
    const element_type & operator ()(int i, int j) const
    {
        return const_cast<matrix_type *>(this)->operator ()(i, j);
    }

    // ����������� �������
    matrix_type & operator +=(const matrix_type &src)
    {
        assert(hsize() == src.hsize() && vsize() == src.vsize());
        #pragma omp parallel for
        for (int i = 0; i < vsize(); ++i)
            for (int j = 0; j < hsize(); ++j)
                (*this)(i, j) += src(i, j);
        return *this;
    }
    // ��������� �������
    matrix_type & operator -=(const matrix_type &src)
    {
        assert(hsize() == src.hsize() && vsize() == src.vsize());
        #pragma omp parallel for
        for (int i = 0; i < vsize(); ++i)
            for (int j = 0; j < hsize(); ++j)
                (*this)(i, j) -= src(i, j);
        return *this;
    }

    // ����� ���� ������
    friend
    matrix_type operator +(const matrix_type &src1, const matrix_type &src2)
    {
        assert(src1.hsize() == src2.hsize());
        assert(src1.vsize() == src2.vsize());
        return (matrix_type(src1) += src2);
    }
    // �������� ���� ������
    friend
    matrix_type operator -(const matrix_type &src1, const matrix_type &src2)
    {
        assert(src1.hsize() == src2.hsize());
        assert(src1.vsize() == src2.vsize());
        return (matrix_type(src1) -= src2);
    }
    // ������������ ���� ������
    friend
    matrix_type operator *(const matrix_type &src1, const matrix_type &src2)
    {
        assert(src1.hsize() == src2.vsize());
        matrix_type mtx(src1.vsize(), src2.hsize());
        #pragma omp parallel for
        for (int i = 0; i < mtx.vsize(); ++i)
        {
            for (int j = 0; j < mtx.hsize(); ++j)
            {
                element_type sum(0);
                for (int k = 0; k < src1.hsize(); ++k)
                    sum += src1(i, k) * src2(k, j);
                mtx(i, j) = sum;
            };
        };
        return mtx;
    }
};

// ������ - ������� � ���� �������
template <typename e_t>
class vector_type: public matrix_type<e_t>
{
public:
    typedef matrix_type<e_t> base_type;
    typedef typename base_type::element_type element_type;

public:
    // ����������� (������������� ���������� element_type())
    vector_type(int vsize):
        base_type(vsize, 1)
    {}
    // ����������� - �������������� ����
    vector_type(const base_type &src):
        base_type(src.vsize(), 1)
    {
        assert(src.hsize() == 1);
        this->operator =(src);
    }

    // ��������� � ��������� �� ������� (��������� � ����)
    const element_type & operator ()(int i) const
    {
        return base_type::operator ()(i, 0);
    }
    element_type & operator ()(int i)
    {
        return base_type::operator ()(i, 0);
    }
    // ������������ ������� � ���� �������
    vector_type & operator =(const base_type &src)
    {
        assert(src.hsize() == 1);
        return static_cast<vector_type &>(base_type::operator =(src));
    }
};

} // namespace z

#endif /* _ZMATRIX_HXX_ */
