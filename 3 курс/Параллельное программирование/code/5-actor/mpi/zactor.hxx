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


// генерация псевдослучайного числа в интервале [0, bound)
inline int random(int bound)
{
    assert(bound > 0);
    return int(rand() / (RAND_MAX + 1.0) * bound);
}

class arbiter_type;
class factory_type;
class scheduler_type;
class dispatcher_type;

// почтовый адрес актера
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

// базовый класс актера
class actor_type
{
private:
    // === внутренние типы и общие функции для всех классов ===
    friend class factory_type;
    friend class arbiter_type;
    friend class dispatcher_type;
    friend class scheduler_type;

    // идентификатор поведения актера
    typedef std::string defid_type;
    // идентификатор образца сообщения
    typedef std::string patid_type;
    // параметры создания актера или сообщения,
    // преобразованные к массиву байтов
    typedef std::vector<char> chunk_type;

    // структура поведения актера
    struct behaviour_type
    {
        // идентификатор поведения
        defid_type defid;
        // параметры создания актера
        chunk_type chunk;
    };

    // структура сообщения
    struct task_type
    {
        // адрес назначения
        address_type address;
        // идентификатор образца
        patid_type patid;
        // параметры сообщения
        chunk_type chunk;
    };

    // преобразование структуры параметров в массив байтов и обратно
    template <typename param_type>
    static
    chunk_type param2chunk(const param_type &p)
    {
        typedef chunk_type::value_type data_type;
        assert(sizeof(param_type) % sizeof(data_type) == 0);
        const data_type &c = reinterpret_cast<const data_type &>(p);
        int size = sizeof(param_type) / sizeof(data_type);
        // читаем передаваемый параметр как массив
        return chunk_type(&c, &c + size);
    }
    template <typename param_type>
    static
    const param_type & chunk2param(const chunk_type &chunk)
    {
        return reinterpret_cast<const param_type &>(chunk.front());
    }

    // генерация идентификатора поведения по типам
    template <typename someactor_type, typename init_type>
    static
    defid_type def_id(void)
    {
        // проверка соответствия параметров конструктора
        assert(1 ? 1 : (delete new someactor_type(init_type()), 0));
        // составной идентификатор конструктор-параметр
        return
            defid_type(typeid(someactor_type).name()) +
            "|" +
            defid_type(typeid(init_type).name());
    }

    // генерация идентификатора образца сообщения по его типу
    template <typename pattern_type>
    static
    patid_type pat_id(void)
    {
        return patid_type(typeid(pattern_type).name());
    }

public:
    // === типы, используемые клиентским кодом ===

    // "пустой" тип параметров инициализации, используется
    // для краткой записи создания актера без параметров
    class empty_type {};
    // неопределенный тип сообщения, используется для обработки
    // сообщений, не соответствующих ни одному образцу
    class unknown_type
    {
        friend class actor_type;
        task_type task;
    };

private:
    // указатель на функцию-активатор
    typedef void (*activator_type)(actor_type *, const task_type &);
    // таблица активаторов
    typedef std::map<patid_type, activator_type> activatortable_type;
    activatortable_type m_acttable;

    // привязки актера к арбитру и планировщику
    scheduler_type *m_sched;
    arbiter_type *m_arbiter;

    // шаблон активатора обработки конкретного сообщения
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
    // шаблон активатора обработки "прочих" сообщений
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

    // функции инстанцирования нужного активатора
    // на основе переданных типов
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

    // тела этих функций описаны после арбитра и планировщика
    inline address_type raw_create(const behaviour_type &behaviour) const;
    inline void raw_become(const behaviour_type &behaviour) const;
    inline void raw_send(const task_type &task) const;
    inline address_type raw_self(void) const;

    // === интерфейс для arbiter_type и scheduler_type ===
    // привязка только что созданного актера
    void bind(scheduler_type *sched, arbiter_type *arbiter)
    {
        m_sched = sched;
        m_arbiter = arbiter;
    }
    // проверка наличия активатора по сообщению
    bool match(const task_type &task) const
    {
        return
            m_acttable.find(task.patid) != m_acttable.end() ||
            m_acttable.find(pat_id<unknown_type>()) != m_acttable.end();
    }
    // запуск активатора по сообщению
    void apply(const task_type &task)
    {
        assert(task.address == self());
        activatortable_type::iterator it;
        // найдем обработчик по образцу
        it = m_acttable.find(task.patid);
        // если нет, поищем обработчик по умолчанию
        if (it == m_acttable.end())
            it = m_acttable.find(pat_id<unknown_type>());
        // обработчик должен быть найден, запустим его
        assert(it != m_acttable.end());
        it->second(this, task);
    }

protected:
    // === интерфейс для клиентского кода ===

    // конструктор
    actor_type(void):
        m_sched(NULL), m_arbiter(NULL)
    {}

    // добавление обработчика сообщения заданного образца
    template <typename someactor_type, typename pattern_type>
    void add_action(void)
    {
        patid_type patid = pat_id<pattern_type>();
        // такого обработчика пока не должно быть
        assert(m_acttable.find(patid) == m_acttable.end());
        // получим обработчик на основе текущих типов
        m_acttable[patid] = get_activator(
            static_cast<const someactor_type *>(NULL),
            static_cast<const pattern_type *>(NULL));
    }

    // выполнение операции become с параметрами
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
    // то же без параметров
    template <typename someactor_type>
    void become(void) const
    {
        become<someactor_type>(empty_type());
    }

    // получение адреса текущего актера
    address_type self(void) const
    {
        return raw_self();
    }

public:
    // выполнение операции create с параметрами
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
    // то же без параметров
    template <typename someactor_type>
    address_type create(void) const
    {
        return create<someactor_type>(empty_type());
    }

    // выполнение отправки некоторого сообщения
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
    // перенаправление "прочего" сообщения
    void send(const address_type &address, const unknown_type &msg) const
    {
        task_type task = msg.task;
        task.address = address;
        raw_send(task);
    }
};

// класс фабрики актеров
class factory_type
{
private:
    typedef actor_type::defid_type defid_type;
    typedef actor_type::chunk_type chunk_type;
    typedef actor_type::behaviour_type behaviour_type;

    // тип указателя на функцию создания актера
    typedef actor_type *(*creator_type)(const chunk_type &chunk);
    // тип указателя на функцию уничтожения актера
    typedef void (*destroyer_type)(actor_type *actor);

    typedef std::map<defid_type, creator_type> creatortable_type;
    typedef std::map<defid_type, destroyer_type> destroyertable_type;
    creatortable_type m_ctortable;
    destroyertable_type m_dtortable;

    // функции создания и уничтожения актеров заданного типа
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

    // === интерфейс для arbiter_type ===
    friend class arbiter_type;
    // создание актера
    actor_type *create_actor(const behaviour_type &behaviour) const
    {
        // ищем функцию создания актера с заданным поведением
        creatortable_type::const_iterator it;
        it = m_ctortable.find(behaviour.defid);
        // убедимся, что поведение было внесено в фабрику
        assert(it != m_ctortable.end());
        // создаем актера с заданными параметрами
        actor_type *actor = (it->second)(behaviour.chunk);
        return actor;
    }
    void destroy_actor(
        const behaviour_type &behaviour,
        actor_type *actor) const
    {
        // ищем по заданному поведению функцию уничтожения
        destroyertable_type::const_iterator it;
        it = m_dtortable.find(behaviour.defid);
        // убедимся, что поведение было внесено в фабрику
        assert(it != m_dtortable.end());
        // и уничтожаем актера
        (it->second)(actor);
    }

public:
    // === интерфейс для клиентского кода ===

    // добавление поведения в фабрику
    template <typename someactor_type, typename init_type>
    void add_definition(void)
    {
        // инстанцируем функции создания и уничтожения актера
        // и запоминаем их в соответствии с поведением
        defid_type defid = actor_type::def_id<someactor_type, init_type>();
        m_ctortable[defid] = &factory_type::template
            construct<someactor_type, init_type>;
        m_dtortable[defid] = &factory_type::template
            destruct<someactor_type>;
    }
};

// арбитр, клиентскому коду интерфейса не предоставляет
class arbiter_type
{
private:
    typedef actor_type::behaviour_type behaviour_type;
    typedef actor_type::task_type task_type;
    typedef std::list<task_type> mailbox_type;

    // фабрика для создания и уничтожения актеров
    const factory_type &m_factory;
    // личный адрес
    address_type m_self;
    // планировщик текущего арбитра
    scheduler_type *m_sched;
    // список замещающих поведений
    std::list<behaviour_type> m_behaviour;
    // текущий актер
    actor_type *m_actor;
    // почтовый ящик
    mailbox_type m_mailbox;

    // === интерфейс для actor_type ===
    friend class actor_type;
    // задание нового поведения
    void new_behaviour(const behaviour_type &behaviour)
    {
        // в момент вызова должен быть ровно один элемент
        assert(m_behaviour.size() == 1);
        m_behaviour.push_back(behaviour);
    }
    // получение адреса почтового ящика
    const address_type & address(void) const
    {
        return m_self;
    }

    // === интерфейс для scheduler_type ===
    friend class scheduler_type;
    // конструктор
    arbiter_type(
        const factory_type &factory,
        const address_type &self,
        const behaviour_type &behaviour,
        scheduler_type *sched):
        m_factory(factory), m_self(self), m_sched(sched)
    {
        // запомним начальное поведение
        m_behaviour.push_back(behaviour);
        // создадим актера
        m_actor = m_factory.create_actor(m_behaviour.front());
        // привяжем к арбитру и планировщику
        m_actor->bind(m_sched, this);
    }
    // деструктор
    ~arbiter_type(void)
    {
        m_factory.destroy_actor(m_behaviour.front(), m_actor);
    }
    // помещение сообщения в почтовый ящик
    void deliver(const task_type &task)
    {
        m_mailbox.push_back(task);
    }
    // проверка почтового ящика на пустоту
    bool empty(void) const
    {
        return m_mailbox.empty();
    }
    // извлечение очередного сообщения из ящика
    bool retrieve(task_type &task)
    {
        bool rc;
        if ((rc = !m_mailbox.empty()))
        {
            // внесение элемента недетерминированности
            mailbox_type::iterator it = m_mailbox.begin();
            advance(it, random(m_mailbox.size()));
            // проверка соответствия сообщения образцам
            if ((rc = m_actor->match(*it)))
            {
                // извлечение и удаление сообщения из ящика
                task = *it;
                m_mailbox.erase(it);
            };
        };
        return rc;
    }
    // обработка сообщения
    void process(const task_type &task)
    {
        // обработка сообщения
        m_actor->apply(task);
        // если было задано новое поведение
        if (m_behaviour.size() > 1)
        {
            // уничтожаем актера
            m_factory.destroy_actor(m_behaviour.front(), m_actor);
            // удаляем текущее поведение
            while (m_behaviour.size() > 1)
                m_behaviour.pop_front();
            // создаем и привязываем нового актера
            m_actor = m_factory.create_actor(m_behaviour.front());
            m_actor->bind(m_sched, this);
        };
    }
};

// диспетчер, клиентскому коду интерфейса не предоставляет
class dispatcher_type
{
private:
    typedef actor_type::chunk_type chunk_type;

    // тип пересылаемого запроса
    enum { REQ_CREATE, REQ_SEND };
    // структура запроса
    struct request_type
    {
        int type;
        address_type addr;
        std::string id;
        chunk_type body;
    };

    // тип упакованных данных
    typedef std::vector<char> pack_type;
    // тип очереди запросов
    typedef std::list<request_type> queue_type;
    // смещения в заголовке упакованного запроса
    enum {
        HDR_TYPE,
        HDR_ADDRRANK,
        HDR_ADDRNUM,
        HDR_IDLEN,
        HDR_BODYLEN,
        HEADERLENGTH
    };

    // количество процессов и ранг текущего
    int m_size, m_rank;
    // отображение адресов на абсолютные номера арбитров
    std::vector<std::vector<int> > m_addrmap;
    // счетчики созданных адресов в текущем процессе и во всех
    int m_count, m_full;
    // выходные очереди
    std::vector<queue_type> m_sque;
    // входная очередь
    queue_type m_rque;

    // упаковка очереди для отправки
    static
    pack_type packque(const queue_type &queue)
    {
        pack_type pack;
        int pos = 0;
        queue_type::const_iterator it;
        for (it = queue.begin(); it != queue.end(); ++it)
        {
            const request_type &req = *it;
            // формируем заголовок пакета
            int hdr[HEADERLENGTH] = {0};
            hdr[HDR_TYPE] = req.type;
            hdr[HDR_ADDRRANK] = req.addr.rank;
            hdr[HDR_ADDRNUM] = req.addr.num;
            hdr[HDR_IDLEN] = req.id.size();
            hdr[HDR_BODYLEN] = req.body.size();
            // вычисляем полный размер пакета
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
            // формируем пакет и заполняем его данными
            pack.resize(pack.size() + full);
            MPI_Pack(
                &hdr, HEADERLENGTH, MPI_INT,
                &pack.front(), pack.size(), &pos,
                MPI_COMM_WORLD);
            // вносим строку, если она не пуста
            if (hdr[HDR_IDLEN] > 0)
            {
                MPI_Pack(
                    (void *) req.id.data(), req.id.size(), MPI_CHAR,
                    &pack.front(), pack.size(), &pos,
                    MPI_COMM_WORLD);
            };
            // вносим тело, также если не пусто
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

    // распаковка принятых данных и формирование очереди
    static
    queue_type unpackque(const pack_type &pack)
    {
        queue_type queue;
        int pos = 0;
        while (size_t(pos) < pack.size())
        {
            request_type req;
            // распаковываем заголовок
            int hdr[HEADERLENGTH] = {0};
            MPI_Unpack(
                (void *) &pack.front(), pack.size(), &pos,
                &hdr, HEADERLENGTH, MPI_INT,
                MPI_COMM_WORLD);
            // сохраняем данные заголовка
            req.type = hdr[HDR_TYPE];
            req.addr.rank = hdr[HDR_ADDRRANK];
            req.addr.num = hdr[HDR_ADDRNUM];
            // строку формируем через промежуточный буфер
            if (hdr[HDR_IDLEN] > 0)
            {
                std::vector<char> buf(hdr[HDR_IDLEN]);
                MPI_Unpack(
                    (void *) &pack.front(), pack.size(), &pos,
                    &buf.front(), buf.size(), MPI_CHAR,
                    MPI_COMM_WORLD);
                req.id = std::string(buf.begin(), buf.end());
            };
            // тело в байтах получаем напрямую
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

    // === интерфейс для scheduler_type ===
    friend class scheduler_type;
    // конструктор
    dispatcher_type(void):
        m_count(0), m_full(0)
    {
        MPI_Comm_size(MPI_COMM_WORLD, &m_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
        m_addrmap.resize(m_size);
        m_sque.resize(m_size);
    }

    // создание нового уникального адреса
    address_type newaddr(void)
    {
        address_type newaddr;
        newaddr.rank = m_rank;
        newaddr.num = m_count++;
        return newaddr;
    }
    // межпроцессная репликация адресов
    void replicate(void)
    {
        // собираем счетчики из всех процессов
        std::vector<int> allcnt(m_size);
        MPI_Allgather(
            &m_count, 1, MPI_INT,
            &allcnt.front(), 1, MPI_INT,
            MPI_COMM_WORLD);
        // дополняем карту новыми абсолютными номерами
        for (int rank = 0; rank < m_size; ++rank)
        {
            int oldcnt = m_addrmap[rank].size();
            m_addrmap[rank].resize(allcnt[rank]);
            for (int i = oldcnt; i < allcnt[rank]; ++i)
                m_addrmap[rank][i] = m_full++;
        };
    }
    // размещение (ранг процесса) арбитра с заданным адресом
    int location(const address_type &address) const
    {
        assert(address.rank >= 0 && address.rank < m_size);
        assert(
            address.num >= 0 &&
            size_t(address.num) < m_addrmap[address.rank].size());
        return m_addrmap[address.rank][address.num] % m_size;
    }
    // номер арбитра в содержащем его процессе
    int index(const address_type &address) const
    {
        assert(location(address) == m_rank);
        return m_addrmap[address.rank][address.num] / m_size;
    }

    // помещение запроса в одну из выходных очередей
    void send(const request_type &req)
    {
        m_sque[location(req.addr)].push_back(req);
    }
    // изъятие запроса из входной очереди
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
    // межпроцессный обмен запросами
    // опустошаются выходные очереди, заполняется входная
    void dispatch(void)
    {
        // отправляемые и принимаемые данные
        pack_type sdata, rdata;
        // размеры и смещения отправляемых порций данных
        std::vector<int> ssize(m_size), spos(m_size);
        // размеры и смещения принимаемых порций данных
        std::vector<int> rsize(m_size), rpos(m_size);

        // пробегаем по всем выходным очередям
        for (int rank = 0; rank < m_size; ++rank)
        {
            // пакуем и чистим очередь
            pack_type pack = packque(m_sque[rank]);
            m_sque[rank].clear();
            // добавляем к отправляемым данным
            sdata.insert(sdata.end(), pack.begin(), pack.end());
            // сохраняем размер
            ssize[rank] = pack.size();
            // и смещение от начала отправляемых данных
            spos[rank] = !rank ? 0 : spos[rank - 1] + ssize[rank - 1];
        };
        // производим обмен размерами отправляемых порций данных
        // результат - размеры принимаемых порций данных
        MPI_Alltoall(
            &ssize.front(), 1, MPI_INT,
            &rsize.front(), 1, MPI_INT,
            MPI_COMM_WORLD);
        // формируем массив и смещения для принятия данных
        for (int rank = 0; rank < m_size; ++rank)
        {
            rdata.resize(rdata.size() + rsize[rank]);
            rpos[rank] = !rank ? 0 : rpos[rank - 1] + rsize[rank - 1];
        };

        // добавляем по байту для безопасного обращения к front()
        sdata.push_back(0);
        rdata.push_back(0);
        // производим обмен данными между всеми процессами
        MPI_Alltoallv(
            &sdata.front(), &ssize.front(), &spos.front(), MPI_PACKED,
            &rdata.front(), &rsize.front(), &rpos.front(), MPI_PACKED,
            MPI_COMM_WORLD);
        // убираем лишний байт
        sdata.pop_back();
        rdata.pop_back();

        // распаковываем и сохраняем принятую очередь запросов
        queue_type rque = unpackque(rdata);
        m_rque.insert(m_rque.end(), rque.begin(), rque.end());
    }

    // проверка занятости системы
    bool busy(bool b) const
    {
        // объединение признаков занятости из всех процессов
        int localbusy = b ? 1 : 0, allbusy;
        MPI_Allreduce(
            &localbusy, &allbusy, 1, MPI_INT,
            MPI_LOR, MPI_COMM_WORLD);
        return allbusy != 0;
    }
};

// класс планировщика
class scheduler_type
{
private:
    typedef actor_type::behaviour_type behaviour_type;
    typedef actor_type::task_type task_type;

    typedef std::vector<arbiter_type *> arbiterlist_type;
    typedef std::list<std::pair<address_type, behaviour_type> > actorqueue_type;
    typedef std::list<task_type> taskqueue_type;

    // псевдо-актер, "точка отсчета" для системы актеров
    actor_type m_origin;
    // очередь запросов на создание актеров
    actorqueue_type m_actorqueue;
    // очередь запросов на доставку сообщений
    taskqueue_type m_taskqueue;
    // список созданных арбитров
    arbiterlist_type m_arbiterlist;
    // диспетчер запросов
    dispatcher_type m_disp;

    // === интерфейс для actor_type ===
    friend class actor_type;
    // добавление запроса на создание актера
    address_type new_actor(const behaviour_type &behaviour)
    {
        // формирование нового адреса
        address_type newaddr = m_disp.newaddr();
        // добавление запроса
        m_actorqueue.push_back(
            actorqueue_type::value_type(newaddr, behaviour));
        return newaddr;
    }
    // добавление запроса на доставку сообщения
    void new_task(const task_type &task)
    {
        m_taskqueue.push_back(task);
    }

public:
    // === интерфейс для клиентского кода ===

    // конструктор
    scheduler_type(void)
    {
        // псевдо-актер привязан только к планировщику
        // арбитра у него нет, как и почтового адреса
        m_origin.bind(this, NULL);
    }

    // "точка отсчета" системы актеров
    const actor_type & system(void) const
    {
        return m_origin;
    }

    // полный цикл развития системы актеров
    void evolve(const factory_type &factory)
    {
        bool busy;
        do
        {
            // репликация новых почтовых адресов
            m_disp.replicate();

            // отправка всех запросов
            while (!m_actorqueue.empty())
            {
                // изымаем из очереди новый адрес и поведение
                dispatcher_type::request_type req = {
                    dispatcher_type::REQ_CREATE,
                    m_actorqueue.front().first,
                    m_actorqueue.front().second.defid,
                    m_actorqueue.front().second.chunk
                };
                m_actorqueue.pop_front();
                // передаем запрос диспетчеру
                m_disp.send(req);
            };
            while (!m_taskqueue.empty())
            {
                // изымаем из очереди сообщение
                dispatcher_type::request_type req = {
                    dispatcher_type::REQ_SEND,
                    m_taskqueue.front().address,
                    m_taskqueue.front().patid,
                    m_taskqueue.front().chunk
                };
                m_taskqueue.pop_front();
                // передаем запрос диспетчеру
                m_disp.send(req);
            };

            // распределение всех запросов между процессами
            m_disp.dispatch();

            // получение и обработка всех своих запросов
            dispatcher_type::request_type req;
            while (m_disp.recv(req))
            {
                if (req.type == dispatcher_type::REQ_CREATE)
                {
                    // обработка запроса на создание актера
                    behaviour_type behaviour = { req.id, req.body };
                    // создаем арбитра
                    assert(
                        size_t(m_disp.index(req.addr)) ==
                        m_arbiterlist.size());
                    m_arbiterlist.push_back(
                        new arbiter_type(factory, req.addr, behaviour, this));
                }
                else
                {
                    // обработка запроса на доставку сообщения
                    task_type task = { req.addr, req.id, req.body };
                    // помещаем его в соответствующий почтовый ящик
                    m_arbiterlist[m_disp.index(req.addr)]->deliver(task);
                };
            };

            // цикл работы системы актеров
            busy = false;
            int allnum = m_arbiterlist.size();
            for (int addr = 0; addr < allnum; ++addr)
            {
                // пополняем признак наличия сообщений в системе
                busy = busy || !m_arbiterlist[addr]->empty();
                // пытаемся получить и обработать сообщение
                task_type task;
                if (m_arbiterlist[addr]->retrieve(task))
                    m_arbiterlist[addr]->process(task);
            };
            // выход, если в системе нет сообщений
        } while (m_disp.busy(busy));

        // уничтожение арбитров
        int allnum = m_arbiterlist.size();
        for (int addr = 0; addr < allnum; ++addr)
            delete m_arbiterlist[addr];
        m_arbiterlist.clear();
        m_disp = dispatcher_type();
    }
};

// не объявленные ранее функции класса actor_type
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


// актер sink
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

// актер forwarder
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

// актер print
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
