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


#ifndef _ZMATRIX_HXX_
#define _ZMATRIX_HXX_

#include <vector>
#include <cassert>

namespace z {

// матрица
template <typename e_t>
class matrix_type
{
public:
    typedef e_t element_type;

private:
    int m_vsize, m_hsize;
    std::vector<element_type> m_data;

public:
    // конструктор (инициализация значениями element_type())
    matrix_type(int vsize, int hsize):
        m_vsize(vsize), m_hsize(hsize)
    {
        assert(m_vsize > 0 && m_hsize > 0);
        m_data.resize(m_vsize * m_hsize, element_type());
    }

    // размеры по вертикали и горизонтали
    int vsize(void) const
    {
        return m_vsize;
    }
    int hsize(void) const
    {
        return m_hsize;
    }

    // обращение к элементам по индексам (нумерация с нуля)
    element_type & operator ()(int i, int j)
    {
        assert(i >= 0 && j >= 0 && i < vsize() && j < hsize());
        return m_data[i * hsize() + j];
    }
    const element_type & operator ()(int i, int j) const
    {
        return const_cast<matrix_type *>(this)->operator ()(i, j);
    }

    // прибавление матрицы
    matrix_type & operator +=(const matrix_type &src)
    {
        assert(hsize() == src.hsize() && vsize() == src.vsize());
        #pragma omp parallel for
        for (int i = 0; i < vsize(); ++i)
            for (int j = 0; j < hsize(); ++j)
                (*this)(i, j) += src(i, j);
        return *this;
    }
    // вычитание матрицы
    matrix_type & operator -=(const matrix_type &src)
    {
        assert(hsize() == src.hsize() && vsize() == src.vsize());
        #pragma omp parallel for
        for (int i = 0; i < vsize(); ++i)
            for (int j = 0; j < hsize(); ++j)
                (*this)(i, j) -= src(i, j);
        return *this;
    }

    // сумма двух матриц
    friend
    matrix_type operator +(const matrix_type &src1, const matrix_type &src2)
    {
        assert(src1.hsize() == src2.hsize());
        assert(src1.vsize() == src2.vsize());
        return (matrix_type(src1) += src2);
    }
    // разность двух матриц
    friend
    matrix_type operator -(const matrix_type &src1, const matrix_type &src2)
    {
        assert(src1.hsize() == src2.hsize());
        assert(src1.vsize() == src2.vsize());
        return (matrix_type(src1) -= src2);
    }
    // перемножение двух матриц
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

// вектор - матрица в один столбец
template <typename e_t>
class vector_type: public matrix_type<e_t>
{
public:
    typedef matrix_type<e_t> base_type;
    typedef typename base_type::element_type element_type;

public:
    // конструктор (инициализация значениями element_type())
    vector_type(int vsize):
        base_type(vsize, 1)
    {}
    // конструктор - преобразование типа
    vector_type(const base_type &src):
        base_type(src.vsize(), 1)
    {
        assert(src.hsize() == 1);
        this->operator =(src);
    }

    // обращение к элементам по индексу (нумерация с нуля)
    const element_type & operator ()(int i) const
    {
        return base_type::operator ()(i, 0);
    }
    element_type & operator ()(int i)
    {
        return base_type::operator ()(i, 0);
    }
    // присваивание матрицы в один столбец
    vector_type & operator =(const base_type &src)
    {
        assert(src.hsize() == 1);
        return static_cast<vector_type &>(base_type::operator =(src));
    }
};

} // namespace z

#endif /* _ZMATRIX_HXX_ */
