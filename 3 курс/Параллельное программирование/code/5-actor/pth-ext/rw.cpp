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


#include <set>
#include <map>
#include <list>
#include <utility>
#include "zactor.hxx"

using namespace z;
using namespace z::actor;
using namespace std;

typedef int tag_type;
typedef int data_type;

struct rrequest_type
{
    address_type customer;
    tag_type tag;
};
struct wrequest_type
{
    address_type customer;
    tag_type tag;
    data_type data;
};

struct rreply_type
{
    tag_type tag;
    data_type data;
};
struct wreply_type
{
    tag_type tag;
};

class resource_type: public actor_type
{
private:
    data_type m_data;

public:
    struct init_type
    {
        data_type data;
    };

    resource_type(const init_type &init):
        m_data(init.data)
    {
        add_action<resource_type, rrequest_type>();
        add_action<resource_type, wrequest_type>();
    }
    void action(const rrequest_type &msg)
    {
        rreply_type rep = { msg.tag, m_data };
        send(msg.customer, rep);
    }
    void action(const wrequest_type &msg)
    {
        init_type init = { msg.data };
        become<resource_type>(init);

        wreply_type rep = { msg.tag };
        send(msg.customer, rep);
    }
};

class serializer_type: public actor_type
{
private:
    // почтовый адрес контролируемого ресурса
    address_type m_resource;


    // группы читателей и писателей
    typedef multimap<tag_type, address_type> crowd_type;
    crowd_type m_rcrowd, m_wcrowd;

    // элемент очереди запросов на обслуживание
    struct request_type
    {
        enum { READ, WRITE } type;
        address_type customer;
        tag_type tag;
        data_type data;
    };
    // очередь запросов на обслуживание
    typedef list<request_type> queue_type;
    queue_type m_queue;

public:
    struct init_type
    {
        address_type resource;
    };

    struct next_type {};

    serializer_type(const init_type &init):
        m_resource(init.resource)
    {
        add_action<serializer_type, rrequest_type>();
        add_action<serializer_type, wrequest_type>();
        add_action<serializer_type, next_type>();
        add_action<serializer_type, rreply_type>();
        add_action<serializer_type, wreply_type>();
    }

    // прием и помещение в очередь запросов на чтение и запись
    void action(const rrequest_type &msg)
    {
        request_type r = {
            request_type::READ,
            msg.customer,
            msg.tag
        };
        m_queue.push_back(r);

        send(self(), next_type());
    }
    void action(const wrequest_type &msg)
    {
        request_type r = {
            request_type::WRITE,
            msg.customer,
            msg.tag,
            msg.data
        };
        m_queue.push_back(r);

        send(self(), next_type());
    }

    // попытка отправки очередного запроса ресурсу
    void action(const next_type &msg)
    {
        // в очереди должен быть хотя бы один элемент
        if (!m_queue.empty())
        {
            request_type &r = m_queue.front();
            switch (r.type)
            {
            case request_type::READ:
                // если запрос на чтение, группа писателей должна быть пустой
                if (m_wcrowd.empty())
                {
                    rrequest_type req = { self(), r.tag };
                    send(m_resource, req);
                    synprintf(stdout, "READER %d begin\n", r.tag);

                    m_rcrowd.insert(crowd_type::value_type(r.tag, r.customer));
                    m_queue.pop_front();

                    send(self(), next_type());
                };
                break;
            case request_type::WRITE:
                // если запрос на запись, обе группы должны быть пустыми
                if (m_wcrowd.empty() && m_rcrowd.empty())
                {
                    wrequest_type req = { self(), r.tag, r.data };
                    send(m_resource, req);
                    synprintf(stdout, "WRITER %d begin: %d\n", r.tag, r.data);

                    m_wcrowd.insert(crowd_type::value_type(r.tag, r.customer));
                    m_queue.pop_front();

                    send(self(), next_type());
                };
                break;
            };
        };
    }

    // прием и переправка ответов от ресурса
    void action(const rreply_type &msg)
    {
        crowd_type::iterator it = m_rcrowd.find(msg.tag);
        assert(it != m_rcrowd.end());
        // переправляем ответ читателю
        send(it->second, msg);
        // удаляем читателя из группы
        m_rcrowd.erase(it);

        send(self(), next_type());
        synprintf(stdout, "READER %d end: %d\n", msg.tag, msg.data);
    }
    void action(const wreply_type &msg)
    {
        crowd_type::iterator it = m_wcrowd.find(msg.tag);
        assert(it != m_wcrowd.end());
        // переправляем ответ писателю
        send(it->second, msg);
        // удаляем писателя из группы
        m_wcrowd.erase(it);

        send(self(), next_type());
        synprintf(stdout, "WRITER %d end\n", msg.tag);
    }
};

class customer_type: public actor_type
{
private:
    int m_id;
    address_type m_resource;

public:
    struct init_type
    {
        int id;
        address_type resource;
    };

    struct next_type {};

    customer_type(const init_type &init):
        m_id(init.id), m_resource(init.resource)
    {
        add_action<customer_type, next_type>();
        add_action<customer_type, rreply_type>();
        add_action<customer_type, wreply_type>();
    }
    void action(const next_type &msg)
    {
        if (random(5) > 0)
        {
            rrequest_type req = { self(), m_id };
            send(m_resource, req);
        }
        else
        {
            wrequest_type req = { self(), m_id, random(100) };
            send(m_resource, req);
        };
    }
    void action(const rreply_type &msg)
    {
        send(self(), next_type());
    }
    void action(const wreply_type &msg)
    {
        send(self(), next_type());
    }
};

class main_type: public actor_type
{
public:
    struct start_type {};

    main_type(const empty_type &init)
    {
        add_action<main_type, start_type>();
    }
    void action(const start_type &msg)
    {
        enum { CUSTOMER_NUMBER = 16 };
        // создание ресурса с нулевым начальным значением
        resource_type::init_type rinit = {0};
        address_type resource = create<resource_type>(rinit);
        // создание синхронизатора
        serializer_type::init_type sinit = { resource };
        address_type serializer = create<serializer_type>(sinit);
        // создание потребителей
        for (int i = 0; i < CUSTOMER_NUMBER; ++i)
        {
            customer_type::init_type cinit = { i, serializer };
            send(create<customer_type>(cinit), customer_type::next_type());
        };
    }
};

int main(int argc, char *argv[])
{
    factory_type factory;
    factory.add_definition<resource_type, resource_type::init_type>();
    factory.add_definition<serializer_type, serializer_type::init_type>();
    factory.add_definition<customer_type, customer_type::init_type>();
    factory.add_definition<main_type, actor_type::empty_type>();

    scheduler_type sched;
    sched.system().send(
        sched.system().create<main_type>(),
        main_type::start_type());
    sched.evolve(factory);

    return 0;
}
