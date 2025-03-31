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


#include "zjobs.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::jobs;

class job_type: public job_abstract_type
{
private:
    int m_num;
public:
    job_type(int num): m_num(num) {}
    void run(void)
    {
        for (int i = 0; i < 0xFFFFFFF * m_num; ++i) continue;
        synprintf(stdout, "job %d\n", m_num);
    }
};

int main(int argc, char *argv[])
{
    // создание объектов работ
    job_type j1(1),j2(2),j3(3),j4(4),j5(5),j6(6),j7(7),j8(8);

    // создание и наполнение комплекса работ
    jobcomplex_type jobcomplex;
    // добавление списка работ
    jobcomplex.add_job(j1);
    jobcomplex.add_job(j2);
    jobcomplex.add_job(j3);
    jobcomplex.add_job(j4);
    jobcomplex.add_job(j5);
    jobcomplex.add_job(j6);
    jobcomplex.add_job(j7);
    jobcomplex.add_job(j8);
    // добавление списков зависимостей
    jobcomplex.add_dependence(j1, j5);
    jobcomplex.add_dependence(j1, j8);
    jobcomplex.add_dependence(j2, j6);
    jobcomplex.add_dependence(j2, j8);
    jobcomplex.add_dependence(j3, j2);
    jobcomplex.add_dependence(j4, j1);
    jobcomplex.add_dependence(j5, j2);
    jobcomplex.add_dependence(j5, j6);
    jobcomplex.add_dependence(j5, j8);
    jobcomplex.add_dependence(j7, j1);
    jobcomplex.add_dependence(j7, j2);
    jobcomplex.add_dependence(j7, j3);
    jobcomplex.add_dependence(j7, j4);
    jobcomplex.add_dependence(j7, j5);

    // выполнение комплекса работ
    jobcomplex.run();

    return 0;
}
