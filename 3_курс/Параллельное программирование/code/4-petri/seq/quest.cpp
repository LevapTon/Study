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


#include <string>
#include <deque>
#include "zpetri.hxx"
#include "../../common/synprintf.hxx"

using namespace std;
using namespace z;
using namespace z::petri;


// ������� ���� ����� (��������� ������-���� �������� � ����)
class gstate_type: public place_type
{
private:
    int type;
public:
    enum {
        OUTSPACE,   // ����� �������
        ROOM1,      // ����� � ������� 1
        ROOM2,      // ����� � ������� 2
        ROOM3,      // ����� � ������� 3
        ROOM4,      // ����� � ������� 4
        SWOFF,      // ����� � ��������� "����"
        SWON,       // ����� � ��������� "���"
        KLYING,     // ���� �� ����
        KTAKEN,     // ���� � �����
        KUSED,      // ����� ����������� ����
        DLYING,     // ���� �� ����
        DTAKEN,     // ���� ����� ������
        DHAVENO,    // � ����� ��� ���� (����� �����)
        DPLACED,    // ���� �������� �� ����
        NUMBER      // ������ ���������� �������
    };
    explicit
    gstate_type(int t): type(t)
    {
        assert(type >=0 && type < NUMBER);
    }
    // ...
    string what(void) const
    {
        const char *name[] = {
            "we are in outspace",
            "we are in room 1",
            "we are in room 2",
            "we are in room 3",
            "we are in room 4",
            "switch is off",
            "switch is on",
            "key is somewere on the floor",
            "we have a key",
            "key is used",
            "dumbbell is somewere on the floor",
            "we have a dumbbell",
            "we can take a dumbbell",
            "dumbbell is placed on the scale platform"
        };
        return name[type];
    }
};

// ������� ���� ����� (�����-���� �������� � ����)
class gaction_type: public transition_simple_type
{
private:
    int type;
public:
    enum {
        EXIT,       // �������� ���������
        LEFT21,     // ������ ����� �� 2 ������� � 1
        RIGHT12,    // ������ ������ �� 1 ������� �� 2
        LEFT34,     // ������ ����� �� 3 ������� � 4
        RIGHT43,    // ������ ������ �� 4 ������� � 3
        UP32,       // ��������� �� 3 ������� �� 2
        DOWN23,     // ���������� �� 2 ������� � 3
        TURNON,     // ��������� ����� � ��������� "���"
        TURNOFF,    // ��������� ����� � ��������� "����"
        GETKEY,     // ����� ����
        USEKEY,     // �������� �����
        GETDMB,     // ����� ����
        PUTDMB,     // �������� ���� �� ����
        NUMBER      // ������ ���������� ���������
    };
    explicit
    gaction_type(int t): type(t)
    {
        assert(type >=0 && type < NUMBER);
    }
    // ...
    string what(void) const
    {
        const char *name[] = {
            "exit",
            "move left",
            "move right",
            "move left",
            "move right",
            "move up",
            "move down",
            "turn switch on",
            "turn switch off",
            "find the key",
            "open the door",
            "get dumbbell",
            "put dumbbell"
        };
        return name[type];
    }
};

// ��������� ���� ����� (�������������� ���� � �������)
class genv_type: public petrinet_type::environment_abstract_type
{
public:
    int wait(
        const petrinet_type::enabledlist_type &enabled,
        const petrinet_type::markedlist_type &marked)
    {
        // ����������� ������ �������� ��������� ����
        // (�� ������ ����������� marked)
        // ...
        {
            petrinet_type::markedlist_type::const_iterator it;
            for (it = marked.begin(); it != marked.end(); ++it)
                for (int i = 0; i < it->second; ++i)
                    fprintf(stdout, "%s\n",
                        static_cast<gstate_type *>(it->first)->what().c_str());
        };

        // ����������� ������ ��������� ��������� ��������
        // (�� ������ ����������� enabled)
        // ...
        {
            petrinet_type::enabledlist_type::const_iterator it;
            for (it = enabled.begin(); it != enabled.end(); ++it)
                fprintf(stdout, "%d: %s\n",
                    std::distance(enabled.begin(), it),
                    static_cast<gaction_type *>(*it)->what().c_str());
        };

        // ��������� �� ������ ������ ���������� ��������
        // int choice = ...
        int choice = -1;
        do
        {
            assert(!feof(stdin));
            fputs("action number> ", stdout);
            char buf[256] = "";
            fgets(buf, sizeof(buf) / sizeof(buf[0]), stdin);
            sscanf(buf, "%d\n", &choice);
        } while (choice < 0 || size_t(choice) >= enabled.size());
        // ������� ������ ��� �����
        fprintf(stdout, "ok, action %d: %s\n\n",
            choice,
            static_cast<gaction_type *>(enabled[choice])->what().c_str());

        return choice;
    }
};

int main(int argc, char *argv[])
{
    // ���������� ����������� ���� �����
    petrinet_type::content_type content;

    // �������� ��������-�������
    deque<gstate_type> p;
    for (int i = 0; i < gstate_type::NUMBER; ++i)
    {
        p.push_back(gstate_type(i));
        content.add_place(p.back());
    };
    // �������� ��������-���������
    deque<gaction_type> t;
    for (int i = 0; i < gaction_type::NUMBER; ++i)
    {
        t.push_back(gaction_type(i));
        content.add_transition(t.back());
    };

    // ���� ��������� ����������� �����
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::RIGHT12]);
    content.add_arc(t[gaction_type::RIGHT12], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::LEFT21]);
    content.add_arc(t[gaction_type::LEFT21], p[gstate_type::ROOM1]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::DOWN23]);
    content.add_arc(t[gaction_type::DOWN23], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::UP32]);
    content.add_arc(t[gaction_type::UP32], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::LEFT34]);
    content.add_arc(t[gaction_type::LEFT34], p[gstate_type::ROOM4]);
    content.add_arc(p[gstate_type::ROOM4], t[gaction_type::RIGHT43]);
    content.add_arc(t[gaction_type::RIGHT43], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::EXIT]);
    content.add_arc(t[gaction_type::EXIT], p[gstate_type::OUTSPACE]);
    // ����������� ����������� � ����������� �� ��������� ������
    content.add_arc(p[gstate_type::KUSED], t[gaction_type::LEFT21]);
    content.add_arc(t[gaction_type::LEFT21], p[gstate_type::KUSED]);
    //~ content.add_arc(p[gstate_type::KUSED], t[gaction_type::RIGHT12]);
    //~ content.add_arc(t[gaction_type::RIGHT12], p[gstate_type::KUSED]);
    content.add_arc(p[gstate_type::SWON], t[gaction_type::LEFT34]);
    content.add_arc(t[gaction_type::LEFT34], p[gstate_type::SWON]);
    //~ content.add_arc(p[gstate_type::SWON], t[gaction_type::RIGHT43]);
    //~ content.add_arc(t[gaction_type::RIGHT43], p[gstate_type::SWON]);
    content.add_arc(p[gstate_type::DPLACED], t[gaction_type::EXIT], 2);

    // ���� ��������� ������������ ������
    content.add_arc(p[gstate_type::SWOFF], t[gaction_type::TURNON]);
    content.add_arc(t[gaction_type::TURNON], p[gstate_type::SWON]);
    content.add_arc(p[gstate_type::SWON], t[gaction_type::TURNOFF]);
    content.add_arc(t[gaction_type::TURNOFF], p[gstate_type::SWOFF]);
    // ����������� ����������� ������������ �� ������������ �����
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::TURNON]);
    content.add_arc(t[gaction_type::TURNON], p[gstate_type::ROOM2]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::TURNOFF]);
    content.add_arc(t[gaction_type::TURNOFF], p[gstate_type::ROOM2]);

    // ���� ��������� �������� � ������
    content.add_arc(p[gstate_type::KLYING], t[gaction_type::GETKEY]);
    content.add_arc(t[gaction_type::GETKEY], p[gstate_type::KTAKEN]);
    content.add_arc(p[gstate_type::KTAKEN], t[gaction_type::USEKEY]);
    content.add_arc(t[gaction_type::USEKEY], p[gstate_type::KUSED]);
    // ����������� �� ������������ �����
    content.add_arc(p[gstate_type::ROOM3], t[gaction_type::GETKEY]);
    content.add_arc(t[gaction_type::GETKEY], p[gstate_type::ROOM3]);
    content.add_arc(p[gstate_type::ROOM2], t[gaction_type::USEKEY]);
    content.add_arc(t[gaction_type::USEKEY], p[gstate_type::ROOM2]);

    // ���� ��������� �������� � ������
    content.add_arc(p[gstate_type::DLYING], t[gaction_type::GETDMB]);
    content.add_arc(p[gstate_type::DHAVENO], t[gaction_type::GETDMB]);
    content.add_arc(t[gaction_type::GETDMB], p[gstate_type::DTAKEN]);
    content.add_arc(p[gstate_type::DTAKEN], t[gaction_type::PUTDMB]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::DHAVENO]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::DPLACED]);
    // ����������� �� ������������
    content.add_arc(p[gstate_type::ROOM1], t[gaction_type::GETDMB]);
    content.add_arc(t[gaction_type::GETDMB], p[gstate_type::ROOM1]);
    content.add_arc(p[gstate_type::ROOM4], t[gaction_type::PUTDMB]);
    content.add_arc(t[gaction_type::PUTDMB], p[gstate_type::ROOM4]);

    // ��������� ��������
    content.add_token(p[gstate_type::DLYING], 2);
    content.add_token(p[gstate_type::DHAVENO]);
    content.add_token(p[gstate_type::KLYING]);
    content.add_token(p[gstate_type::SWOFF]);
    content.add_token(p[gstate_type::ROOM2]);

    // �������� � ���������� ���� �����
    genv_type env;
    petrinet_type petrinet(content);
    petrinet.live(env);

    return 0;
}
