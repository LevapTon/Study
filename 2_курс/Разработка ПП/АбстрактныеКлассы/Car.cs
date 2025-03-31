using System;

namespace AbstractClasses
{
    class Car: Vehicle
    {
        public Car()
        {
            this.Comp = "Неизвестно";
            this.Name = "Неизвестно";
            this.Rel_year = 1768;
            this.Pas_num = 0;
            this.Cost = 0;
        }
        public Car(string comp, string name, int rel_year, int pas_num, double cost)
        {
            this.Comp = comp;
            this.Name = name;
            this.Rel_year = rel_year;
            this.Pas_num = pas_num;
            this.Cost = cost;
        }

        public override string PrintVehicle()
        {
            return $"Автомобиль {Name}\nПроизводитель {Comp, -30}Кол-во мест {Pas_num, -5}Год выпуска {Rel_year, -6} г. Стоимость {Cost, -12} руб.";
        }

        public override bool Equals(object? obj)
        {
            if (obj is Vehicle vehicle) return Name == vehicle.Name;
            return false;
        }

        public override int GetHashCode() => Name.GetHashCode();
    }
}