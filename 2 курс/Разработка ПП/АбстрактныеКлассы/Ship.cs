using System;

namespace AbstractClasses
{
    class Ship: Vehicle
    {
        string home_port = "Без порта";

        public string Home_port
        {
            get
            {
                return home_port;
            }
            set
            {
                home_port = value; 
            }
        }

        public Ship()
        {
            this.Comp = "Неизвестно";
            this.Name = "Неизвестно";
            this.Home_port = "Неизвестно";
            this.Rel_year = 1858;
            this.Pas_num = 0;
            this.Cost = 0;
        }
        public Ship(string comp, string name, string homePort, int rel_year, int pas_num, double cost)
        {
            this.Comp = comp;
            this.Name = name;
            this.Home_port = homePort;
            this.Rel_year = rel_year;
            this.Pas_num = pas_num;
            this.Cost = cost;
        }

        public override string PrintVehicle()
        {
            string str1 = $"Корабль {Name}\nПроизводитель {Comp, -30}Кол-во мест {Pas_num, -5}";
            string str2 = $"Год выпуска {Rel_year, -6} г. Стоимость {Cost, -12} руб.  Порт приписки {Home_port}";
            return str1 + str2;
        }

        public override bool Equals(object? obj)
        {
            if (obj is Vehicle vehicle) return Name == vehicle.Name;
            return false;
        }

        public override int GetHashCode() => Name.GetHashCode();
    }
}