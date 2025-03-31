using System;

namespace AbstractClasses
{
    abstract public class Vehicle: IComparable
    {
        public double Cost{get;set;}
        public int Rel_year{get;set;}
        public int Pas_num{get;set;}
        public string Name{get;set;}
        public string Comp{get;set;}

        abstract public string PrintVehicle();
        //abstract public bool Equels(object? obj);

        public int CompareTo(object? o)
        {
            Vehicle temp = (Vehicle)o;
            if (this.Cost > temp.Cost) return 1;
            if (this.Cost < temp.Cost) return -1;
            return 0;
        }

        public bool SearchByCapacity(int capacity)
        {
            if (this.Pas_num >= capacity) return true;
            return false;
        }
    }

    class SortByRelYear: IComparer<Vehicle>
    {
        public int Compare(Vehicle? veh1, Vehicle? veh2)
        {
            if (veh1.Rel_year > veh2.Rel_year) return 1;
            if (veh1.Rel_year < veh2.Rel_year) return -1;
            return 0;
        }
    }

    class SortByPasNum: IComparer<Vehicle>
    {
        public int Compare(Vehicle? veh1, Vehicle? veh2)
        {
            if (veh1.Pas_num > veh2.Pas_num) return 1;
            if (veh1.Pas_num < veh2.Pas_num) return -1;
            return 0;
        }
    }
}